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
#define HWY_TARGET_INCLUDE "src/ChannelsReformat.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "Channels-inl.h"
#include "BlendAlpha-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define CHANNEL_XXX_REFORMAT_DECLARATION_8(srcPixelType, dstPixelType) HWY_EXPORT(srcPixelType##To##dstPixelType##HWY);

CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, RGBA1010102)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA1010102, RGBA)

CHANNEL_XXX_REFORMAT_DECLARATION_8(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ARGB, BGR)

CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ARGB, BGRA)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_8

#define CHANNEL_XXX_REFORMAT_DECLARATION_E(srcPixelType, dstPixelType) \
    void srcPixelType##To##dstPixelType(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                        uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(srcPixelType##To##dstPixelType##HWY)(src, srcStride, dst, dstStride, width, height); \
    }

CHANNEL_XXX_REFORMAT_DECLARATION_E(RGBA, RGBA1010102)
CHANNEL_XXX_REFORMAT_DECLARATION_E(RGBA1010102, RGBA)

CHANNEL_XXX_REFORMAT_DECLARATION_E(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_E(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_E(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_E(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_E(BGRA, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_E(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_E(ARGB, BGRA)

CHANNEL_XXX_REFORMAT_DECLARATION_E(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E(ARGB, BGR)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_E

HWY_EXPORT(RGBA16ToRGBA1010102HWY);

#define CHANNEL_XXX_REFORMAT_DECLARATION_16(srcPixelType, dstPixelType) HWY_EXPORT(srcPixelType##16To##dstPixelType##16HWY);
CHANNEL_XXX_REFORMAT_DECLARATION_16(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_16(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ARGB, BGR)

CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ARGB, BGRA)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_8

#define CHANNEL_XXX_REFORMAT_DECLARATION_1010102(srcPixelType, dstPixelType) \
    void srcPixelType##16To##dstPixelType##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                               uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                               const uint32_t width, const uint32_t height, const int depth) {\
       HWY_DYNAMIC_DISPATCH(srcPixelType##16To##dstPixelType##HWY)(src, srcStride, dst, dstStride, width, height, depth); \
    }

CHANNEL_XXX_REFORMAT_DECLARATION_1010102(RGBA, RGBA1010102)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_1010102

#define CHANNEL_XXX_REFORMAT_DECLARATION_E_16(srcPixelType, dstPixelType) \
    void srcPixelType##To##dstPixelType(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                        uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height, const int bitDepth) {\
        HWY_DYNAMIC_DISPATCH(srcPixelType##16To##dstPixelType##16HWY)(src, srcStride, dst, dstStride, width, height, bitDepth); \
    }

CHANNEL_XXX_REFORMAT_DECLARATION_E_16(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_E_16(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_E_16(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(ARGB, BGR)

CHANNEL_XXX_REFORMAT_DECLARATION_E_16(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_E_16(ARGB, BGRA)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_E_16

//MARK: Reformat to F16

#define CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(srcPixel, dstPixel) HWY_EXPORT(srcPixel##To##dstPixel##F16HWY);

CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(RGBA1010102, RGBA)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(RGBA, RGBA)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(RGB, RGB)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(Channel, Channel)

CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(RGBA16, RGBA16)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(RGB16, RGB16)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY(Channel16, Channel16)

#undef CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_HWY

#define CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_E(srcPixel, dstPixel, storageType) \
    void srcPixel##To##dstPixel##F16(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                       uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(srcPixel##To##dstPixel##F16HWY)(src, srcStride, dst, dstStride,\
                                                               width, height); \
    }

CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_E(RGBA1010102, RGBA, uint8_t)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_E(RGBA, RGBA, uint8_t)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_E(RGB, RGB, uint8_t)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_E(Channel, Channel, uint8_t)

#undef CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_E

#define CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_E(pixelType, declareType, storageType) \
    void pixelType##To##declareType##F16(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                       uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height, const int depth) {\
        HWY_DYNAMIC_DISPATCH(pixelType##To##pixelType##F16HWY)(src, srcStride, dst, dstStride,\
                                                               width, height, depth); \
    }

CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_E(RGBA16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_E(RGB16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_E(Channel16, RGBA, uint16_t)

#undef CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_E

// MARK: Reformat from F16

#define CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(srcPixel, dstPixel) HWY_EXPORT(srcPixel##F16To##dstPixel##HWY);

CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(RGBA, RGBA1010102)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(RGBA, RGBA)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(RGB, RGB)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(Channel, Channel)

CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(RGBA16, RGBA16)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(RGB16, RGB16)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY(Channel16, Channel16)

#undef CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_HWY

#define CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_E(srcPixel, dstPixel, storageType) \
    void srcPixel##F16To##dstPixel(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                     storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                     const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(srcPixel##F16To##dstPixel##HWY)(src, srcStride, dst, dstStride,\
                                                               width, height); \
    }

CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_E(RGBA, RGBA1010102, uint8_t)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_E(RGBA, RGBA, uint8_t)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_E(RGB, RGB, uint8_t)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_E(Channel, Channel, uint8_t)

#undef CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_E

#define CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_E(pixelType, declareType, storageType) \
    void declareType##F16To##pixelType(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                       storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height, const int depth) {\
        HWY_DYNAMIC_DISPATCH(pixelType##F16To##pixelType##HWY)(src, srcStride, dst, dstStride,\
                                                               width, height, depth); \
    }

CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_E(RGBA16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_E(RGB16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_E(Channel16, Channel, uint16_t)

#undef CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_E

HWY_EXPORT(RGBAPremultiplyAlphaHWY);
HWY_EXPORT(ARGBPremultiplyAlphaHWY);
HWY_EXPORT(BGRAPremultiplyAlphaHWY);
HWY_EXPORT(ABGRPremultiplyAlphaHWY);

#define PREMULTIPLY_ALPHA_DECLARATION_E(pixelType) \
    void pixelType##PremultiplyAlpha(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                        uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(pixelType##PremultiplyAlphaHWY)(src, srcStride, dst, dstStride, width, height); \
    }

PREMULTIPLY_ALPHA_DECLARATION_E(RGBA)
PREMULTIPLY_ALPHA_DECLARATION_E(ARGB)
PREMULTIPLY_ALPHA_DECLARATION_E(ABGR)
PREMULTIPLY_ALPHA_DECLARATION_E(BGRA)

#undef PREMULTIPLY_ALPHA_DECLARATION_E

HWY_EXPORT(RGBAUnpremultiplyAlphaHWY);
HWY_EXPORT(ARGBUnpremultiplyAlphaHWY);
HWY_EXPORT(BGRAUnpremultiplyAlphaHWY);
HWY_EXPORT(ABGRUnpremultiplyAlphaHWY);

#define UNPREMULTIPLY_ALPHA_DECLARATION_R(pixelType) \
    void pixelType##UnpremultiplyAlpha(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                       uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(pixelType##UnpremultiplyAlphaHWY)(src, srcStride, dst, dstStride, width, height); \
    }

UNPREMULTIPLY_ALPHA_DECLARATION_R(RGBA)
UNPREMULTIPLY_ALPHA_DECLARATION_R(ARGB)
UNPREMULTIPLY_ALPHA_DECLARATION_R(ABGR)
UNPREMULTIPLY_ALPHA_DECLARATION_R(BGRA)

#undef UNPREMULTIPLY_ALPHA_DECLARATION_R

}

#endif