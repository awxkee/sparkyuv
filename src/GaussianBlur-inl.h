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
#include "TypeSupport.h"

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
                           const float *mKernel, const int kernelSize) {
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

        float weight1 = mKernel[halfOfKernel + r];
        acc = MulAdd(i1, Set(df, weight1), acc);

        float weight2 = mKernel[halfOfKernel + r + 1];
        acc = MulAdd(i2, Set(df, weight2), acc);

        float weight3 = mKernel[halfOfKernel + r + 2];
        acc = MulAdd(i3, Set(df, weight3), acc);

        float weight4 = mKernel[halfOfKernel + r + 3];
        acc = MulAdd(i4, Set(df, weight4), acc);
      }

      for (; r <= maxKernel; ++r) {
        float weight = mKernel[halfOfKernel + r];
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 4;
        auto vx = ConvertTo(df, PromoteTo(d32, LoadU(d8x4, &localSource[sourcePX])));
        acc = MulAdd(vx, Set(df, weight), acc);
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
    typename std::enable_if<!std::is_same<T, uint8_t>::value, int>::type = 0,
    ENABLE_TYPE_IS_F16(T)>
void
GaussianBlurVerticalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                         T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                         const uint32_t startY, const uint32_t endY,
                         const uint32_t width, const uint32_t height,
                         const float *mKernel, const int kernelSize) {
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int64_t maxHeight = static_cast<int>(height) - 1;

  const FixedTag<uint16_t, 8> d16x8;
  const FixedTag<uint16_t, 4> d16x4;
  const FixedTag<hwy::float16_t, 8> df16;
  const FixedTag<hwy::float16_t, 4> df16x4;
  const FixedTag<hwy::float32_t, 4> df;
  using VF = Vec<decltype(df)>;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;

      VF accumulator = Zero(df);

      auto kx = static_cast<int>(x) * 4;

      for (; r <= maxKernel; ++r) {
        uint32_t shiftX = std::clamp(static_cast<int64_t>(y) + static_cast<int64_t>(r),
                                     static_cast<int64_t>(0),
                                     static_cast<int64_t>(maxHeight));
        auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + shiftX * srcStride);
        float weight = mKernel[halfOfKernel + r];
        VF pixelData;
#if SPARKYUV_ALLOW_FLOAT16
        const auto pxf16 = LoadU(df16x4, &localSource[kx]);
        pixelData = PromoteTo(df, pxf16);
#else
        const auto pxf16 = BitCast(df16x4, LoadU(d16x4, reinterpret_cast<const uint16_t*>(&localSource[kx])));
        pixelData = PromoteTo(df, pxf16);
#endif
        accumulator = MulAdd(pixelData, Set(df, weight), accumulator);
      }

#if SPARKYUV_ALLOW_FLOAT16
      StoreU(DemoteTo(df16x4, accumulator), df16x4, dst);
#else
      auto duStore = BitCast(d16x4, DemoteTo(df16x4, accumulator));
      StoreU(duStore, d16x4, reinterpret_cast<uint16_t*>(dst));
#endif

      dst += 4;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_4, int>::type = 0,
    typename std::enable_if<!std::is_same<T, uint8_t>::value, int>::type = 0,
    ENABLE_TYPE_IS_F16(T)>
