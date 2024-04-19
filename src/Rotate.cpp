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
#define HWY_TARGET_INCLUDE "src/Rotate.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "sparkyuv.h"
#include "yuv-inl.h"
#include "Rotate-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define ROTATE_EXPORT(srcPixel, storageType)\
  HWY_EXPORT(Rotate##srcPixel##HWY);

ROTATE_EXPORT(RGBA, uint8_t)
ROTATE_EXPORT(RGB, uint8_t)
ROTATE_EXPORT(Channel, uint8_t)
ROTATE_EXPORT(RGBA1010102, uint8_t)

ROTATE_EXPORT(RGBA16, uint16_t)
ROTATE_EXPORT(RGB16, uint16_t)
ROTATE_EXPORT(Channel16, uint16_t)

#undef ROTATE_EXPORT

#define ROTATE_DECLARATION_E(srcPixel, storageType, surfaceType) \
    void Rotate##srcPixel(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                          storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                          const uint32_t width, const uint32_t height, SparkYuvRotation rotation) {\
        HWY_DYNAMIC_DISPATCH(Rotate##srcPixel##HWY)(src, srcStride, dst, dstStride, width, height, rotation); \
    }

ROTATE_DECLARATION_E(RGBA, uint8_t, CHANNELS_4)
ROTATE_DECLARATION_E(RGB, uint8_t, CHANNELS_3)
ROTATE_DECLARATION_E(Channel, uint8_t, CHANNEL)
ROTATE_DECLARATION_E(RGBA1010102, uint8_t, RGBA1010102)

ROTATE_DECLARATION_E(RGBA16, uint16_t, CHANNELS_4)
ROTATE_DECLARATION_E(RGB16, uint16_t, CHANNELS_3)
ROTATE_DECLARATION_E(Channel16, uint16_t, CHANNEL)

#undef ROTATE_DECLARATION_E

}
#endif