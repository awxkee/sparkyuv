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
#define HWY_TARGET_INCLUDE "src/ChannelLength.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "yuv-inl.h"
#include "Wide-inl.h"
#include "Saturate-inl.h"

#if HWY_ONCE
namespace sparkyuv {

HWY_EXPORT(WideRGBA8To10HWY);
HWY_EXPORT(WideRGB8To10HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(WideBGRA8To10HWY);
HWY_EXPORT(WideABGR8To10HWY);
HWY_EXPORT(WideARGB8To10HWY);
HWY_EXPORT(WideBGR8To10HWY);
#endif

HWY_EXPORT(WideRGBA8To12HWY);
HWY_EXPORT(WideRGB8To12HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(WideBGRA8To12HWY);
HWY_EXPORT(WideABGR8To12HWY);
HWY_EXPORT(WideARGB8To12HWY);
HWY_EXPORT(WideBGR8To12HWY);
#endif

HWY_EXPORT(WideRGBA8To16HWY);
HWY_EXPORT(WideRGB8To16HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(WideBGRA8To16HWY);
HWY_EXPORT(WideABGR8To16HWY);
HWY_EXPORT(WideARGB8To16HWY);
HWY_EXPORT(WideBGR8To16HWY);
#endif

HWY_EXPORT(WideRGBA8HWY);
HWY_EXPORT(WideRGB8HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(WideBGRA8HWY);
HWY_EXPORT(WideABGR8HWY);
HWY_EXPORT(WideARGB8HWY);
HWY_EXPORT(WideBGR8HWY);
#endif

#define WideXXXXFrom8ToN_DECLARATION_E(pixelType, targetBitDepth) \
HWY_DLLEXPORT void Wide##pixelType##8To##targetBitDepth(uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                        uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                        const uint32_t width, const uint32_t height) {\
    HWY_DYNAMIC_DISPATCH(Wide##pixelType##8To##targetBitDepth##HWY)(src, srcStride, dst, dstStride, width, height);\
  }

WideXXXXFrom8ToN_DECLARATION_E(RGBA, 10)
WideXXXXFrom8ToN_DECLARATION_E(RGB, 10)
#if SPARKYUV_FULL_CHANNELS
WideXXXXFrom8ToN_DECLARATION_E(BGRA, 10)
WideXXXXFrom8ToN_DECLARATION_E(ABGR, 10)
WideXXXXFrom8ToN_DECLARATION_E(ARGB, 10)
WideXXXXFrom8ToN_DECLARATION_E(BGR, 10)
#endif

WideXXXXFrom8ToN_DECLARATION_E(RGBA, 12)
WideXXXXFrom8ToN_DECLARATION_E(RGB, 12)
#if SPARKYUV_FULL_CHANNELS
WideXXXXFrom8ToN_DECLARATION_E(BGRA, 12)
WideXXXXFrom8ToN_DECLARATION_E(ABGR, 12)
WideXXXXFrom8ToN_DECLARATION_E(ARGB, 12)
WideXXXXFrom8ToN_DECLARATION_E(BGR, 12)
#endif

WideXXXXFrom8ToN_DECLARATION_E(RGBA, 16)
WideXXXXFrom8ToN_DECLARATION_E(RGB, 16)
#if SPARKYUV_FULL_CHANNELS
WideXXXXFrom8ToN_DECLARATION_E(BGRA, 16)
WideXXXXFrom8ToN_DECLARATION_E(ABGR, 16)
WideXXXXFrom8ToN_DECLARATION_E(ARGB, 16)
WideXXXXFrom8ToN_DECLARATION_E(BGR, 16)
#endif

#undef WideXXXXFrom8ToN_DECLARATION_E

#define WIDE8_XXXX_DECLARATION_E(pixelType) \
HWY_DLLEXPORT void Wide##pixelType##8(uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                        uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                        const uint32_t width, const uint32_t height, const int bitDepth) {\
    HWY_DYNAMIC_DISPATCH(Wide##pixelType##8##HWY)(src, srcStride, dst, dstStride, width, height, bitDepth);\
  }

WIDE8_XXXX_DECLARATION_E(RGBA)
WIDE8_XXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
WIDE8_XXXX_DECLARATION_E(BGRA)
WIDE8_XXXX_DECLARATION_E(ABGR)
WIDE8_XXXX_DECLARATION_E(ARGB)
WIDE8_XXXX_DECLARATION_E(BGR)
#endif

#undef WIDE8_XXXX_DECLARATION_E

HWY_EXPORT(SaturateRGBA10To8HWY);
HWY_EXPORT(SaturateRGB10To8HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(SaturateBGRA10To8HWY);
HWY_EXPORT(SaturateABGR10To8HWY);
HWY_EXPORT(SaturateARGB10To8HWY);
HWY_EXPORT(SaturateBGR10To8HWY);
#endif

HWY_EXPORT(SaturateRGBA12To8HWY);
HWY_EXPORT(SaturateRGB12To8HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(SaturateBGRA12To8HWY);
HWY_EXPORT(SaturateABGR12To8HWY);
HWY_EXPORT(SaturateARGB12To8HWY);
HWY_EXPORT(SaturateBGR12To8HWY);
#endif

HWY_EXPORT(SaturateRGBA16To8HWY);
HWY_EXPORT(SaturateRGB16To8HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(SaturateBGRA16To8HWY);
HWY_EXPORT(SaturateABGR16To8HWY);
HWY_EXPORT(SaturateARGB16To8HWY);
HWY_EXPORT(SaturateBGR16To8HWY);
#endif

HWY_EXPORT(SaturateRGBA16HWY);
HWY_EXPORT(SaturateRGB16HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(SaturateBGRA16HWY);
HWY_EXPORT(SaturateABGR16HWY);
HWY_EXPORT(SaturateARGB16HWY);
HWY_EXPORT(SaturateBGR16HWY);
#endif

#define SaturateXXXXFrom8ToN_DECLARATION_E(pixelType, sourceBitDepth) \
HWY_DLLEXPORT void Saturate##pixelType##sourceBitDepth##To8(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                            uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                            const uint32_t width, const uint32_t height) {\
    HWY_DYNAMIC_DISPATCH(Saturate##pixelType##sourceBitDepth##To8HWY)(src, srcStride, dst, dstStride, width, height);\
  }

SaturateXXXXFrom8ToN_DECLARATION_E(RGBA, 10)
SaturateXXXXFrom8ToN_DECLARATION_E(RGB, 10)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFrom8ToN_DECLARATION_E(BGRA, 10)
SaturateXXXXFrom8ToN_DECLARATION_E(ABGR, 10)
SaturateXXXXFrom8ToN_DECLARATION_E(ARGB, 10)
SaturateXXXXFrom8ToN_DECLARATION_E(BGR, 10)
#endif

SaturateXXXXFrom8ToN_DECLARATION_E(RGBA, 12)
SaturateXXXXFrom8ToN_DECLARATION_E(RGB, 12)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFrom8ToN_DECLARATION_E(BGRA, 12)
SaturateXXXXFrom8ToN_DECLARATION_E(ABGR, 12)
SaturateXXXXFrom8ToN_DECLARATION_E(ARGB, 12)
SaturateXXXXFrom8ToN_DECLARATION_E(BGR, 12)
#endif

SaturateXXXXFrom8ToN_DECLARATION_E(RGBA, 16)
SaturateXXXXFrom8ToN_DECLARATION_E(RGB, 16)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFrom8ToN_DECLARATION_E(BGR, 16)
SaturateXXXXFrom8ToN_DECLARATION_E(BGRA, 16)
SaturateXXXXFrom8ToN_DECLARATION_E(ABGR, 16)
SaturateXXXXFrom8ToN_DECLARATION_E(ARGB, 16)
#endif

#undef SaturateXXXXFrom8ToN_DECLARATION_E

#define SATURATE_XXXX16_DYNAMIC_E(pixelType) \
HWY_DLLEXPORT void Saturate##pixelType##To8(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                            uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                            const uint32_t width, const uint32_t height, const int bitDepth) {\
    HWY_DYNAMIC_DISPATCH(Saturate##pixelType##16##HWY)(src, srcStride, dst, dstStride, width, height, bitDepth);\
  }

SATURATE_XXXX16_DYNAMIC_E(RGBA)
SATURATE_XXXX16_DYNAMIC_E(RGB)
#if SPARKYUV_FULL_CHANNELS
SATURATE_XXXX16_DYNAMIC_E(BGR)
SATURATE_XXXX16_DYNAMIC_E(BGRA)
SATURATE_XXXX16_DYNAMIC_E(ABGR)
SATURATE_XXXX16_DYNAMIC_E(ARGB)
#endif

#undef SATURATE_XXXX16_DYNAMIC_E

}
#endif