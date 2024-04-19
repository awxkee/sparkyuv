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

#if defined(SPARKYUV_FLIP_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_FLIP_INL_H
#undef SPARKYUV_FLIP_INL_H
#else
#define SPARKYUV_FLIP_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include "math/gaussian.h"
#include "hwy/aligned_allocator.h"
#include "concurrency.hpp"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_4, int>::type = 0,
    typename std::enable_if<std::is_same<T, uint8_t>::value, int>::type = 0>
void
GaussianBlurHorizontalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                           T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                           const uint32_t startY, const uint32_t endY,
                           const uint32_t width, const uint32_t /* height */,
                           const std::vector<float> &mKernel) {
  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int maxWidth = static_cast<int>(width) - 1;
  int sZero = 0;

  const FixedTag<uint8_t, 16> d8x16;
  const Half<decltype(d8x16)> dh8;
  const Rebind<uint16_t, decltype(dh8)> d16;
  const FixedTag<uint8_t, 4> d8x4;
  const FixedTag<uint32_t, 4> d32;
  const FixedTag<float, 4> df;
  using VF = Vec<decltype(df)>;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      VF acc = Zero(df);
      auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + y * srcStride);
      auto kx = static_cast<int>(x);

      for (; r + 4 <= maxKernel && kx + r + 4 < width; r += 4) {
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 4;
        auto vx = LoadU(d8x16, &localSource[sourcePX]);
        auto i1 = ConvertTo(df, PromoteLowerTo(d32, LowerHalf(vx)));
        auto i2 = ConvertTo(df, PromoteLowerTo(d32, UpperHalf(dh8, vx)));
        auto i3 = ConvertTo(df, PromoteUpperTo(d32, PromoteTo(d16, LowerHalf(vx))));
        auto i4 = ConvertTo(df, PromoteUpperTo(d32, PromoteTo(d16, UpperHalf(dh8, vx))));

        float weight1 = kernel[halfOfKernel + r];
        acc = Add(acc, Mul(i1, Set(df, weight1)));

        float weight2 = kernel[halfOfKernel + r + 1];
        acc = Add(acc, Mul(i2, Set(df, weight2)));

        float weight3 = kernel[halfOfKernel + r + 2];
        acc = Add(acc, Mul(i3, Set(df, weight3)));

        float weight4 = kernel[halfOfKernel + r + 3];
        acc = Add(acc, Mul(i4, Set(df, weight4)));
      }

      for (; r <= maxKernel; ++r) {
        float weight = kernel[halfOfKernel + r];
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 4;
        auto vx = ConvertTo(df, PromoteTo(d32, LoadU(d8x4, &localSource[sourcePX])));
        acc = Add(acc, Mul(vx, Set(df, weight)));
      }
      acc = Round(acc);
      auto newPX = DemoteTo(d8x4, ConvertTo(d32, acc));
      StoreU(newPX, d8x4, dst);
      dst += 4;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_4, int>::type = 0,
    typename std::enable_if<!std::is_same<T, uint8_t>::value, int>::type = 0>
void
GaussianBlurHorizontalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                           T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                           const uint32_t startY, const uint32_t endY,
                           const uint32_t width, const uint32_t /* height */,
                           const std::vector<float> &mKernel) {
  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int maxWidth = static_cast<int>(width) - 1;
  int sZero = 0;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      float accumulator1 = 0.f;
      float accumulator2 = 0.f;
      float accumulator3 = 0.f;
      float accumulator4 = 0.f;
      auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + y * srcStride);
      auto kx = static_cast<int>(x);
      for (; r <= maxKernel; ++r) {
        float weight = kernel[halfOfKernel + r];
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 4;
        accumulator1 += static_cast<float>(localSource[sourcePX]) * weight;
        accumulator2 += static_cast<float>(localSource[sourcePX + 1]) * weight;
        accumulator3 += static_cast<float>(localSource[sourcePX + 2]) * weight;
        accumulator4 += static_cast<float>(localSource[sourcePX + 3]) * weight;
      }
      dst[0] = static_cast<T>(::roundf(accumulator1));
      dst[1] = static_cast<T>(::roundf(accumulator2));
      dst[2] = static_cast<T>(::roundf(accumulator3));
      dst[3] = static_cast<T>(::roundf(accumulator4));
      dst += 4;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_3, int>::type = 0>
