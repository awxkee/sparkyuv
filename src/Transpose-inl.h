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

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL>
void
TransposeClockwiseImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                       T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                       const uint32_t width, const uint32_t height) {

  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  for (uint32_t y = 0; y < height; ++y) {
    auto mSrc = reinterpret_cast<const T *>(src + y * srcStride);
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

    mSourcePtr += srcStride;
  }
}

#define TRANSPOSE_CLOCKWISE_DECLARATION(srcPixel, storageType, surfaceType) \
    void TransposeClockwise##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
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

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL>
void
TransposeCounterClockwiseImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                              T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                              const uint32_t width, const uint32_t height) {

  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  for (uint32_t y = 0; y < height; ++y) {
    auto mSrc = reinterpret_cast<const T *>(src + y * srcStride);
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

    mSourcePtr += srcStride;
  }
}

#define TRANSPOSE_CCW_DECLARATION(srcPixel, storageType, surfaceType) \
    void TransposeCounterClockwise##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
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