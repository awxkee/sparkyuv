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

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "src/Transpose.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "sparkyuv.h"
#include "yuv-inl.h"
#include "Transpose-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define TRANSPOSE_CLOCKWISE_EXPORT(srcPixel, storageType)\
  HWY_EXPORT(TransposeClockwise##srcPixel##HWY);

TRANSPOSE_CLOCKWISE_EXPORT(RGBA, uint8_t)
TRANSPOSE_CLOCKWISE_EXPORT(RGB, uint8_t)
TRANSPOSE_CLOCKWISE_EXPORT(Channel, uint8_t)
TRANSPOSE_CLOCKWISE_EXPORT(RGBA1010102, uint8_t)

TRANSPOSE_CLOCKWISE_EXPORT(RGBA16, uint16_t)
TRANSPOSE_CLOCKWISE_EXPORT(RGB16, uint16_t)
TRANSPOSE_CLOCKWISE_EXPORT(Channel16, uint16_t)

#undef TRANSPOSE_CLOCKWISE_EXPORT

#define TRANSPOSE_CLOCKWISE_DECLARATION_R(srcPixel, storageType, surfaceType) \
    void TransposeClockwise##srcPixel(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                      storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                      const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(TransposeClockwise##srcPixel##HWY)(src, srcStride, dst, dstStride,\
                                                                width, height); \
    }

TRANSPOSE_CLOCKWISE_DECLARATION_R(RGBA, uint8_t, CHANNELS_4)
TRANSPOSE_CLOCKWISE_DECLARATION_R(RGB, uint8_t, CHANNELS_3)
TRANSPOSE_CLOCKWISE_DECLARATION_R(Channel, uint8_t, CHANNEL)
TRANSPOSE_CLOCKWISE_DECLARATION_R(RGBA1010102, uint8_t, RGBA1010102)

TRANSPOSE_CLOCKWISE_DECLARATION_R(RGBA16, uint16_t, CHANNELS_4)
TRANSPOSE_CLOCKWISE_DECLARATION_R(RGB16, uint16_t, CHANNELS_3)
TRANSPOSE_CLOCKWISE_DECLARATION_R(Channel16, uint16_t, CHANNEL)

#undef TRANSPOSE_CLOCKWISE_DECLARATION_R

#define TRANSPOSE_CCW_EXPORT(srcPixel, storageType)\
  HWY_EXPORT(TransposeCounterClockwise##srcPixel##HWY);

TRANSPOSE_CCW_EXPORT(RGBA, uint8_t)
TRANSPOSE_CCW_EXPORT(RGB, uint8_t)
TRANSPOSE_CCW_EXPORT(Channel, uint8_t)
TRANSPOSE_CCW_EXPORT(RGBA1010102, uint8_t)

TRANSPOSE_CCW_EXPORT(RGBA16, uint16_t)
TRANSPOSE_CCW_EXPORT(RGB16, uint16_t)
TRANSPOSE_CCW_EXPORT(Channel16, uint16_t)

#undef TRANSPOSE_CCW_EXPORT

#define TRANSPOSE_CCW_DECLARATION_E(srcPixel, storageType, surfaceType) \
    void TransposeCounterClockwise##srcPixel(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                           storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                           const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(TransposeCounterClockwise##srcPixel##HWY)(src, srcStride, dst, dstStride,\
                                                                       width, height); \
    }

TRANSPOSE_CCW_DECLARATION_E(RGBA, uint8_t, CHANNELS_4)
TRANSPOSE_CCW_DECLARATION_E(RGB, uint8_t, CHANNELS_3)
TRANSPOSE_CCW_DECLARATION_E(Channel, uint8_t, CHANNEL)
TRANSPOSE_CCW_DECLARATION_E(RGBA1010102, uint8_t, RGBA1010102)

TRANSPOSE_CCW_DECLARATION_E(RGBA16, uint16_t, CHANNELS_4)
TRANSPOSE_CCW_DECLARATION_E(RGB16, uint16_t, CHANNELS_3)
TRANSPOSE_CCW_DECLARATION_E(Channel16, uint16_t, CHANNEL)

#undef TRANSPOSE_CCW_DECLARATION_E

}
#endif