void
GaussianBlurHorizontalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                           T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                           const uint32_t startY, const uint32_t endY,
                           const uint32_t width, const uint32_t /* height */,
                           const std::vector<float> &mKernel) {
  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int maxWidth = static_cast<int>(width) - 1;
  int sZero = 0;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      float accumulator1 = 0.f;
      float accumulator2 = 0.f;
      float accumulator3 = 0.f;
      auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + y * srcStride);
      auto kx = static_cast<int>(x);
      for (; r <= maxKernel; ++r) {
        float weight = kernel[halfOfKernel + r];
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 3;
        accumulator1 += localSource[sourcePX] * weight;
        accumulator2 += localSource[sourcePX + 1] * weight;
        accumulator3 += localSource[sourcePX + 2] * weight;
      }
      dst[0] = static_cast<T>(::roundf(accumulator1));
      dst[1] = static_cast<T>(::roundf(accumulator2));
      dst[2] = static_cast<T>(::roundf(accumulator3));
      dst += 3;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNEL, int>::type = 0>
void
GaussianBlurHorizontalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                           T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                           const uint32_t startY, const uint32_t endY,
                           const uint32_t width, const uint32_t /* height */,
                           const std::vector<float> &mKernel) {

  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int maxWidth = static_cast<int>(width) - 1;
  int sZero = 0;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      float accumulator = 0.f;
      auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + y * srcStride);
      auto kx = static_cast<int>(x);
      for (; r <= maxKernel; ++r) {
        accumulator += localSource[std::clamp(kx + r, sZero, maxWidth)] * kernel[halfOfKernel + r];
      }
      dst[0] = static_cast<T>(::roundf(accumulator));
      dst += 1;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNEL, int>::type = 0>
void
GaussianBlurVerticalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                         T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                         const uint32_t startY, const uint32_t endY,
                         const uint32_t width, const uint32_t height,
                         const std::vector<float> &mKernel) {

  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int64_t maxHeight = static_cast<int>(height) - 1;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      float accumulator = 0.f;
      auto kx = static_cast<int>(x);
      for (; r <= maxKernel; ++r) {
        uint32_t shiftX = std::clamp(static_cast<int64_t>(y) + static_cast<int64_t>(r),
                                     static_cast<int64_t>(0),
                                     static_cast<int64_t>(maxHeight));
        auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + shiftX * srcStride);
        accumulator += localSource[kx] * kernel[halfOfKernel + r];
      }
      dst[0] = static_cast<T>(::roundf(accumulator));
      dst += 1;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_3, int>::type = 0>
void
GaussianBlurVerticalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                         T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                         const uint32_t startY, const uint32_t endY,
                         const uint32_t width, const uint32_t height,
                         const std::vector<float> &mKernel) {

  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int64_t maxHeight = static_cast<int>(height) - 1;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      float accumulator = 0.f;
      float accumulator1 = 0.f;
      float accumulator2 = 0.f;
      auto kx = static_cast<int>(x) * 3;
      for (; r <= maxKernel; ++r) {
        uint32_t shiftX = std::clamp(static_cast<int64_t>(y) + static_cast<int64_t>(r),
                                     static_cast<int64_t>(0),
                                     static_cast<int64_t>(maxHeight));
        auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + shiftX * srcStride);
        float weight = kernel[halfOfKernel + r];
        accumulator += localSource[kx] * weight;
        accumulator1 += localSource[kx + 1] * weight;
        accumulator2 += localSource[kx + 2] * weight;
      }
      dst[0] = static_cast<T>(::roundf(accumulator));
      dst[1] = static_cast<T>(::roundf(accumulator1));
      dst[2] = static_cast<T>(::roundf(accumulator2));
      dst += 3;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_4, int>::type = 0,
    typename std::enable_if<!std::is_same<T, uint8_t>::value, int>::type = 0>
void
GaussianBlurVerticalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                         T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                         const uint32_t startY, const uint32_t endY,
                         const uint32_t width, const uint32_t height,
                         const std::vector<float> &mKernel) {

  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int64_t maxHeight = static_cast<int>(height) - 1;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      float accumulator = 0.f;
      float accumulator1 = 0.f;
      float accumulator2 = 0.f;
      float accumulator3 = 0.f;
      auto kx = static_cast<int>(x) * 4;
      for (; r <= maxKernel; ++r) {
        uint32_t shiftX = std::clamp(static_cast<int64_t>(y) + static_cast<int64_t>(r),
                                     static_cast<int64_t>(0),
                                     static_cast<int64_t>(maxHeight));
        auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + shiftX * srcStride);
        float weight = kernel[halfOfKernel + r];
        accumulator += localSource[kx] * weight;
        accumulator1 += localSource[kx + 1] * weight;
        accumulator2 += localSource[kx + 2] * weight;
        accumulator3 += localSource[kx + 3] * weight;
      }
      dst[0] = static_cast<T>(::roundf(accumulator));
      dst[1] = static_cast<T>(::roundf(accumulator1));
      dst[2] = static_cast<T>(::roundf(accumulator2));
      dst[3] = static_cast<T>(::roundf(accumulator3));
      dst += 4;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_4, int>::type = 0,
    typename std::enable_if<std::is_same<T, uint8_t>::value, int>::type = 0>
