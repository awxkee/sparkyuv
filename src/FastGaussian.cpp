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
#include "hwy/aligned_allocator.h"
#include "TypeSupport.h"

namespace sparkyuv {

template<typename T, typename V>
void VerticalGaussianPassChannel(T *data, const int stride, const int width, const int height, const int radius) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  auto buffer = hwy::AllocateAligned<V>(bufLength + 1);

  for (int x = 0; x < width; ++x) {
    V dif = 0, sum = (radius * radius) >> 1;
    for (int y = 0 - 2 * radius; y < static_cast<int>(height); ++y) {
      auto src = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      if (y >= 0) {
        int arrIndex = (y - radius) & bufLength;
        int dArrIndex = y & bufLength;
        T pixel = TransformCast<T, float>(static_cast<float>(sum) * weight);
        src[x] = pixel;
        dif += buffer[arrIndex] - 2 * buffer[dArrIndex];
      } else if (y + radius >= 0) {
        int dArrIndex = y & bufLength;
        dif -= 2 * buffer[dArrIndex];
      }

      auto srcNext =
          reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + std::clamp(y + radius, 0, height - 1) * stride);
      V p = LoadPixel<V>(&srcNext[x]);
      sum += dif += p;
      int arrIndex = (y + radius) & bufLength;
      buffer[arrIndex] = p;
    }
  }
}

template<typename T, typename V>
void HorizontalGaussianPassChannel(T *data, const int stride, const int width, const int height, const int radius) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  auto buffer = hwy::AllocateAligned<V>(bufLength + 1);

  for (int y = 0; y < height; ++y) {
    V dif = 0, sum = (radius * radius) >> 1;
    for (int x = 0 - 2 * radius; x < static_cast<int>(width); ++x) {
      auto src = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      if (x >= 0) {
        int arrIndex = (x - radius) & bufLength;
        int dArrIndex = x & bufLength;
        T pixel = TransformCast<T, float>(static_cast<float>(sum) * weight);
        src[x] = pixel;
        dif += buffer[arrIndex] - 2 * buffer[dArrIndex];
      } else if (x + radius >= 0) {
        int dArrIndex = x & bufLength;
        dif -= 2 * buffer[dArrIndex];
      }

      auto srcNext = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      V p = LoadPixel<V>(&srcNext[std::clamp(x + radius, 0, width - 1)]);
      sum += dif += p;
      int arrIndex = (x + radius) & bufLength;
      buffer[arrIndex] = p;
    }
  }
}

