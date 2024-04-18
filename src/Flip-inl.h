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

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL>
void
FlipVerticalImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                 T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                 const uint32_t width, const uint32_t height) {
  const ScalableTag<T> d;
  const int lanes = Lanes(d);

  auto mFlippedSourcePtr = reinterpret_cast<const uint8_t *>(src) + (height - 1) * srcStride;
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  for (uint32_t y = 0; y < height; ++y) {

    uint32_t x = 0;

    T *mDestination = reinterpret_cast<T *>(mDestinationPtr);
    const T *mSource = reinterpret_cast<const T *>(mFlippedSourcePtr);

    for (; x + lanes < width; x += lanes) {
      if (Surface == sparkyuv::SURFACE_CHANNEL) {
        const auto v = LoadU(d, mSource);
        StoreU(v, d, mDestination);
        mSource += lanes;
        mDestination += lanes;
      } else if (Surface == sparkyuv::SURFACE_CHANNELS_3) {
        using V = Vec<decltype(d)>;
        V i1, i2, i3;
        LoadInterleaved3(d, mSource, i1, i2, i3);
        StoreInterleaved3(i1, i2, i3, d, mDestination);
        mSource += lanes * 3;
        mDestination += lanes * 3;
      } else {
        using V = Vec<decltype(d)>;
        V i1, i2, i3, i4;
        LoadInterleaved4(d, mSource, i1, i2, i3, i4);
        StoreInterleaved4(i1, i2, i3, i4, d, mDestination);
        mSource += lanes * 4;
        mDestination += lanes * 4;
      }
    }

    for (; x < width; ++x) {
      if (Surface == sparkyuv::SURFACE_CHANNEL) {
        mDestination[0] = mSource[0];
        mDestination++;
        mSource++;
      } else if (Surface == sparkyuv::SURFACE_CHANNELS_3) {
        mDestination[0] = mSource[0];
        mDestination[1] = mSource[1];
        mDestination[2] = mSource[2];
        mDestination += 3;
        mSource += 3;
      } else {
        mDestination[0] = mSource[0];
        mDestination[1] = mSource[1];
        mDestination[2] = mSource[2];
        mDestination[3] = mSource[3];
        mDestination += 4;
        mSource += 4;
      }
    }

    mFlippedSourcePtr -= srcStride;
    mDestinationPtr += newStride;
  }
}

#define FLIP_VERTICAL_XXX_DECLARATION(srcPixel, storageType, surfaceType) \
    void FlipVertical##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                    storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                    const uint32_t width, const uint32_t height) {\
        FlipVerticalImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                       width, height); \
    }

FLIP_VERTICAL_XXX_DECLARATION(RGBA, uint8_t, CHANNELS_4)
FLIP_VERTICAL_XXX_DECLARATION(RGB, uint8_t, CHANNELS_3)
FLIP_VERTICAL_XXX_DECLARATION(Channel, uint8_t, CHANNEL)
FLIP_VERTICAL_XXX_DECLARATION(RGBA1010102, uint8_t, RGBA1010102)

FLIP_VERTICAL_XXX_DECLARATION(RGBA16, uint16_t, CHANNELS_4)
FLIP_VERTICAL_XXX_DECLARATION(RGB16, uint16_t, CHANNELS_3)
FLIP_VERTICAL_XXX_DECLARATION(Channel16, uint16_t, CHANNEL)

#undef FLIP_VERTICAL_XXX_DECLARATION

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL>
void
FlipHorizontalImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                   T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                   const uint32_t width, const uint32_t height) {
  const ScalableTag<T> d;
  const int lanes = Lanes(d);

  auto mFlippedSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  for (uint32_t y = 0; y < height; ++y) {

    int64_t x = width - 1;

    T *mDestination = reinterpret_cast<T *>(mDestinationPtr + (width - 1)*channels);
    const T *mSource = reinterpret_cast<const T *>(mFlippedSourcePtr);

    if (x - lanes >= 0) {
      mDestination = reinterpret_cast<T *>(mDestinationPtr + width*channels);
      while (x - lanes >= 0) {
        mDestination -= lanes * channels;
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
        mSource += lanes * channels;
        x -= lanes;
      }
    }

    for (; x >= 0; --x) {
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
      mDestination -= channels;
      mSource += channels;
    }

    mFlippedSourcePtr += srcStride;
    mDestinationPtr += newStride;
  }
}

#define FLIP_HORIZONTAL_DECLARATION_R(srcPixel, storageType, surfaceType) \
    void FlipHorizontal##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                    storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                    const uint32_t width, const uint32_t height) {\
        FlipHorizontalImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                       width, height); \
    }

FLIP_HORIZONTAL_DECLARATION_R(RGBA, uint8_t, CHANNELS_4)
FLIP_HORIZONTAL_DECLARATION_R(RGB, uint8_t, CHANNELS_3)
FLIP_HORIZONTAL_DECLARATION_R(Channel, uint8_t, CHANNEL)
FLIP_HORIZONTAL_DECLARATION_R(RGBA1010102, uint8_t, RGBA1010102)

FLIP_HORIZONTAL_DECLARATION_R(RGBA16, uint16_t, CHANNELS_4)
FLIP_HORIZONTAL_DECLARATION_R(RGB16, uint16_t, CHANNELS_3)
FLIP_HORIZONTAL_DECLARATION_R(Channel16, uint16_t, CHANNEL)

#undef FLIP_HORIZONTAL_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif