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
#define HWY_TARGET_INCLUDE "src/RGB565Reformat.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "yuv-inl.h"
#include "RGB565Reformat-inl.h"

#if HWY_ONCE
namespace sparkyuv {

#define REFORMAT_TO_RGB565_DECLARE_HWY(PixelName) HWY_EXPORT(PixelName##ToRGB565HWY);
REFORMAT_TO_RGB565_DECLARE_HWY(RGB)
REFORMAT_TO_RGB565_DECLARE_HWY(BGR)
REFORMAT_TO_RGB565_DECLARE_HWY(RGBA)
REFORMAT_TO_RGB565_DECLARE_HWY(BGRA)
REFORMAT_TO_RGB565_DECLARE_HWY(ABGR)
REFORMAT_TO_RGB565_DECLARE_HWY(ARGB)
REFORMAT_TO_RGB565_DECLARE_HWY(RGB16)
REFORMAT_TO_RGB565_DECLARE_HWY(BGR16)
REFORMAT_TO_RGB565_DECLARE_HWY(RGBA16)
REFORMAT_TO_RGB565_DECLARE_HWY(BGRA16)
REFORMAT_TO_RGB565_DECLARE_HWY(ABGR16)
REFORMAT_TO_RGB565_DECLARE_HWY(ARGB16)
REFORMAT_TO_RGB565_DECLARE_HWY(RGBA1010102)
REFORMAT_TO_RGB565_DECLARE_HWY(RGBF16)
REFORMAT_TO_RGB565_DECLARE_HWY(BGRF16)
REFORMAT_TO_RGB565_DECLARE_HWY(RGBAF16)
REFORMAT_TO_RGB565_DECLARE_HWY(BGRAF16)
REFORMAT_TO_RGB565_DECLARE_HWY(ABGRF16)
REFORMAT_TO_RGB565_DECLARE_HWY(ARGBF16)
#undef REFORMAT_TO_RGB565_DECLARE_HWY

#define REFORMATF16_TO_RGB565_E(PixelName, Pixel) \
    void PixelName##ToRGB565HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(PixelName##ToRGB565HWY)(src, srcStride, dst, dstStride,\
                                                     width, height);\
    }

REFORMATF16_TO_RGB565_E(RGBF16, RGB)
REFORMATF16_TO_RGB565_E(BGRF16, BGR)
REFORMATF16_TO_RGB565_E(RGBAF16, RGBA)
REFORMATF16_TO_RGB565_E(BGRAF16, BGRA)
REFORMATF16_TO_RGB565_E(ABGRF16, ABGR)
REFORMATF16_TO_RGB565_E(ARGBF16, ARGB)

#undef REFORMATF16_TO_RGB565_E

#define REFORMAT_TO_RGB565_NBIT(PixelName, Pixel, T) \
    void PixelName##ToRGB565(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(PixelName##ToRGB565HWY)(src, srcStride, dst, dstStride,\
                                                     width, height);\
    }

REFORMAT_TO_RGB565_NBIT(RGB, RGB, uint8_t)
REFORMAT_TO_RGB565_NBIT(BGR, BGR, uint8_t)
REFORMAT_TO_RGB565_NBIT(RGBA, RGBA, uint8_t)
REFORMAT_TO_RGB565_NBIT(BGRA, BGRA, uint8_t)
REFORMAT_TO_RGB565_NBIT(ABGR, ABGR, uint8_t)
REFORMAT_TO_RGB565_NBIT(ARGB, ARGB, uint8_t)
REFORMAT_TO_RGB565_NBIT(RGBA1010102, RGBA1010102, uint8_t)
REFORMAT_TO_RGB565_NBIT(RGBF16, RGB, uint16_t)
REFORMAT_TO_RGB565_NBIT(BGRF16, BGR,uint16_t)
REFORMAT_TO_RGB565_NBIT(RGBAF16, RGBA, uint16_t)
REFORMAT_TO_RGB565_NBIT(BGRAF16, BGRA, uint16_t)
REFORMAT_TO_RGB565_NBIT(ABGRF16, ABGR, uint16_t)
REFORMAT_TO_RGB565_NBIT(ARGBF16, ARGB, uint16_t)

#undef REFORMAT_TO_RGB565_NBIT

#define REFORMAT_TO_RGB565(PixelName, Pixel, T) \
    void PixelName##ToRGB565(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height, const int bitDepth) {\
        HWY_DYNAMIC_DISPATCH(PixelName##ToRGB565HWY)(src, srcStride, dst, dstStride,\
                                                     width, height, bitDepth);\
    }

REFORMAT_TO_RGB565(RGB16, RGB, uint16_t)
REFORMAT_TO_RGB565(BGR16, BGR,uint16_t)
REFORMAT_TO_RGB565(RGBA16, RGBA, uint16_t)
REFORMAT_TO_RGB565(BGRA16, BGRA, uint16_t)
REFORMAT_TO_RGB565(ABGR16, ABGR, uint16_t)
REFORMAT_TO_RGB565(ARGB16, ARGB, uint16_t)

#undef REFORMAT_TO_RGB565

#define REFORMAT_RGB565_TO_PX_DECLARE_HWY(PixelName) HWY_EXPORT(RGB565To##PixelName##HWY);
REFORMAT_RGB565_TO_PX_DECLARE_HWY(RGB)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(BGR)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(RGBA)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(BGRA)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(ABGR)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(ARGB)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(RGB16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(BGR16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(RGBA16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(BGRA16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(ABGR16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(ARGB16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(RGBA1010102)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(RGBF16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(BGRF16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(RGBAF16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(BGRAF16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(ABGRF16)
REFORMAT_RGB565_TO_PX_DECLARE_HWY(ARGBF16)
#undef REFORMAT_RGB565_TO_PX_DECLARE_HWY

#define REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(PixelName, Pixel, T) \
    void RGB565To##PixelName(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                             T *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                             const uint32_t width, const uint32_t height) {\
        HWY_DYNAMIC_DISPATCH(RGB565To##PixelName##HWY)(src, srcStride, dst, dstStride,\
                                                      width, height);\
    }

REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(RGB, RGB, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(BGR, BGR, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(RGBA, RGBA, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(BGRA, BGRA, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(ABGR, ABGR, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(ARGB, ARGB, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(RGBA1010102, RGBA1010102, uint8_t)

REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(RGBF16, RGB, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(BGRF16, BGR, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(RGBAF16, RGBA, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(BGRAF16, BGRA, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(ABGRF16, ABGR, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E(ARGBF16, ARGB, uint16_t)

#undef REFORMAT_RGB565_TO_NEW_BIT_DEFINED_E

#define REFORMAT_RGB565_TO_NEW_E(PixelName, Pixel, T) \
    void RGB565To##PixelName(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                             T *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                             const uint32_t width, const uint32_t height, const int bitDepth) {\
        HWY_DYNAMIC_DISPATCH(RGB565To##PixelName##HWY)(src, srcStride, dst, dstStride,\
                                                      width, height, bitDepth);\
    }

REFORMAT_RGB565_TO_NEW_E(RGB16, RGB, uint16_t)
REFORMAT_RGB565_TO_NEW_E(BGR16, BGR,uint16_t)
REFORMAT_RGB565_TO_NEW_E(RGBA16, RGBA, uint16_t)
REFORMAT_RGB565_TO_NEW_E(BGRA16, BGRA, uint16_t)
REFORMAT_RGB565_TO_NEW_E(ABGR16, ABGR, uint16_t)
REFORMAT_RGB565_TO_NEW_E(ARGB16, ARGB, uint16_t)

#undef REFORMAT_RGB565_TO_NEW_E


}
#endif