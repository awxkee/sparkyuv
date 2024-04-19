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

#include "sparkyuv.h"

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "src/GaussianBlur.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "yuv-inl.h"
#include "GaussianBlur-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define GAUSSIAN_BLUR_DECLARE_EXPORT(srcPixel) HWY_EXPORT(GaussianBlur##srcPixel##HWY);
GAUSSIAN_BLUR_DECLARE_EXPORT(RGBA)
GAUSSIAN_BLUR_DECLARE_EXPORT(RGB)
GAUSSIAN_BLUR_DECLARE_EXPORT(Channel)
GAUSSIAN_BLUR_DECLARE_EXPORT(RGBA16)
GAUSSIAN_BLUR_DECLARE_EXPORT(RGB16)
GAUSSIAN_BLUR_DECLARE_EXPORT(Channel16)
GAUSSIAN_BLUR_DECLARE_EXPORT(RGBAF32)
GAUSSIAN_BLUR_DECLARE_EXPORT(RGBF32)
GAUSSIAN_BLUR_DECLARE_EXPORT(ChannelF32)
#undef GAUSSIAN_BLUR_DECLARE_EXPORT

#define GAUSSIAN_BLUR_DECLARATION_E(srcPixel, storageType) \
    void GaussianBlur##srcPixel(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height,  \
                                const int kernelSize, const float sigma) {\
        HWY_DYNAMIC_DISPATCH(GaussianBlur##srcPixel##HWY)(src, srcStride, dst, dstStride,\
                                                          width, height, kernelSize, sigma); \
    }

GAUSSIAN_BLUR_DECLARATION_E(RGBA, uint8_t)
GAUSSIAN_BLUR_DECLARATION_E(RGB, uint8_t)
GAUSSIAN_BLUR_DECLARATION_E(Channel, uint8_t)

GAUSSIAN_BLUR_DECLARATION_E(RGBA16, uint16_t)
GAUSSIAN_BLUR_DECLARATION_E(RGB16, uint16_t)
GAUSSIAN_BLUR_DECLARATION_E(Channel16, uint16_t)

GAUSSIAN_BLUR_DECLARATION_E(RGBAF32, float)
GAUSSIAN_BLUR_DECLARATION_E(RGBF32, float)
GAUSSIAN_BLUR_DECLARATION_E(ChannelF32, float)

#undef GAUSSIAN_BLUR_DECLARATION_E
}
#endif