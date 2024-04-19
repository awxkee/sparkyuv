// Copyright (C) 2024 Radzivon Bartoshyk
//
// This file belongs to sparkyuv project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "sparkyuv-def.h"
#include "sparkyuv-internal.h"
#include "PixelLoader.h"
#include "concurrency.hpp"

namespace sparkyuv {

template<typename T>
void VerticalGaussianPassChannel(T *data, const int stride, const int width, const int height, const int radius) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  int buffer[bufLength + 1];

  for (int x = 0; x < width; ++x) {
    int dif = 0, sum = (radius * radius) >> 1;
    for (int y = 0 - 2 * radius; y < height; ++y) {
      auto src = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      if (y >= 0) {
        int arrIndex = (y - radius) & bufLength;
        int dArrIndex = y & bufLength;
        T pixel = static_cast<T>(static_cast<float>(sum) * weight);
        src[x] = pixel;
        dif += buffer[arrIndex] - 2 * buffer[dArrIndex];
      } else if (y + radius >= 0) {
        int dArrIndex = y & bufLength;
        dif -= 2 * buffer[dArrIndex];
      }

      auto srcNext =
          reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + std::clamp(y + radius, 0, height - 1) * stride);
      int p = srcNext[x];
      sum += dif += p;
      int arrIndex = (y + radius) & bufLength;
      buffer[arrIndex] = p;
    }
  }
}

template<typename T>
void HorizontalGaussianPassChannel(T *data, const int stride, const int width, const int height, const int radius) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  int buffer[bufLength + 1];

  for (int y = 0; y < height; ++y) {
    int dif = 0, sum = (radius * radius) >> 1;
    for (int x = 0 - 2 * radius; x < width; ++x) {
      auto src = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      if (x >= 0) {
        int arrIndex = (x - radius) & bufLength;
        int dArrIndex = x & bufLength;
        T pixel = static_cast<T>(static_cast<float>(sum) * weight);
        src[x] = pixel;
        dif += buffer[arrIndex] - 2 * buffer[dArrIndex];
      } else if (x + radius >= 0) {
        int dArrIndex = x & bufLength;
        dif -= 2 * buffer[dArrIndex];
      }

      auto srcNext = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      int p = srcNext[std::clamp(x + radius, 0, width - 1)];
      sum += dif += p;
      int arrIndex = (x + radius) & bufLength;
      buffer[arrIndex] = p;
    }
  }
}

template<typename T, SparkYuvDefaultPixelType PixelType>
void VerticalGaussianPass(T *data, const int stride,
                          const int width, const int height,
                          const int radius, const int start,
                          const int end) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  int bufferR[bufLength + 1], bufferG[bufLength + 1], bufferB[bufLength + 1];

  const int channels = getPixelTypeComponents(PixelType);

  for (int x = start; x < width && x < end; ++x) {
    int difR = 0, sumR = (radius * radius) >> 1, difG = 0, sumG = (radius * radius) >> 1,
        difB = 0, sumB = (radius * radius) >> 1;
    for (int y = 0 - 2 * radius; y < height; ++y) {
      auto src = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      if (y >= 0) {
        auto store = src + x * channels;
        const T newR = static_cast<T>(static_cast<float>(sumR) * weight);
        int arrIndex = (y - radius) & bufLength;
        int dArrIndex = y & bufLength;
        difR += bufferR[arrIndex] - 2 * bufferR[dArrIndex];

        const T newG = static_cast<T>(static_cast<float>(sumG) * weight);
        difG += bufferG[arrIndex] - 2 * bufferG[dArrIndex];

        const T newB = static_cast<T>(static_cast<float>(sumB) * weight);
        difB += bufferB[arrIndex] - 2 * bufferB[dArrIndex];

        StoreRGB<T, T, PixelType>(store, newR, newG, newB);
      } else if (y + radius >= 0) {
        int arrIndex = (y) & bufLength;
        difR -= 2 * bufferR[arrIndex];
        difG -= 2 * bufferG[arrIndex];
        difB -= 2 * bufferB[arrIndex];
      }

      auto srcNext =
          reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + std::clamp(y + radius, 0, height - 1) * stride);
      srcNext += x * channels;

      int pR, pG, pB, pA;
      LoadRGBA<T, int, PixelType>(srcNext, pR, pG, pB, pA);

      int arrIndex = (y + radius) & bufLength;

      sumR += difR += pR;
      bufferR[arrIndex] = pR;
      sumG += difG += pG;
      bufferG[arrIndex] = pG;
      sumB += difB += pB;
      bufferB[arrIndex] = pB;
    }
  }
}

template<typename T, SparkYuvDefaultPixelType PixelType>
void HorizontalGaussianPass(T *data, const int stride, const int width, const int height, const int radius,
                            const int startY, const int endY) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  int bufferR[bufLength + 1], bufferG[bufLength + 1], bufferB[bufLength + 1];

  const int channels = getPixelTypeComponents(PixelType);

  for (int y = startY; y < height && y < endY; ++y) {
    int difR = 0, sumR = (radius * radius) >> 1, difG = 0, sumG = (radius * radius) >> 1,
        difB = 0, sumB = (radius * radius) >> 1;
    for (int x = 0 - 2 * radius; x < width; ++x) {
      auto src = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      if (x >= 0) {
        auto store = src + x * channels;

        int arrIndex = (x - radius) & bufLength;
        int dArrIndex = x & bufLength;

        const T newR = static_cast<T>(static_cast<float>(sumR) * weight);
        difR += bufferR[arrIndex] - 2 * bufferR[dArrIndex];

        const T newG = static_cast<T>(static_cast<float>(sumG) * weight);
        difG += bufferG[arrIndex] - 2 * bufferG[dArrIndex];

        const T newB = static_cast<T>(static_cast<float>(sumB) * weight);
        difB += bufferB[arrIndex] - 2 * bufferB[dArrIndex];

        StoreRGB<T, T, PixelType>(store, newR, newG, newB);
      } else if (x + radius >= 0) {
        int dx = x & bufLength;
        difR -= 2 * bufferR[dx];
        difG -= 2 * bufferG[dx];
        difB -= 2 * bufferB[dx];
      }

      int arrIndex = (x + radius) & bufLength;

      auto srcNext = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      int px = std::clamp(x + radius, 0, width - 1) * channels;
      srcNext += px;

      int pR, pG, pB, pA;
      LoadRGBA<T, int, PixelType>(srcNext, pR, pG, pB, pA);

      sumR += difR += pR;
      bufferR[arrIndex] = pR;

      sumG += difG += pG;
      bufferG[arrIndex] = pG;

      sumB += difB += pB;
      bufferB[arrIndex] = pB;
    }
  }
}

#define FAST_GAUSSIAN_DECLARATION_R(pixelName, pixelType, storageType) \
    void FastGaussianBlur##pixelName(storageType *data, uint32_t stride, uint32_t width, uint32_t height, int radius) {\
      const int threadCount = concurrency::getThreadCounts(width, height);\
      concurrency::parallel_for_segment(threadCount, width, [&](int start, int end) {\
        VerticalGaussianPass<storageType, sparkyuv::PIXEL_##pixelType>(data, stride, width, height, radius, start, end);\
      });\
      concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {\
        HorizontalGaussianPass<storageType, sparkyuv::PIXEL_##pixelType>(data, stride, width, height, radius, start, end);\
      });\
    }

FAST_GAUSSIAN_DECLARATION_R(RGBA, RGBA, uint8_t)
FAST_GAUSSIAN_DECLARATION_R(RGB, RGB, uint8_t)
#if SPARKYUV_FULL_CHANNELS
FAST_GAUSSIAN_DECLARATION_R(ARGB, ARGB, uint8_t)
FAST_GAUSSIAN_DECLARATION_R(ABGR, ABGR, uint8_t)
FAST_GAUSSIAN_DECLARATION_R(BGRA, BGRA, uint8_t)
FAST_GAUSSIAN_DECLARATION_R(BGR, BGR, uint8_t)
#endif

FAST_GAUSSIAN_DECLARATION_R(RGBA16, RGBA, uint16_t)
FAST_GAUSSIAN_DECLARATION_R(RGB16, RGB, uint16_t)
#if SPARKYUV_FULL_CHANNELS
FAST_GAUSSIAN_DECLARATION_R(ARGB16, ARGB, uint16_t)
FAST_GAUSSIAN_DECLARATION_R(ABGR16, ABGR, uint16_t)
FAST_GAUSSIAN_DECLARATION_R(BGRA16, BGRA, uint16_t)
FAST_GAUSSIAN_DECLARATION_R(BGR16, BGR, uint16_t)
#endif

#undef FAST_GAUSSIAN_DECLARATION_R

#define FAST_GAUSSIAN_DECLARATION_CHAN_R(pixelName, storageType) \
    void FastGaussianBlur##pixelName(storageType *data, uint32_t stride, uint32_t width, uint32_t height, int radius) {\
      VerticalGaussianPassChannel<storageType>(data, stride, width, height, radius);\
      HorizontalGaussianPassChannel<storageType>(data, stride, width, height, radius);\
    }

FAST_GAUSSIAN_DECLARATION_CHAN_R(Channel, uint8_t)
FAST_GAUSSIAN_DECLARATION_CHAN_R(Channel16, uint16_t)

#undef FAST_GAUSSIAN_DECLARATION_CHAN_R
}