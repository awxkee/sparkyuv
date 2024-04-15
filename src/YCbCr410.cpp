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
#define HWY_TARGET_INCLUDE "src/YCbCr410.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "sparkyuv.h"
#include "yuv-inl.h"
#include "YCbCr411-inl.h"
#include "YCbCr411P16-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define DECLARE_YCbCr410_TO_RGB(pixel) HWY_EXPORT(YCbCr410To##pixel##HWY);

DECLARE_YCbCr410_TO_RGB(RGBA)
DECLARE_YCbCr410_TO_RGB(RGB)
#if SPARKYUV_FULL_CHANNELS
DECLARE_YCbCr410_TO_RGB(ARGB)
DECLARE_YCbCr410_TO_RGB(ABGR)
DECLARE_YCbCr410_TO_RGB(BGRA)
DECLARE_YCbCr410_TO_RGB(BGR)
#endif

#undef DECLARE_YCbCr410_TO_RGB

#define DECLARE_RGB_TO_YCbCr410(pixel) HWY_EXPORT(pixel##ToYCbCr410HWY);

DECLARE_RGB_TO_YCbCr410(RGBA)
DECLARE_RGB_TO_YCbCr410(RGB)
#if SPARKYUV_FULL_CHANNELS
DECLARE_RGB_TO_YCbCr410(ARGB)
DECLARE_RGB_TO_YCbCr410(ABGR)
DECLARE_RGB_TO_YCbCr410(BGRA)
DECLARE_RGB_TO_YCbCr410(BGR)
#endif

#undef DECLARE_RGB_TO_YCbCr410

#define XXXX_TO_YCbCr410_DECLARATION_E(pixelType) \
 void pixelType##ToYCbCr410(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                            const uint32_t width, const uint32_t height,\
                            uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                            uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                            uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
   HWY_DYNAMIC_DISPATCH(pixelType##ToYCbCr410HWY)(src, srcStride, width, height,\
                                                  yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                  kr, kb, colorRange);\
 }

XXXX_TO_YCbCr410_DECLARATION_E(RGBA)
XXXX_TO_YCbCr410_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXX_TO_YCbCr410_DECLARATION_E(ARGB)
XXXX_TO_YCbCr410_DECLARATION_E(ABGR)
XXXX_TO_YCbCr410_DECLARATION_E(BGRA)
XXXX_TO_YCbCr410_DECLARATION_E(BGR)
#endif

#undef XXXX_TO_YCbCr410_DECLARATION_E

#define YCbCr410_TO_RGBX_DECLARATION_E(pixelType) \
 void YCbCr410To##pixelType(uint8_t *SPARKYUV_RESTRICT dst,const uint32_t dstStride,\
                            const uint32_t width,const uint32_t height,\
                            const uint8_t *SPARKYUV_RESTRICT yPlane,const uint32_t yStride,\
                            const uint8_t *SPARKYUV_RESTRICT uPlane,const uint32_t uStride,\
                            const uint8_t *SPARKYUV_RESTRICT vPlane,const uint32_t vStride,\
                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
   HWY_DYNAMIC_DISPATCH(YCbCr410To##pixelType##HWY)(dst, dstStride, width, height,\
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,\
                              kr, kb, colorRange);\
 }

YCbCr410_TO_RGBX_DECLARATION_E(RGBA)
YCbCr410_TO_RGBX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr410_TO_RGBX_DECLARATION_E(ARGB)
YCbCr410_TO_RGBX_DECLARATION_E(ABGR)
YCbCr410_TO_RGBX_DECLARATION_E(BGRA)
YCbCr410_TO_RGBX_DECLARATION_E(BGR)
#endif

#undef YCbCr410_TO_RGBX_DECLARATION_E

// MARK: P16

#define YCbCr410_TO_RGBXP16_DECLARATION_E(pixelType, bit, yuvname) HWY_EXPORT(pixelType##To##yuvname##P##bit##HWY);