template<typename T, typename V, SparkYuvDefaultPixelType PixelType>
void VerticalGaussianPass(T *data, const int stride,
                          const int width, const int height,
                          const int radius, const int start,
                          const int end) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  auto bufferR = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferG = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferB = hwy::AllocateAligned<V>(bufLength + 1);

  const int channels = getPixelTypeComponents(PixelType);

  for (int x = start; x < width && x < end; ++x) {
    V difR = 0, sumR = 0, difG = 0, sumG = 0,
        difB = 0, sumB = 0;
    if (!std::is_same<T, hwy::float16_t>::value) {
      sumR = (radius * radius) >> 1;
      sumG = (radius * radius) >> 1;
      sumB = (radius * radius) >> 1;
    }
    for (int y = 0 - 2 * radius; y < static_cast<int>(height); ++y) {
      auto mSrc = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      if (y >= 0) {
        auto store = mSrc + x * channels;
        const T newR = TransformCast<T, float>(static_cast<float>(sumR) * weight);
        int arrIndex = (y - radius) & bufLength;
        int dArrIndex = y & bufLength;
        difR += bufferR[arrIndex] - 2 * bufferR[dArrIndex];

        const T newG = TransformCast<T, float>(static_cast<float>(sumG) * weight);
        difG += bufferG[arrIndex] - 2 * bufferG[dArrIndex];

        const T newB = TransformCast<T, float>(static_cast<float>(sumB) * weight);
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

      V pR, pG, pB, pA;
      LoadRGBA<T, V, PixelType>(srcNext, pR, pG, pB, pA);

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

template<typename T, typename V, SparkYuvDefaultPixelType PixelType>
void HorizontalGaussianPass(T *data, const int stride, const int width, const int height, const int radius,
                            const int startY, const int endY) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  auto bufferR = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferG = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferB = hwy::AllocateAligned<V>(bufLength + 1);

  const int channels = getPixelTypeComponents(PixelType);

  for (int y = startY; y < height && y < endY; ++y) {
    V difR = 0, sumR = 0, difG = 0, sumG = 0,
        difB = 0, sumB = 0;
    if (!std::is_same<T, hwy::float16_t>::value) {
      sumR = (radius * radius) >> 1;
      sumG = (radius * radius) >> 1;
      sumB = (radius * radius) >> 1;
    }
    auto store = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data) + y * stride);
    for (int x = 0 - 2 * radius; x < static_cast<int>(width); ++x) {
      if (x >= 0) {
        int arrIndex = (x - radius) & bufLength;
        int dArrIndex = x & bufLength;

        const T newR = TransformCast<T, float>(static_cast<float>(sumR) * weight);
        difR += bufferR[arrIndex] - 2 * bufferR[dArrIndex];

        const T newG = TransformCast<T, float>(static_cast<float>(sumG) * weight);
        difG += bufferG[arrIndex] - 2 * bufferG[dArrIndex];

        const T newB = TransformCast<T, float>(static_cast<float>(sumB) * weight);
        difB += bufferB[arrIndex] - 2 * bufferB[dArrIndex];

        StoreRGB<T, T, PixelType>(store, newR, newG, newB);
        store += channels;
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

      V pR, pG, pB, pA;
      LoadRGBA<T, V, PixelType>(srcNext, pR, pG, pB, pA);

      sumR += difR += pR;
      bufferR[arrIndex] = pR;

      sumG += difG += pG;
      bufferG[arrIndex] = pG;

      sumB += difB += pB;
      bufferB[arrIndex] = pB;
    }
  }
}

template<typename T, typename V, SparkYuvDefaultPixelType PixelType>
void VerticalFastGaussianPassNext(T *data, const uint32_t stride, const int width, const int height,
                                  const int radius, const int start, const int end) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  auto bufferR = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferG = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferB = hwy::AllocateAligned<V>(bufLength + 1);

  const int channels = getPixelTypeComponents(PixelType);

  for (int x = start; x < static_cast<int>(width) && x < end; ++x) {
    V difR = 0, derR = 0, difG = 0, derG = 0,
        difB = 0, derB = 0;
    float sumR = 0, sumG = 0, sumB = 0;
    for (int y = 0 - 3 * radius; y < static_cast<int>(height); ++y) {
      auto store = reinterpret_cast<T*>(reinterpret_cast<uint8_t *>(data) + y * stride);
      int px = x * channels;
      if (y >= 0) {
        int mpy = y & bufLength;
        int mpyPRadius = (y + radius) & bufLength;
        int mpyMRadius = (y - radius) & bufLength;
        difR += 3 * (bufferR[mpy] - bufferR[mpyPRadius]) - bufferR[mpyMRadius];
        const T newR = TransformCast<T, float>(static_cast<float>(sumR) * weight);

        difG += 3 * (bufferG[mpy] - bufferG[mpyPRadius]) - bufferG[mpyMRadius];
        const T newG = TransformCast<T, float>(static_cast<float>(sumG) * weight);

        difB += 3 * (bufferB[mpy] - bufferB[mpyPRadius]) - bufferB[mpyMRadius];
        const T newB = TransformCast<T, float>(static_cast<float>(sumB) * weight);
        StoreRGB<T, T, PixelType>(&store[px], newR, newG, newB);
      } else if (y + radius >= 0) {
        int mpy = y & bufLength;
        int mpyPRadius = (y + radius) & bufLength;
        difR += 3 * (bufferR[mpy] - bufferR[mpyPRadius]);
        difG += 3 * (bufferG[mpy] - bufferG[mpyPRadius]);
        difB += 3 * (bufferB[mpy] - bufferB[mpyPRadius]);
      } else if (y + 2 * radius >= 0) {
        int mpyPRadius = (y + radius) & bufLength;
        difR -= 3 * bufferR[mpyPRadius];
        difG -= 3 * bufferG[mpyPRadius];
        difB -= 3 * bufferB[mpyPRadius];
      }

      int mPNextRad = (y + 2 * radius) & bufLength;

      auto srcNext = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(data)
          + std::clamp(static_cast<int>(y + 3 * radius / 2), 0, static_cast<int>(height - 1)) * stride);
      srcNext += px;

      V pR, pG, pB, pA;
      LoadRGBA<T, V, PixelType>(srcNext, pR, pG, pB, pA);

      sumR += derR += difR += pR;
      bufferR[mPNextRad] = pR;

      sumG += derG += difG += pG;
      bufferG[mPNextRad] = pG;

      sumB += derB += difB += pB;
      bufferB[mPNextRad] = pB;
    }
  }
}

