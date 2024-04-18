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
#define HWY_TARGET_INCLUDE "src/Flip.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "Flip-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define FLIP_VERTICAL_DECLARE_EXPORT(srcPixel, storageType)\
  HWY_EXPORT(FlipVertical##srcPixel##HWY);

FLIP_VERTICAL_DECLARE_EXPORT(RGBA, uint8_t)
FLIP_VERTICAL_DECLARE_EXPORT(RGB, uint8_t)
FLIP_VERTICAL_DECLARE_EXPORT(Channel, uint8_t)
FLIP_VERTICAL_DECLARE_EXPORT(RGBA1010102, uint8_t)

FLIP_VERTICAL_DECLARE_EXPORT(RGBA16, uint16_t)
FLIP_VERTICAL_DECLARE_EXPORT(RGB16, uint16_t)
FLIP_VERTICAL_DECLARE_EXPORT(Channel16, uint16_t)

#undef FLIP_VERTICAL_DECLARE_EXPORT

#define FLIP_VERTICAL_DECLARATION_E(srcPixel, storageType, surfaceType) \
    void FlipVertical##srcPixel(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(FlipVertical##srcPixel##HWY)(src, srcStride, dst, dstStride,\
                                                                        width, height); \
    }

FLIP_VERTICAL_DECLARATION_E(RGBA, uint8_t, CHANNELS_4)
FLIP_VERTICAL_DECLARATION_E(RGB, uint8_t, CHANNELS_3)
FLIP_VERTICAL_DECLARATION_E(Channel, uint8_t, CHANNEL)
FLIP_VERTICAL_DECLARATION_E(RGBA1010102, uint8_t, RGBA1010102)

FLIP_VERTICAL_DECLARATION_E(RGBA16, uint16_t, CHANNELS_4)
FLIP_VERTICAL_DECLARATION_E(RGB16, uint16_t, CHANNELS_3)
FLIP_VERTICAL_DECLARATION_E(Channel16, uint16_t, CHANNEL)

#undef FLIP_VERTICAL_DECLARATION_E

#define FLIP_HORIZONTAL_DECLARE_EXPORT(srcPixel, storageType)\
  HWY_EXPORT(FlipHorizontal##srcPixel##HWY);

FLIP_HORIZONTAL_DECLARE_EXPORT(RGBA, uint8_t)
FLIP_HORIZONTAL_DECLARE_EXPORT(RGB, uint8_t)
FLIP_HORIZONTAL_DECLARE_EXPORT(Channel, uint8_t)
FLIP_HORIZONTAL_DECLARE_EXPORT(RGBA1010102, uint8_t)

FLIP_HORIZONTAL_DECLARE_EXPORT(RGBA16, uint16_t)
FLIP_HORIZONTAL_DECLARE_EXPORT(RGB16, uint16_t)
FLIP_HORIZONTAL_DECLARE_EXPORT(Channel16, uint16_t)

#undef FLIP_HORIZONTAL_DECLARE_EXPORT

#define FLIP_HORIZONTAL_DECLARATION_E(srcPixel, storageType, surfaceType) \
    void FlipHorizontal##srcPixel(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(FlipHorizontal##srcPixel##HWY)(src, srcStride, dst, dstStride,\
                                                                        width, height); \
    }

FLIP_HORIZONTAL_DECLARATION_E(RGBA, uint8_t, CHANNELS_4)
FLIP_HORIZONTAL_DECLARATION_E(RGB, uint8_t, CHANNELS_3)
FLIP_HORIZONTAL_DECLARATION_E(Channel, uint8_t, CHANNEL)
FLIP_HORIZONTAL_DECLARATION_E(RGBA1010102, uint8_t, RGBA1010102)

FLIP_HORIZONTAL_DECLARATION_E(RGBA16, uint16_t, CHANNELS_4)
FLIP_HORIZONTAL_DECLARATION_E(RGB16, uint16_t, CHANNELS_3)
FLIP_HORIZONTAL_DECLARATION_E(Channel16, uint16_t, CHANNEL)

#undef FLIP_HORIZONTAL_DECLARATION_E
}
#endif
