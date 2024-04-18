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

#if defined(SPARKYUV_TRANSPOSE_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_TRANSPOSE_INL_H
#undef SPARKYUV_TRANSPOSE_INL_H
#else
#define SPARKYUV_TRANSPOSE_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include <algorithm>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL,
    typename std::enable_if<
        (Surface == SURFACE_RGBA1010102 || Surface == SURFACE_CHANNELS_4) && std::is_same<T, uint8_t>::value,
        int>::type = 0>
static void
TransposeClockwiseImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                       T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                       const uint32_t width, const uint32_t height) {
  const FixedTag<uint32_t, 4> d;
  using V = Vec<decltype(d)>;
  const int lanes = 4;

  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  uint32_t y = 0;
  for (; y + lanes < height; y += lanes) {
    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const V p0 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + y * srcStride + x * channels));
      const V p1 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + (y + 1) * srcStride + x * channels));
      const V p2 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + (y + 2) * srcStride + x * channels));
      const V p3 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + (y + 3) * srcStride + x * channels));

      /**
       * x1 x2 x3 x4
       * x4 x5 x6 x7
       * x8 x9 x10 x11
       * x12 x13 x14 x15
       */

      /**
       * x12 x8 x4 x1
       * x13 x9 x5 x2
       * x14 x10 x6 x3
       * x15 x11 x7 x4
       */

      const auto q0 = InterleaveLower(d, p0, p2); // x3 x10 x4 x11
      const auto q1 = InterleaveLower(d, p1, p3); // x6 x14 x7 x15
      const auto q2 = InterleaveUpper(d, p0, p2); // x1 x8 x2 x9
      const auto q3 = InterleaveUpper(d, p1, p3); // x4 x12 x5 x13

      const auto r0 = Reverse(d, InterleaveLower(d, q0, q1));  // x4 x7 x11 x15  -> Reverse
      const auto r1 = Reverse(d, InterleaveUpper(d, q0, q1));  // x3 x6 x10 x14  -> Reverse
      const auto r2 = Reverse(d, InterleaveLower(d, q2, q3));  // x2 x5 x9 x13   -> Reverse
      const auto r3 = Reverse(d, InterleaveUpper(d, q2, q3));  // x1 x4 x8 x12   -> Reverse

      auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + x * newStride);
      mTransposed += (height - 1 - y) * channels - (lanes - 1) * channels;
      StoreU(r0, d, reinterpret_cast<uint32_t *>(mTransposed));
      mTransposed = reinterpret_cast<T *>(mDestinationPtr + (x + 1) * newStride);
      mTransposed += (height - 1 - y) * channels - (lanes - 1) * channels;
      StoreU(r1, d, reinterpret_cast<uint32_t *>(mTransposed));
      mTransposed = reinterpret_cast<T *>(mDestinationPtr + (x + 2) * newStride);
      mTransposed += (height - 1 - y) * channels - (lanes - 1) * channels;
      StoreU(r2, d, reinterpret_cast<uint32_t *>(mTransposed));
      mTransposed = reinterpret_cast<T *>(mDestinationPtr + (x + 3) * newStride);
      mTransposed += (height - 1 - y) * channels - (lanes - 1) * channels;
      StoreU(r3, d, reinterpret_cast<uint32_t *>(mTransposed));
    }

    for (; x < width; ++x) {
      for (uint32_t realY = y; realY < y + lanes; ++realY) {
        auto mSrc = reinterpret_cast<const T *>(mSourcePtr + realY * srcStride);
        mSrc += channels * x;

        auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + x * newStride);
        mTransposed += (height - 1 - realY) * channels;

        mTransposed[0] = mSrc[0];
        mTransposed[1] = mSrc[1];
        mTransposed[2] = mSrc[2];
        mTransposed[3] = mSrc[3];
      }
    }
  }

  for (; y < height; ++y) {
    auto mSrc = reinterpret_cast<const T *>(mSourcePtr + y * srcStride);

    for (uint32_t x = 0; x < width; ++x) {
      auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + x * newStride);
      mTransposed += (height - 1 - y) * channels;

      mTransposed[0] = mSrc[0];
      mTransposed[1] = mSrc[1];
      mTransposed[2] = mSrc[2];
      mTransposed[3] = mSrc[3];

      mSrc += channels;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL,
    typename std::enable_if<!(Surface == SURFACE_RGBA1010102 && std::is_same<T, uint8_t>::value)
                                && !(Surface == SURFACE_CHANNELS_4 && std::is_same<T, uint8_t>::value), int>::type = 0>
static void
TransposeClockwiseImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                       T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                       const uint32_t width, const uint32_t height) {

  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  for (uint32_t y = 0; y < height; ++y) {
    auto mSrc = reinterpret_cast<const T *>(mSourcePtr + y * srcStride);
    for (uint32_t x = 0; x < width; ++x) {

      auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + x * newStride);
      mTransposed += (height - 1 - y) * channels;

      if (Surface == SURFACE_CHANNEL) {
        mTransposed[0] = mSrc[0];
      } else if (Surface == SURFACE_CHANNELS_3) {
        mTransposed[0] = mSrc[0];
        mTransposed[1] = mSrc[1];
        mTransposed[2] = mSrc[2];
      } else if (Surface == SURFACE_CHANNELS_4) {
        mTransposed[0] = mSrc[0];
        mTransposed[1] = mSrc[1];
        mTransposed[2] = mSrc[2];
        mTransposed[3] = mSrc[3];
      }

      mSrc += channels;
    }
  }
}

#define TRANSPOSE_CLOCKWISE_DECLARATION(srcPixel, storageType, surfaceType) \
    static void TransposeClockwise##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                           storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                           const uint32_t width, const uint32_t height) {\
        TransposeClockwiseImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                             width, height); \
    }

TRANSPOSE_CLOCKWISE_DECLARATION(RGBA, uint8_t, CHANNELS_4)
TRANSPOSE_CLOCKWISE_DECLARATION(RGB, uint8_t, CHANNELS_3)
TRANSPOSE_CLOCKWISE_DECLARATION(Channel, uint8_t, CHANNEL)
TRANSPOSE_CLOCKWISE_DECLARATION(RGBA1010102, uint8_t, RGBA1010102)

TRANSPOSE_CLOCKWISE_DECLARATION(RGBA16, uint16_t, CHANNELS_4)
TRANSPOSE_CLOCKWISE_DECLARATION(RGB16, uint16_t, CHANNELS_3)
TRANSPOSE_CLOCKWISE_DECLARATION(Channel16, uint16_t, CHANNEL)

#undef TRANSPOSE_CLOCKWISE_DECLARATION

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL,
    typename std::enable_if<
        (Surface == SURFACE_RGBA1010102 || Surface == SURFACE_CHANNELS_4) && std::is_same<T, uint8_t>::value,
        int>::type = 0>