template<typename T, typename V, SparkYuvDefaultPixelType PixelType>
void HorizontalFastGaussianNext(T *data, const uint32_t stride,
                                const int width, const int height, const int radius,
                                const int start, const int end) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int bufLength = 1023;
  auto bufferR = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferG = hwy::AllocateAligned<V>(bufLength + 1);
  auto bufferB = hwy::AllocateAligned<V>(bufLength + 1);

  const int channels = getPixelTypeComponents(PixelType);

  for (int y = start; y < height && y < end; ++y) {
    auto store = reinterpret_cast<T*>(reinterpret_cast<uint8_t *>(data) + y * stride);
    V difR = 0, derR = 0, difG = 0, derG = 0, difB = 0, derB = 0;
    float sumR = 0, sumG = 0, sumB = 0;
    for (int x = 0 - 3 * radius; x < width; ++x) {
      if (x >= 0) {
        int mpy = x & bufLength;
        int mpyPRadius = (x + radius) & bufLength;
        int mpyMRadius = (x - radius) & bufLength;

        difR += 3 * (bufferR[mpy] - bufferR[mpyPRadius]) - bufferR[mpyMRadius];
        const T newR = TransformCast<T, float>(static_cast<float>(sumR) * weight);

        difG += 3 * (bufferG[mpy] - bufferG[mpyPRadius]) - bufferG[mpyMRadius];
        const T newG = TransformCast<T, float>(static_cast<float>(sumG) * weight);

        difB += 3 * (bufferB[mpy] - bufferB[mpyPRadius]) - bufferB[mpyMRadius];
        const T newB = TransformCast<T, float>(static_cast<float>(sumB) * weight);
        StoreRGB<T, T, PixelType>(store, newR, newG, newB);

        store += channels;
      } else if (x + radius >= 0) {
        int mpy = x & bufLength;
        int mpyPRadius = (x + radius) & bufLength;
        difR += 3 * (bufferR[mpy] - bufferR[mpyPRadius]);
        difG += 3 * (bufferG[mpy] - bufferG[mpyPRadius]);
        difB += 3 * (bufferB[mpy] - bufferB[mpyPRadius]);
      } else if (x + 2 * radius >= 0) {
        int mpyPRadius = (x + radius) & bufLength;
        difR -= 3 * bufferR[mpyPRadius];
        difG -= 3 * bufferG[mpyPRadius];
        difB -= 3 * bufferB[mpyPRadius];
      }

      int mPNextRad = (x + 2 * radius) & bufLength;

      auto srcNext = reinterpret_cast<T*>(reinterpret_cast<uint8_t *>(data) + y * stride);
      int px = std::clamp(x + radius, 0, static_cast<int>(width - 1)) * channels;
      srcNext += px;

      V pR, pG, pB, pA;
      LoadRGBA<T, V, PixelType>(srcNext, pR, pG, pB, pA);

      sumR += derR += difR += pR;
      bufferR[mPNextRad] = pR;

      sumG += derG += difG += pG;
      bufferG[mPNextRad] = pG;

      sumB += derB += difB += pB;
      bufferB[mPNextRad] = pB;
    }
  }
}

#define FAST_GAUSSIAN_DECLARATION_R(pixelName, pixelType, storageType) \
    void FastGaussianBlur##pixelName(storageType *data, uint32_t stride, uint32_t width, uint32_t height, int radius) {\
      const int threadCount = concurrency::getThreadCounts(width, height);\
      concurrency::parallel_for_segment(threadCount, width, [&](int start, int end) {\
        VerticalGaussianPass<storageType, int, sparkyuv::PIXEL_##pixelType>(data, stride, width, height, radius, start, end);\
      });\
      concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {\
        HorizontalGaussianPass<storageType, int, sparkyuv::PIXEL_##pixelType>(data, stride, width, height, radius, start, end);\
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

#define FAST_GAUSSIAN_DECLARATION_R_F16(pixelName, pixelType) \
    void FastGaussianBlur##pixelName(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius) {\
      const int threadCount = concurrency::getThreadCounts(width, height);\
      concurrency::parallel_for_segment(threadCount, width, [&](int start, int end) {\
        VerticalGaussianPass<hwy::float16_t, float, sparkyuv::PIXEL_##pixelType>(reinterpret_cast<hwy::float16_t*>(data), \
                            stride, width, height, radius, start, end);\
      });\
      concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {\
        HorizontalGaussianPass<hwy::float16_t, float, sparkyuv::PIXEL_##pixelType>(reinterpret_cast<hwy::float16_t*>(data), \
                stride, width, height, radius, start, end);\
      });\
    }