void
GaussianBlurVerticalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                           T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                           const uint32_t startY, const uint32_t endY,
                           const uint32_t width, const uint32_t height,
                           const std::vector<float> &mKernel) {
  float kernel[mKernel.size()];

  for (size_t i = 0; i < mKernel.size(); ++i) {
    kernel[i] = mKernel[i];
  }

  const int kernelSize = static_cast<int>(mKernel.size());
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int64_t maxHeight = static_cast<int>(height) - 1;

  const FixedTag<uint8_t, 4> d8x4;
  const FixedTag<uint32_t, 4> d32;
  const FixedTag<float, 4> df;
  using VF = Vec<decltype(df)>;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;
      VF acc = Zero(df);

      for (; r <= maxKernel; ++r) {
        uint32_t shiftX = std::clamp(static_cast<int64_t>(y) + static_cast<int64_t>(r),
                                     static_cast<int64_t>(0),
                                     static_cast<int64_t>(maxHeight));
        auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + shiftX * srcStride);
        float weight = kernel[halfOfKernel + r];
        uint32_t sourcePX = x * 4;
        auto vx = ConvertTo(df, PromoteTo(d32, LoadU(d8x4, &localSource[sourcePX])));
        acc = Add(acc, Mul(vx, Set(df, weight)));
      }
      acc = Round(acc);
      auto newPX = DemoteTo(d8x4, ConvertTo(d32, acc));
      StoreU(newPX, d8x4, dst);
      dst += 4;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface>
void
GaussianBlurImpl(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                 T *SPARKYUV_RESTRICT mDestination, const uint32_t newStride,
                 const uint32_t width, const uint32_t height, const int kernelSize,
                 const float sigma) {
  const auto kernel = Get1DGaussianKernel(kernelSize, sigma);
  const auto transient = hwy::AllocateAligned<uint8_t>(newStride * height);
  const auto threadCount = concurrency::getThreadCounts(width, height);
  concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {
    GaussianBlurHorizontalPass<T, Surface>(mSource,
                                           srcStride,
                                           reinterpret_cast<T *>(transient.get()),
                                           newStride,
                                           start,
                                           end,
                                           width,
                                           height,
                                           kernel);
  });

  concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {
    GaussianBlurVerticalPass<T, Surface>(reinterpret_cast<const T *>(transient.get()),
                                         newStride,
                                         mDestination,
                                         newStride,
                                         start,
                                         end,
                                         width,
                                         height,
                                         kernel);
  });
}

#define GAUSSIAN_BLUR_DECLARATION_R(srcPixel, storageType, surfaceType) \
    void GaussianBlur##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                    storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                    const uint32_t width, const uint32_t height,  \
                                    const int kernelSize, const float sigma) {\
        GaussianBlurImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                       width, height, kernelSize, sigma); \
    }

GAUSSIAN_BLUR_DECLARATION_R(RGBA, uint8_t, CHANNELS_4)
GAUSSIAN_BLUR_DECLARATION_R(RGB, uint8_t, CHANNELS_3)
GAUSSIAN_BLUR_DECLARATION_R(Channel, uint8_t, CHANNEL)

GAUSSIAN_BLUR_DECLARATION_R(RGBA16, uint16_t, CHANNELS_4)
GAUSSIAN_BLUR_DECLARATION_R(RGB16, uint16_t, CHANNELS_3)
GAUSSIAN_BLUR_DECLARATION_R(Channel16, uint16_t, CHANNEL)

GAUSSIAN_BLUR_DECLARATION_R(RGBAF32, float, CHANNELS_4)
GAUSSIAN_BLUR_DECLARATION_R(RGBF32, float, CHANNELS_3)
GAUSSIAN_BLUR_DECLARATION_R(ChannelF32, float, CHANNEL)

#undef GAUSSIAN_BLUR_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif