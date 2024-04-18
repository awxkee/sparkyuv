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
#define HWY_TARGET_INCLUDE "src/CopyImage.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "CopyImage-inl.h"

#if HWY_ONCE
namespace sparkyuv {

#define COPY_IMAGE_EXPORT_DECLARATION(srcPixel, storageType) HWY_EXPORT(Copy##srcPixel##HWY);

COPY_IMAGE_EXPORT_DECLARATION(RGBA, uint8_t)
COPY_IMAGE_EXPORT_DECLARATION(RGB, uint8_t)
COPY_IMAGE_EXPORT_DECLARATION(Channel, uint8_t)
COPY_IMAGE_EXPORT_DECLARATION(RGBA1010102, uint8_t)

COPY_IMAGE_EXPORT_DECLARATION(RGBA16, uint16_t)
COPY_IMAGE_EXPORT_DECLARATION(RGB16, uint16_t)
COPY_IMAGE_EXPORT_DECLARATION(Channel16, uint16_t)

#undef COPY_IMAGE_EXPORT_DECLARATION

#define COPY_IMAGE_DECLARATION_E(srcPixel, storageType, surfaceType) \
    void Copy##srcPixel(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                        storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                        const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(Copy##srcPixel##HWY)(src, srcStride, dst, dstStride,\
                                                  width, height); \
    }

COPY_IMAGE_DECLARATION_E(RGBA, uint8_t, CHANNELS_4)
COPY_IMAGE_DECLARATION_E(RGB, uint8_t, CHANNELS_3)
COPY_IMAGE_DECLARATION_E(Channel, uint8_t, CHANNEL)
COPY_IMAGE_DECLARATION_E(RGBA1010102, uint8_t, RGBA1010102)

COPY_IMAGE_DECLARATION_E(RGBA16, uint16_t, CHANNELS_4)
COPY_IMAGE_DECLARATION_E(RGB16, uint16_t, CHANNELS_3)
COPY_IMAGE_DECLARATION_E(Channel16, uint16_t, CHANNEL)

#undef COPY_IMAGE_DECLARATION_E

}
#endif