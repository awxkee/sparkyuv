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
#define HWY_TARGET_INCLUDE "src/YCbCr411.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "sparkyuv.h"
#include "yuv-inl.h"
#include "YCbCr411-inl.h"
#include "YCbCr411P16-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define DECLARE_YCbCr411_TO_RGB(pixel) HWY_EXPORT(YCbCr411To##pixel##HWY);

DECLARE_YCbCr411_TO_RGB(RGBA)
DECLARE_YCbCr411_TO_RGB(RGB)
#if SPARKYUV_FULL_CHANNELS
DECLARE_YCbCr411_TO_RGB(ARGB)
DECLARE_YCbCr411_TO_RGB(ABGR)
DECLARE_YCbCr411_TO_RGB(BGRA)
DECLARE_YCbCr411_TO_RGB(BGR)
#endif

#undef DECLARE_YCbCr411_TO_RGB

#define DECLARE_RGB_TO_YCbCr411(pixel) HWY_EXPORT(pixel##ToYCbCr411HWY);

DECLARE_RGB_TO_YCbCr411(RGBA)
DECLARE_RGB_TO_YCbCr411(RGB)
#if SPARKYUV_FULL_CHANNELS
DECLARE_RGB_TO_YCbCr411(ARGB)
DECLARE_RGB_TO_YCbCr411(ABGR)
DECLARE_RGB_TO_YCbCr411(BGRA)
DECLARE_RGB_TO_YCbCr411(BGR)
#endif

#undef DECLARE_RGB_TO_YCbCr411

#define XXXX_TO_YCbCr411_DECLARATION_E(pixelType) \
 void pixelType##ToYCbCr411(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                            const uint32_t width, const uint32_t height,\
                            uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                            uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                            uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
   HWY_DYNAMIC_DISPATCH(pixelType##ToYCbCr411HWY)(src, srcStride, width, height,\
                                                  yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                  kr, kb, colorRange);\
 }

XXXX_TO_YCbCr411_DECLARATION_E(RGBA)
XXXX_TO_YCbCr411_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXX_TO_YCbCr411_DECLARATION_E(ARGB)
XXXX_TO_YCbCr411_DECLARATION_E(ABGR)
XXXX_TO_YCbCr411_DECLARATION_E(BGRA)
XXXX_TO_YCbCr411_DECLARATION_E(BGR)
#endif

#undef XXXX_TO_YCbCr411_DECLARATION_E

#define YCbCr411_TO_RGBX_DECLARATION_E(pixelType) \
 void YCbCr411To##pixelType(uint8_t *SPARKYUV_RESTRICT dst,const uint32_t dstStride,\
                            const uint32_t width,const uint32_t height,\
                            const uint8_t *SPARKYUV_RESTRICT yPlane,const uint32_t yStride,\
                            const uint8_t *SPARKYUV_RESTRICT uPlane,const uint32_t uStride,\
                            const uint8_t *SPARKYUV_RESTRICT vPlane,const uint32_t vStride,\
                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
   HWY_DYNAMIC_DISPATCH(YCbCr411To##pixelType##HWY)(dst, dstStride, width, height,\
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,\
                              kr, kb, colorRange);\
 }

YCbCr411_TO_RGBX_DECLARATION_E(RGBA)
YCbCr411_TO_RGBX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr411_TO_RGBX_DECLARATION_E(ARGB)
YCbCr411_TO_RGBX_DECLARATION_E(ABGR)
YCbCr411_TO_RGBX_DECLARATION_E(BGRA)
YCbCr411_TO_RGBX_DECLARATION_E(BGR)
#endif

#undef YCbCr411_TO_RGBX_DECLARATION_E

// MARK: P16

#define YCbCr411_TO_RGBXP16_DECLARATION_E(pixelType, bit, yuvname) HWY_EXPORT(pixelType##To##yuvname##P##bit##HWY);

YCbCr411_TO_RGBXP16_DECLARATION_E(RGBA, 10, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(RGB, 10, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411_TO_RGBXP16_DECLARATION_E(ARGB, 10, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(ABGR, 10, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(BGRA, 10, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(BGR, 10, YCbCr411)
#endif

YCbCr411_TO_RGBXP16_DECLARATION_E(RGBA, 12, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(RGB, 12, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411_TO_RGBXP16_DECLARATION_E(ARGB, 12, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(ABGR, 12, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(BGRA, 12, YCbCr411)
YCbCr411_TO_RGBXP16_DECLARATION_E(BGR, 12, YCbCr411)
#endif

#undef YCbCr411_TO_RGBXP16_DECLARATION_E

#define YCbCr411PXToXXXX_DECLARATION_E(pixelType, bit, yuvname) HWY_EXPORT(yuvname##P##bit##To##pixelType##HWY);

YCbCr411PXToXXXX_DECLARATION_E(RGBA, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(RGB, 10, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_E(ARGB, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(ABGR, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGRA, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGR, 10, YCbCr411)
#endif

YCbCr411PXToXXXX_DECLARATION_E(RGBA, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(RGB, 12, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_E(ARGB, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(ABGR, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGRA, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGR, 12, YCbCr411)
#endif

#undef YCbCr411PXToXXXX_DECLARATION_E

#define XXXXToYCbCr411PHWY_DECLARATION_E(pixelType, bit, yuvname) \
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

XXXXToYCbCr411PHWY_DECLARATION_E(RGBA, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(RGB, 10, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr411PHWY_DECLARATION_E(ARGB, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(ABGR, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(BGRA, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(BGR, 10, YCbCr411)
#endif

XXXXToYCbCr411PHWY_DECLARATION_E(RGBA, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(RGB, 12, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr411PHWY_DECLARATION_E(ARGB, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(ABGR, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(BGRA, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_E(BGR, 12, YCbCr411)
#endif

#undef XXXXToYCbCr411PHWY_DECLARATION_E

#define YCbCr411PXToXXXX_DECLARATION_E(pixelType, bit, yuvname) \
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

YCbCr411PXToXXXX_DECLARATION_E(RGBA, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(RGB, 10, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_E(ARGB, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(ABGR, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGRA, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGR, 10, YCbCr411)
#endif

YCbCr411PXToXXXX_DECLARATION_E(RGBA, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(RGB, 12, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_E(ARGB, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(ABGR, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGRA, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_E(BGR, 12, YCbCr411)
#endif

#undef YCbCr411PXToXXXX_DECLARATION_E

}
#endif