void
GaussianBlurHorizontalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                           T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                           const uint32_t startY, const uint32_t endY,
                           const uint32_t width, const uint32_t /* height */,
                           const float *mKernel, const int kernelSize) {
  const int halfOfKernel = kernelSize / 2;
  const bool isEven = kernelSize % 2 == 0;
  const int maxKernel = isEven ? halfOfKernel - 1 : halfOfKernel;

  auto mDst = reinterpret_cast<uint8_t *>(mDestination);
  int maxWidth = static_cast<int>(width) - 1;
  int sZero = 0;

  const FixedTag<uint16_t, 8> d16x8;
  const FixedTag<uint16_t, 4> d16x4;
  const FixedTag<hwy::float16_t, 8> df16;
  const FixedTag<hwy::float16_t, 4> df16x4;
  const FixedTag<hwy::float32_t, 4> df;
  using VF = Vec<decltype(df)>;

  for (uint32_t y = startY; y < endY; ++y) {
    auto dst = reinterpret_cast<T *>(mDst + dstStride * y);
    auto localSource = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSource) + y * srcStride);
    for (uint32_t x = 0; x < width; ++x) {
      int r = -halfOfKernel;

      VF accumulator = Zero(df);
      auto kx = static_cast<int>(x);

      for (; r + 2 <= maxKernel && kx + x + 2 < width; r += 2) {
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 4;
        auto movedSrc = localSource + sourcePX;
        const float weight1 = mKernel[halfOfKernel + r];
        const float weight2 = mKernel[halfOfKernel + r + 1];
        VF pixelData1;
        VF pixelData2;

#if SPARKYUV_ALLOW_FLOAT16
        const auto pxf16 = LoadU(df16, movedSrc);
        pixelData1 = PromoteLowerTo(df, pxf16);
        pixelData2 = PromoteUpperTo(df, pxf16);
#else
        const auto pxf16 = BitCast(df16, LoadU(d16x8, reinterpret_cast<const uint16_t*>(movedSrc)));
        pixelData1 = PromoteLowerTo(df, pxf16);
        pixelData2 = PromoteUpperTo(df, pxf16);
#endif

        accumulator = MulAdd(pixelData1, Set(df, weight1), accumulator);
        accumulator = MulAdd(pixelData2, Set(df, weight2), accumulator);
      }

      for (; r <= maxKernel; ++r) {
        float weight = mKernel[halfOfKernel + r];
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 4;
        auto movedSrc = localSource + sourcePX;
        VF pixelData;
#if SPARKYUV_ALLOW_FLOAT16
        const auto pxf16 = LoadU(df16x4, movedSrc);
        pixelData = PromoteTo(df, pxf16);
#else
        const auto pxf16 = BitCast(df16x4, LoadU(d16x4, reinterpret_cast<const uint16_t*>(movedSrc)));
        pixelData = PromoteTo(df, pxf16);
#endif
        accumulator = MulAdd(pixelData, Set(df, weight), accumulator);
      }

#if SPARKYUV_ALLOW_FLOAT16
      StoreU(DemoteTo(df16x4, accumulator), df16x4, dst);
#else
      auto duStore = BitCast(d16x4, DemoteTo(df16x4, accumulator));
      StoreU(duStore, d16x4, reinterpret_cast<uint16_t*>(dst));
#endif

      dst += 4;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_4, int>::type = 0,
    typename std::enable_if<std::is_same<T, uint8_t>::value, int>::type = 0,
    ENABLE_TYPE_IS_F16(T)>
void
GaussianBlurVerticalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                         T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                         const uint32_t startY, const uint32_t endY,
                         const uint32_t width, const uint32_t height,
                         const float *mKernel, const int kernelSize) {
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
        float weight = mKernel[halfOfKernel + r];
        uint32_t sourcePX = x * 4;
        auto vx = ConvertTo(df, PromoteTo(d32, LoadU(d8x4, &localSource[sourcePX])));
        acc = MulAdd(vx, Set(df, weight), acc);
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
    typename std::enable_if<!std::is_same<T, uint8_t>::value, int>::type = 0,
    ENABLE_TYPE_IS_NOT_F16(T)>
void
GaussianBlurHorizontalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                           T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                           const uint32_t startY, const uint32_t endY,
                           const uint32_t width, const uint32_t /* height */,
                           const float *mKernel, const int kernelSize) {
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
        float weight = mKernel[halfOfKernel + r];
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 4;
        auto movedSrc = localSource + sourcePX;
        // Stupid workaround to avoid errors where hwy f16 not really properly works
        accumulator1 += LoadFloat<T>(&movedSrc[0]) * weight;
        accumulator2 += LoadFloat<T>(&movedSrc[1]) * weight;
        accumulator3 += LoadFloat<T>(&movedSrc[2]) * weight;
        accumulator4 += LoadFloat<T>(&movedSrc[3]) * weight;
      }
      if (!std::is_same<hwy::float16_t, T>::value) {
        StoreRoundedFloat(&dst[0], accumulator1);
        StoreRoundedFloat(&dst[1], accumulator2);
        StoreRoundedFloat(&dst[2], accumulator3);
        StoreRoundedFloat(&dst[3], accumulator4);
      } else {
        StoreFloat(&dst[0], accumulator1);
        StoreFloat(&dst[1], accumulator2);
        StoreFloat(&dst[2], accumulator3);
        StoreFloat(&dst[3], accumulator4);
      }
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
                           const float *mKernel, const int kernelSize) {
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
        float weight = mKernel[halfOfKernel + r];
        int sourcePX = std::clamp(kx + r, sZero, maxWidth) * 3;
        accumulator1 += LoadFloat<T>(&localSource[sourcePX]) * weight;
        accumulator2 += LoadFloat<T>(&localSource[sourcePX + 1]) * weight;
        accumulator3 += LoadFloat<T>(&localSource[sourcePX + 2]) * weight;
      }
      if (!std::is_same<T, hwy::float16_t>::value) {
        StoreRoundedFloat(&dst[0], accumulator1);
        StoreRoundedFloat(&dst[1], accumulator2);
        StoreRoundedFloat(&dst[2], accumulator3);
      } else {
        StoreFloat(&dst[0], accumulator1);
        StoreFloat(&dst[1], accumulator2);
        StoreFloat(&dst[2], accumulator3);
      }
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
                           const float *mKernel, const int kernelSize) {
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
        accumulator += LoadFloat<T>(&localSource[std::clamp(kx + r, sZero, maxWidth)]) * mKernel[halfOfKernel + r];
      }
      if (!std::is_same<T, hwy::float16_t>::value) {
        StoreRoundedFloat(&dst[0], accumulator);
      } else {
        StoreFloat(&dst[0], accumulator);
      }
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
                         const float *mKernel, const int kernelSize) {
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
        // Stupid workaround to avoid errors where hwy f16 not really properly works
        accumulator += LoadFloat<T>(&localSource[kx]) * mKernel[halfOfKernel + r];
      }
      if (!std::is_same<T, hwy::float16_t>::value) {
        StoreRoundedFloat(&dst[0], accumulator);
      } else {
        StoreFloat(&dst[0], accumulator);
      }
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
                         const float *mKernel, const int kernelSize) {
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
        float weight = mKernel[halfOfKernel + r];
        accumulator += LoadFloat<T>(&localSource[kx]) * weight;
        accumulator1 += LoadFloat<T>(&localSource[kx + 1]) * weight;
        accumulator2 += LoadFloat<T>(&localSource[kx + 2]) * weight;
      }
      if (!std::is_same<T, hwy::float16_t>::value) {
        StoreRoundedFloat(&dst[0], accumulator);
        StoreRoundedFloat(&dst[1], accumulator1);
        StoreRoundedFloat(&dst[2], accumulator2);
      } else {
        StoreFloat(&dst[0], accumulator);
        StoreFloat(&dst[1], accumulator1);
        StoreFloat(&dst[2], accumulator2);
      }
      dst += 3;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface,
    typename std::enable_if<Surface == sparkyuv::SURFACE_CHANNELS_4, int>::type = 0,
    typename std::enable_if<!std::is_same<T, uint8_t>::value, int>::type = 0,
    HWY_IF_NOT_F16(T)>
void
GaussianBlurVerticalPass(const T *SPARKYUV_RESTRICT mSource, const uint32_t srcStride,
                         T *SPARKYUV_RESTRICT mDestination, const uint32_t dstStride,
                         const uint32_t startY, const uint32_t endY,
                         const uint32_t width, const uint32_t height,
                         const float *mKernel, const int kernelSize) {
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
        float weight = mKernel[halfOfKernel + r];
        // Stupid workaround to avoid errors where hwy f16 not really properly works
        accumulator += LoadFloat<T>(&localSource[kx]) * weight;
        accumulator1 += LoadFloat<T>(&localSource[kx + 1]) * weight;
        accumulator2 += LoadFloat<T>(&localSource[kx + 2]) * weight;
        accumulator3 += LoadFloat<T>(&localSource[kx + 3]) * weight;
      }
      if (!std::is_same<T, hwy::float16_t>::value) {
        StoreRoundedFloat(&dst[0], accumulator);
        StoreRoundedFloat(&dst[1], accumulator1);
        StoreRoundedFloat(&dst[2], accumulator2);
        StoreRoundedFloat(&dst[3], accumulator3);
      } else {
        StoreFloat(&dst[0], accumulator);
        StoreFloat(&dst[1], accumulator1);
        StoreFloat(&dst[2], accumulator2);
        StoreFloat(&dst[3], accumulator3);
      }
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
                         const float *mKernel, const int kernelSize) {
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
        float weight = mKernel[halfOfKernel + r];
        uint32_t sourcePX = x * 4;
        auto vx = ConvertTo(df, PromoteTo(d32, LoadU(d8x4, &localSource[sourcePX])));
        acc = MulAdd(vx, Set(df, weight), acc);
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
  const auto alignedKernel = hwy::AllocateAligned<float>(kernel.size());
  std::copy(kernel.begin(), kernel.end(), alignedKernel.get());
  concurrency::parallel_for_segment(threadCount, height, [&](int start, int end) {
    GaussianBlurHorizontalPass<T, Surface>(mSource,
                                           srcStride,
                                           reinterpret_cast<T *>(transient.get()),
                                           newStride,
                                           start,
                                           end,
                                           width,
                                           height,
                                           reinterpret_cast<const float *>(alignedKernel.get()),
                                           kernel.size());
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
                                         reinterpret_cast<const float *>(alignedKernel.get()),
                                         kernel.size());
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

#define GAUSSIAN_BLUR_DECLARATION_R_F16(srcPixel, surfaceType) \
    void GaussianBlur##srcPixel##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                    uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                    const uint32_t width, const uint32_t height,  \
                                    const int kernelSize, const float sigma) {\
        GaussianBlurImpl<hwy::float16_t, sparkyuv::SURFACE_##surfaceType>(reinterpret_cast<const hwy::float16_t*>(src), \
            srcStride, reinterpret_cast<hwy::float16_t*>(dst), dstStride, width, height, kernelSize, sigma); \
    }

GAUSSIAN_BLUR_DECLARATION_R_F16(RGBAF16, CHANNELS_4)
GAUSSIAN_BLUR_DECLARATION_R_F16(RGBF16, CHANNELS_3)
GAUSSIAN_BLUR_DECLARATION_R_F16(ChannelF16, CHANNEL)

#undef GAUSSIAN_BLUR_DECLARATION_R_F16

}
HWY_AFTER_NAMESPACE();

#endif