void
TransposeCounterClockwiseImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                              T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                              const uint32_t width, const uint32_t height) {
  const FixedTag<uint32_t, 4> d;
  using V = Vec<decltype(d)>;
  const int lanes = 4;

  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  uint32_t y = 0;
  for (; y + lanes < height; y += lanes) {
    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const V p0 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + y * srcStride + x * channels));
      const V p1 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + (y + 1) * srcStride + x * channels));
      const V p2 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + (y + 2) * srcStride + x * channels));
      const V p3 = LoadU(d, reinterpret_cast<const uint32_t *>(mSourcePtr + (y + 3) * srcStride + x * channels));

      const auto q0 = InterleaveLower(d, p0, p2);
      const auto q1 = InterleaveLower(d, p1, p3);
      const auto q2 = InterleaveUpper(d, p0, p2);
      const auto q3 = InterleaveUpper(d, p1, p3);

      const auto r0 = InterleaveLower(d, q0, q1);
      const auto r1 = InterleaveUpper(d, q0, q1);
      const auto r2 = InterleaveLower(d, q2, q3);
      const auto r3 = InterleaveUpper(d, q2, q3);

      auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + (width - 1 - x) * newStride);
      mTransposed += y * channels;
      StoreU(r0, d, reinterpret_cast<uint32_t *>(mTransposed));
      mTransposed = reinterpret_cast<T *>(mDestinationPtr + (width - 1 - (x + 1)) * newStride);
      mTransposed += y * channels;
      StoreU(r1, d, reinterpret_cast<uint32_t *>(mTransposed));
      mTransposed = reinterpret_cast<T *>(mDestinationPtr + (width - 1 - (x + 2)) * newStride);
      mTransposed += y * channels;
      StoreU(r2, d, reinterpret_cast<uint32_t *>(mTransposed));
      mTransposed = reinterpret_cast<T *>(mDestinationPtr + (width - 1 - (x + 3)) * newStride);
      mTransposed += y * channels;
      StoreU(r3, d, reinterpret_cast<uint32_t *>(mTransposed));
    }

    for (; x < width; ++x) {
      for (uint32_t realY = y; realY < y + lanes; ++realY) {
        auto mSrc = reinterpret_cast<const T *>(mSourcePtr + realY * srcStride);
        mSrc += channels * x;

        auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + (width - 1 - x) * newStride);
        mTransposed += realY * channels;

        mTransposed[0] = mSrc[0];
        mTransposed[1] = mSrc[1];
        mTransposed[2] = mSrc[2];
        mTransposed[3] = mSrc[3];
      }
    }
  }

  for (; y < height; ++y) {
    auto mSrc = reinterpret_cast<const T *>(mSourcePtr + y * srcStride);

    for (uint32_t x = 0; x < width; ++x) {
      auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + (width - 1 - x) * newStride);
      mTransposed += y * channels;

      mTransposed[0] = mSrc[0];
      mTransposed[1] = mSrc[1];
      mTransposed[2] = mSrc[2];
      mTransposed[3] = mSrc[3];

      mSrc += channels;
    }
  }
}

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL,
    typename std::enable_if<!(Surface == SURFACE_RGBA1010102 && std::is_same<T, uint8_t>::value)
                                && !(Surface == SURFACE_CHANNELS_4 && std::is_same<T, uint8_t>::value), int>::type = 0>
static void
TransposeCounterClockwiseImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                              T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                              const uint32_t width, const uint32_t height) {
  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  for (uint32_t y = 0; y < height; ++y) {
    auto mSrc = reinterpret_cast<const T *>(mSourcePtr + y * srcStride);
    for (uint32_t x = 0; x < width; ++x) {

      auto mTransposed = reinterpret_cast<T *>(mDestinationPtr + (width - 1 - x) * newStride);
      mTransposed += y * channels;

      if (Surface == SURFACE_CHANNEL) {
        mTransposed[0] = mSrc[0];
      } else if (Surface == SURFACE_CHANNELS_3) {
        mTransposed[0] = mSrc[0];
        mTransposed[1] = mSrc[1];
        mTransposed[2] = mSrc[2];
      } else if (Surface == SURFACE_CHANNELS_4) {
        mTransposed[0] = mSrc[0];
        mTransposed[1] = mSrc[1];
        mTransposed[2] = mSrc[2];
        mTransposed[3] = mSrc[3];
      }

      mSrc += channels;
    }
  }
}

#define TRANSPOSE_CCW_DECLARATION(srcPixel, storageType, surfaceType) \
    static void TransposeCounterClockwise##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                           storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                           const uint32_t width, const uint32_t height) {\
        TransposeCounterClockwiseImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                             width, height); \
    }

TRANSPOSE_CCW_DECLARATION(RGBA, uint8_t, CHANNELS_4)
TRANSPOSE_CCW_DECLARATION(RGB, uint8_t, CHANNELS_3)
TRANSPOSE_CCW_DECLARATION(Channel, uint8_t, CHANNEL)
TRANSPOSE_CCW_DECLARATION(RGBA1010102, uint8_t, RGBA1010102)

TRANSPOSE_CCW_DECLARATION(RGBA16, uint16_t, CHANNELS_4)
TRANSPOSE_CCW_DECLARATION(RGB16, uint16_t, CHANNELS_3)
TRANSPOSE_CCW_DECLARATION(Channel16, uint16_t, CHANNEL)

#undef TRANSPOSE_CCW_DECLARATION

}
HWY_AFTER_NAMESPACE();

#endif