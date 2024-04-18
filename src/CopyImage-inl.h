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

#if defined(SPARKYUV_COPY_IMAGE_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_COPY_IMAGE_INL_H
#undef SPARKYUV_COPY_IMAGE_INL_H
#else
#define SPARKYUV_COPY_IMAGE_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL>
static void
CopyImageImpl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
              T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
              const uint32_t width, const uint32_t height) {
  static_assert(Surface == SURFACE_CHANNEL || Surface == SURFACE_CHANNELS_3
                    || Surface == SURFACE_CHANNELS_4
                    || Surface == SURFACE_RGBA1010102, "Unknown surface type");
  const ScalableTag<T> d;
  using V = Vec<decltype(d)>;
  const int lanes = Lanes(d);
  const int channels = Surface == SURFACE_CHANNEL ? 1 : (Surface == SURFACE_CHANNELS_3 ? 3 : 4);

  auto mSourcePtr = reinterpret_cast<const uint8_t *>(src);
  auto mDestinationPtr = reinterpret_cast<uint8_t *>(dst);

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    T *mDestination = reinterpret_cast<T *>(mDestinationPtr);
    const T *mSource = reinterpret_cast<const T *>(mSourcePtr);

    for (; x + lanes < width; x += lanes) {
      if (Surface == SURFACE_CHANNEL) {
        const V v = LoadU(d, mSource);
        StoreU(v, d, mDestination);
      } else if (Surface == SURFACE_CHANNELS_3) {
        V v1, v2, v3;
        LoadInterleaved3(d, mSource, v1, v2, v3);
        StoreInterleaved3(v1, v2, v3, d, mDestination);
      } else {
        V v1, v2, v3, v4;
        LoadInterleaved4(d, mSource, v1, v2, v3, v4);
        StoreInterleaved4(v1, v2, v3, v4, d, mDestination);
      }

      mSource += channels * lanes;
      mDestination += channels * lanes;
    }

    for (; x < width; ++x) {
      if (Surface == SURFACE_CHANNEL) {
        mDestination[0] = mSource[0];
      } else if (Surface == SURFACE_CHANNELS_3) {
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
      mDestination += channels;
    }

    mSourcePtr += srcStride;
    mDestinationPtr += newStride;
  }
}

#define COPY_IMAGE_DECLARATION_R(srcPixel, storageType, surfaceType) \
    static void Copy##srcPixel##HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                             storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                             const uint32_t width, const uint32_t height) {\
        CopyImageImpl<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                       width, height); \
    }

COPY_IMAGE_DECLARATION_R(RGBA, uint8_t, CHANNELS_4)
COPY_IMAGE_DECLARATION_R(RGB, uint8_t, CHANNELS_3)
COPY_IMAGE_DECLARATION_R(Channel, uint8_t, CHANNEL)
COPY_IMAGE_DECLARATION_R(RGBA1010102, uint8_t, RGBA1010102)

COPY_IMAGE_DECLARATION_R(RGBA16, uint16_t, CHANNELS_4)
COPY_IMAGE_DECLARATION_R(RGB16, uint16_t, CHANNELS_3)
COPY_IMAGE_DECLARATION_R(Channel16, uint16_t, CHANNEL)

#undef COPY_IMAGE_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif