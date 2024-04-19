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

#if defined(SPARKYUV_SATURATE_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_SATURATE_INL_H
#undef SPARKYUV_SATURATE_INL_H
#else
#define SPARKYUV_SATURATE_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include "Transpose-inl.h"
#include "CopyImage-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL, SparkYuvRotation rotation,
    typename std::enable_if<rotation == sparkyuv::sRotate180, int>::type = 0>
void
RotateImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
           T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
           const uint32_t width, const uint32_t height) {
  const ScalableTag<T> d;
  const int lanes = Lanes(d);

  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mRotatedPtr = reinterpret_cast<uint8_t *>(dst) + (height - 1) * srcStride;

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    T *mDestination = reinterpret_cast<T *>(mRotatedPtr + (width - 1) * channels);
    const T *mSource = reinterpret_cast<const T *>(mSourcePtr);

    if (x + lanes < width) {
      mDestination += channels;
      for (; x + lanes < width; x += lanes) {
        mDestination -= channels * lanes;

        if (Surface == sparkyuv::SURFACE_CHANNEL) {
          const auto v = LoadU(d, mSource);
          StoreU(Reverse(d, v), d, mDestination);
        } else if (Surface == sparkyuv::SURFACE_CHANNELS_3) {
          using V = Vec<decltype(d)>;
          V i1, i2, i3;
          LoadInterleaved3(d, mSource, i1, i2, i3);
          StoreInterleaved3(Reverse(d, i1), Reverse(d, i2), Reverse(d, i3), d, mDestination);
        } else {
          using V = Vec<decltype(d)>;
          V i1, i2, i3, i4;
          LoadInterleaved4(d, mSource, i1, i2, i3, i4);
          StoreInterleaved4(Reverse(d, i1), Reverse(d, i2), Reverse(d, i3), Reverse(d, i4), d, mDestination);
        }

        mSource += channels * lanes;
      }
      mDestination -= channels;
    }

    for (; x < width; ++x) {
      if (Surface == sparkyuv::SURFACE_CHANNEL) {
        mDestination[0] = mSource[0];
      } else if (Surface == sparkyuv::SURFACE_CHANNELS_3) {
        mDestination[0] = mSource[0];
        mDestination[1] = mSource[1];
        mDestination[2] = mSource[2];
      } else {
        mDestination[0] = mSource[0];
        mDestination[1] = mSource[1];
        mDestination[2] = mSource[2];
        mDestination[3] = mSource[3];
      }
      mSource += channels;
      mDestination -= channels;
    }

    mSourcePtr += srcStride;
    mRotatedPtr -= newStride;
  }
}

#define ROTATE_DECLARATION_R(srcPixel, storageType, surfaceType) \
    static void Rotate##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                      storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                      const uint32_t width, const uint32_t height, SparkYuvRotation rotation) {\
        if (rotation == sparkyuv::sRotate180) {                  \
        RotateImpl<storageType, sparkyuv::SURFACE_##surfaceType, sparkyuv::sRotate180>(src, srcStride, dst, dstStride,\
                                                                                       width, height); \
        } else if (rotation == sparkyuv::sRotate90) {            \
            TransposeClockwiseImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride,         \
              dst, dstStride, width, height); \
        } else if (rotation == sparkyuv::sRotate270) {           \
            TransposeCounterClockwiseImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride,  \
              dst, dstStride, width, height); \
        } else if (rotation == sparkyuv::sRotate0) {             \
            CopyImageImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride,  \
              dst, dstStride, width, height);                    \
        }\
    }

ROTATE_DECLARATION_R(RGBA, uint8_t, CHANNELS_4)
ROTATE_DECLARATION_R(RGB, uint8_t, CHANNELS_3)
ROTATE_DECLARATION_R(Channel, uint8_t, CHANNEL)
ROTATE_DECLARATION_R(RGBA1010102, uint8_t, RGBA1010102)

ROTATE_DECLARATION_R(RGBA16, uint16_t, CHANNELS_4)
ROTATE_DECLARATION_R(RGB16, uint16_t, CHANNELS_3)
ROTATE_DECLARATION_R(Channel16, uint16_t, CHANNEL)

#undef ROTATE_DECLARATION_R
}
HWY_AFTER_NAMESPACE();

#endif