FAST_GAUSSIAN_DECLARATION_R_F16(RGBAF16, RGBA)
FAST_GAUSSIAN_DECLARATION_R_F16(RGBF16, RGB)
#if SPARKYUV_FULL_CHANNELS
FAST_GAUSSIAN_DECLARATION_R_F16(ARGBF16, ARGB)
FAST_GAUSSIAN_DECLARATION_R_F16(ABGRF16, ABGR)
FAST_GAUSSIAN_DECLARATION_R_F16(BGRAF16, BGRA)
FAST_GAUSSIAN_DECLARATION_R_F16(BGRF16, BGR)
#endif

#undef FAST_GAUSSIAN_DECLARATION_R_F16

#define FAST_GAUSSIAN_DECLARATION_CHAN_R(pixelName, storageType) \
    void FastGaussianBlur##pixelName(storageType *data, uint32_t stride, uint32_t width, uint32_t height, int radius) {\
      VerticalGaussianPassChannel<storageType, int>(data, stride, width, height, radius);\
      HorizontalGaussianPassChannel<storageType, int>(data, stride, width, height, radius);\
    }

FAST_GAUSSIAN_DECLARATION_CHAN_R(Channel, uint8_t)
FAST_GAUSSIAN_DECLARATION_CHAN_R(Channel16, uint16_t)

#undef FAST_GAUSSIAN_DECLARATION_CHAN_R

#define FAST_GAUSSIAN_NEXT_DECLARATION_R(pixelName, pixelType, storageType) \
    void FastGaussianNextBlur##pixelName(storageType *data, uint32_t stride, uint32_t width, uint32_t height, int radius) {\
      const int threadCount = concurrency::getThreadCounts(width, height);\
      concurrency::parallel_for_segment(threadCount, width, [&](int start, int end) {\
        VerticalFastGaussianPassNext<storageType, int, sparkyuv::PIXEL_##pixelType>(data, stride,  \
                width, height, radius, start, end);\
      });\
      concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {\
        HorizontalFastGaussianNext<storageType, int, sparkyuv::PIXEL_##pixelType>(data, stride,\
                width, height, radius, start, end);\
      });\
    }

FAST_GAUSSIAN_NEXT_DECLARATION_R(RGBA, RGBA, uint8_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(RGB, RGB, uint8_t)
#if SPARKYUV_FULL_CHANNELS
FAST_GAUSSIAN_NEXT_DECLARATION_R(ARGB, ARGB, uint8_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(ABGR, ABGR, uint8_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(BGRA, BGRA, uint8_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(BGR, BGR, uint8_t)
#endif

FAST_GAUSSIAN_NEXT_DECLARATION_R(RGBA16, RGBA, uint16_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(RGB16, RGB, uint16_t)
#if SPARKYUV_FULL_CHANNELS
FAST_GAUSSIAN_NEXT_DECLARATION_R(ARGB16, ARGB, uint16_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(ABGR16, ABGR, uint16_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(BGRA16, BGRA, uint16_t)
FAST_GAUSSIAN_NEXT_DECLARATION_R(BGR16, BGR, uint16_t)
#endif

#undef FAST_GAUSSIAN_NEXT_DECLARATION_R

#define FAST_GAUSSIAN_NEXT_DECLARATION_R_F16(pixelName, pixelType) \
    void FastGaussianNextBlur##pixelName(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius) {\
      const int threadCount = concurrency::getThreadCounts(width, height);\
      concurrency::parallel_for_segment(threadCount, width, [&](int start, int end) {\
        VerticalFastGaussianPassNext<hwy::float16_t, float, sparkyuv::PIXEL_##pixelType>(reinterpret_cast<hwy::float16_t*>(data), \
                            stride, width, height, radius, start, end);\
      });\
      concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {\
        HorizontalFastGaussianNext<hwy::float16_t, float, sparkyuv::PIXEL_##pixelType>(reinterpret_cast<hwy::float16_t*>(data), \
                stride, width, height, radius, start, end);\
      });\
    }

FAST_GAUSSIAN_NEXT_DECLARATION_R_F16(RGBAF16, RGBA)
FAST_GAUSSIAN_NEXT_DECLARATION_R_F16(RGBF16, RGB)
#if SPARKYUV_FULL_CHANNELS
FAST_GAUSSIAN_NEXT_DECLARATION_R_F16(ARGBF16, ARGB)
FAST_GAUSSIAN_NEXT_DECLARATION_R_F16(ABGRF16, ABGR)
FAST_GAUSSIAN_NEXT_DECLARATION_R_F16(BGRAF16, BGRA)
FAST_GAUSSIAN_NEXT_DECLARATION_R_F16(BGRF16, BGR)
#endif

#undef FAST_GAUSSIAN_NEXT_DECLARATION_R_F16
}