YCbCr410_TO_RGBXP16_DECLARATION_E(RGBA, 10, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(RGB, 10, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410_TO_RGBXP16_DECLARATION_E(ARGB, 10, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(ABGR, 10, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(BGRA, 10, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(BGR, 10, YCbCr410)
#endif

YCbCr410_TO_RGBXP16_DECLARATION_E(RGBA, 12, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(RGB, 12, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410_TO_RGBXP16_DECLARATION_E(ARGB, 12, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(ABGR, 12, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(BGRA, 12, YCbCr410)
YCbCr410_TO_RGBXP16_DECLARATION_E(BGR, 12, YCbCr410)
#endif

#undef YCbCr410_TO_RGBXP16_DECLARATION_E

#define YCbCr410PXToXXXX_DECLARATION_E(pixelType, bit, yuvname) HWY_EXPORT(yuvname##P##bit##To##pixelType##HWY);

YCbCr410PXToXXXX_DECLARATION_E(RGBA, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(RGB, 10, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410PXToXXXX_DECLARATION_E(ARGB, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(ABGR, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGRA, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGR, 10, YCbCr410)
#endif

YCbCr410PXToXXXX_DECLARATION_E(RGBA, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(RGB, 12, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410PXToXXXX_DECLARATION_E(ARGB, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(ABGR, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGRA, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGR, 12, YCbCr410)
#endif

#undef YCbCr410PXToXXXX_DECLARATION_E

#define XXXXToYCbCr410PHWY_DECLARATION_E(pixelType, bit, yuvname) \
 void pixelType##bit##To##yuvname##P##bit(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                         const uint32_t width, const uint32_t height,\
                                         uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                         uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                         uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                         const float kr, const float kb, const SparkYuvColorRange colorRange) {\
   HWY_DYNAMIC_DISPATCH(pixelType##To##yuvname##P##bit##HWY)(src, srcStride, width, height,\
                                                             yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                             kr, kb, colorRange);\
 }

XXXXToYCbCr410PHWY_DECLARATION_E(RGBA, 10, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(RGB, 10, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr410PHWY_DECLARATION_E(ARGB, 10, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(ABGR, 10, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(BGRA, 10, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(BGR, 10, YCbCr410)
#endif

XXXXToYCbCr410PHWY_DECLARATION_E(RGBA, 12, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(RGB, 12, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr410PHWY_DECLARATION_E(ARGB, 12, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(ABGR, 12, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(BGRA, 12, YCbCr410)
XXXXToYCbCr410PHWY_DECLARATION_E(BGR, 12, YCbCr410)
#endif

#undef XXXXToYCbCr410PHWY_DECLARATION_E

#define YCbCr410PXToXXXX_DECLARATION_E(pixelType, bit, yuvname) \
 void yuvname##P##bit##To##pixelType##bit(uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                          const uint32_t width, const uint32_t height,\
                                          const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                          const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                          const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                          const float kr, const float kb, const SparkYuvColorRange colorRange) {\
   HWY_DYNAMIC_DISPATCH(yuvname##P##bit##To##pixelType##HWY)(src, srcStride, width, height,\
                                              yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                              kr, kb, colorRange);\
 }

YCbCr410PXToXXXX_DECLARATION_E(RGBA, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(RGB, 10, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410PXToXXXX_DECLARATION_E(ARGB, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(ABGR, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGRA, 10, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGR, 10, YCbCr410)
#endif

YCbCr410PXToXXXX_DECLARATION_E(RGBA, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(RGB, 12, YCbCr410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410PXToXXXX_DECLARATION_E(ARGB, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(ABGR, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGRA, 12, YCbCr410)
YCbCr410PXToXXXX_DECLARATION_E(BGR, 12, YCbCr410)
#endif

#undef YCbCr410PXToXXXX_DECLARATION_E

}
#endif
