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
#define HWY_TARGET_INCLUDE "src/sparkyuv.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "hwy/cache_control.h"
#include "sparkyuv.h"
#include "yuv-inl.h"
#include "YCbCr420-inl.h"
#include "YCbCr422-inl.h"
#include "YCbCr444-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define YCbCr420ToXXXX_DECLARATION_E(pixelType) \
  HWY_DLLEXPORT void \
  YCbCr420To##pixelType(uint8_t *SPARKYUV_RESTRICT rgba,const uint32_t rgbaStride,\
                      const uint32_t width, const uint32_t height,\
                      const uint8_t *SPARKYUV_RESTRICT ySrc, const uint32_t yPlaneStride,\
                      const uint8_t *SPARKYUV_RESTRICT uSrc, const uint32_t uPlaneStride,\
                      const uint8_t *SPARKYUV_RESTRICT vSrc, const uint32_t vPlaneStride,\
                      const float kr, const float kb, const SparkYuvColorRange colorRange) {\
    HWY_DYNAMIC_DISPATCH(YCbCr420To##pixelType##HWY)(rgba, rgbaStride,\
                                                   width, height,\
                                                   ySrc, yPlaneStride,\
                                                   uSrc, uPlaneStride,\
                                                   vSrc, vPlaneStride,\
                                                   kr, kb, colorRange); \
  }

#define YCbCr420ToXXXX_DECLARATION_HWY(pixelType) HWY_EXPORT(YCbCr420To##pixelType##HWY);

YCbCr420ToXXXX_DECLARATION_HWY(RGBA)
YCbCr420ToXXXX_DECLARATION_HWY(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr420ToXXXX_DECLARATION_HWY(ARGB)
YCbCr420ToXXXX_DECLARATION_HWY(ABGR)
YCbCr420ToXXXX_DECLARATION_HWY(BGRA)
YCbCr420ToXXXX_DECLARATION_HWY(BGR)
#endif

#undef YCbCr420ToXXXX_DECLARATION_HWY

YCbCr420ToXXXX_DECLARATION_E(RGBA)
YCbCr420ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr420ToXXXX_DECLARATION_E(ARGB)
YCbCr420ToXXXX_DECLARATION_E(ABGR)
YCbCr420ToXXXX_DECLARATION_E(BGRA)
YCbCr420ToXXXX_DECLARATION_E(BGR)
#endif

#undef YCbCr420ToXXXX_DECLARATION_E

// MARK: YCbCr444 To RGBX

HWY_EXPORT(YCbCr444ToRGBAHWY);
HWY_EXPORT(YCbCr444ToRGBHWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(YCbCr444ToBGRAHWY);
HWY_EXPORT(YCbCr444ToABGRHWY);
HWY_EXPORT(YCbCr444ToARGBHWY);
HWY_EXPORT(YCbCr444ToBGRHWY);
#endif

#define YCbCr444ToXXXX_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void YCbCr444To##pixelType(uint8_t *SPARKYUV_RESTRICT src, uint32_t srcStride, \
                                            uint32_t width, uint32_t height, \
                                            const uint8_t *SPARKYUV_RESTRICT yPlane, uint32_t yStride, \
                                            const uint8_t *SPARKYUV_RESTRICT uPlane, uint32_t uStride, \
                                            const uint8_t *SPARKYUV_RESTRICT vPlane, uint32_t vStride, \
                                            float kr, float kb, SparkYuvColorRange colorRange) { \
        HWY_DYNAMIC_DISPATCH(YCbCr444To##pixelType##HWY)(src, srcStride, width, height, yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                   kr, kb, colorRange); \
    }

YCbCr444ToXXXX_DECLARATION_E(RGBA)
YCbCr444ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr444ToXXXX_DECLARATION_E(ARGB)
YCbCr444ToXXXX_DECLARATION_E(ABGR)
YCbCr444ToXXXX_DECLARATION_E(BGRA)
YCbCr444ToXXXX_DECLARATION_E(BGR)
#endif

#undef YCbCr444ToXXXX_DECLARATION

// MARK: YCbCr422

HWY_EXPORT(YCbCr422ToRGBAHWY);
HWY_EXPORT(YCbCr422ToRGBHWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(YCbCr422ToBGRAHWY);
HWY_EXPORT(YCbCr422ToABGRHWY);
HWY_EXPORT(YCbCr422ToARGBHWY);
HWY_EXPORT(YCbCr422ToBGRHWY);
#endif

#define YCbCr422ToXXXX_DECLARATION_E(pixelType) \
HWY_DLLEXPORT void \
YCbCr422To##pixelType(uint8_t *SPARKYUV_RESTRICT rgba, const uint32_t rgbaStride, \
                    const uint32_t width, const uint32_t height, \
                    const uint8_t *SPARKYUV_RESTRICT ySrc, uint32_t yPlaneStride, \
                    const uint8_t *SPARKYUV_RESTRICT uSrc, uint32_t uPlaneStride, \
                    const uint8_t *SPARKYUV_RESTRICT vSrc, uint32_t vPlaneStride, \
                    const float kr, const float kb, const SparkYuvColorRange colorRange) { \
                       HWY_DYNAMIC_DISPATCH(YCbCr422To##pixelType##HWY)(rgba, rgbaStride, width, height, \
                                                           ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                                                           kr, kb, colorRange);\
                    }

YCbCr422ToXXXX_DECLARATION_E(RGBA)
YCbCr422ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr422ToXXXX_DECLARATION_E(ARGB)
YCbCr422ToXXXX_DECLARATION_E(ABGR)
YCbCr422ToXXXX_DECLARATION_E(BGRA)
YCbCr422ToXXXX_DECLARATION_E(BGR)
#endif

#undef YCbCr422ToXXXX_DECLARATION_E

// MARK: RGBX To YCbCr444

HWY_EXPORT(RGBAToYCbCr444HWY);
HWY_EXPORT(RGBToYCbCr444HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(ARGBToYCbCr444HWY);
HWY_EXPORT(BGRAToYCbCr444HWY);
HWY_EXPORT(ABGRToYCbCr444HWY);
HWY_EXPORT(BGRToYCbCr444HWY);
#endif

#define XXXXToYCbCr444_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void pixelType##ToYCbCr444(const uint8_t *SPARKYUV_RESTRICT src, \
                                           const uint32_t srcStride, const uint32_t width, const uint32_t height, \
                                           uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride, \
                                           uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride, \
                                           uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride, \
                                           const float kr, const float kb, const SparkYuvColorRange colorRange) { \
            HWY_DYNAMIC_DISPATCH(pixelType##ToYCbCr444HWY)(src, srcStride, width, height, \
                                                          yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                          kr, kb, colorRange); \
    }

XXXXToYCbCr444_DECLARATION_E(RGBA)
XXXXToYCbCr444_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444_DECLARATION_E(ARGB)
XXXXToYCbCr444_DECLARATION_E(ABGR)
XXXXToYCbCr444_DECLARATION_E(BGRA)
XXXXToYCbCr444_DECLARATION_E(BGR)
#endif

#undef XXXXToYCbCr444_DECLARATION_E

// MARK: RGBX To YCbCr422

HWY_EXPORT(RGBAToYCbCr422HWY);
HWY_EXPORT(RGBToYCbCr422HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(ARGBToYCbCr422HWY);
HWY_EXPORT(BGRAToYCbCr422HWY);
HWY_EXPORT(ABGRToYCbCr422HWY);
HWY_EXPORT(BGRToYCbCr422HWY);
#endif

#define XXXXToYCbCr422_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void pixelType##ToYCbCr422(const uint8_t *SPARKYUV_RESTRICT src, \
                                           const uint32_t srcStride, const uint32_t width, const uint32_t height, \
                                           uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride, \
                                           uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride, \
                                           uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride, \
                                           const float kr, const float kb, const SparkYuvColorRange colorRange) { \
            HWY_DYNAMIC_DISPATCH(pixelType##ToYCbCr422HWY)(src, srcStride, width, height, \
                                                         yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                         kr, kb, colorRange); \
    }

XXXXToYCbCr422_DECLARATION_E(RGBA)
XXXXToYCbCr422_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr422_DECLARATION_E(ARGB)
XXXXToYCbCr422_DECLARATION_E(ABGR)
XXXXToYCbCr422_DECLARATION_E(BGRA)
XXXXToYCbCr422_DECLARATION_E(BGR)
#endif

#undef XXXXToYCbCr422_DECLARATION_E

HWY_EXPORT(RGBToYCbCr420HWY);
HWY_EXPORT(RGBAToYCbCr420HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(BGRToYCbCr420HWY);
HWY_EXPORT(BGRAToYCbCr420HWY);
HWY_EXPORT(ABGRToYCbCr420HWY);
HWY_EXPORT(ARGBToYCbCr420HWY);
#endif

#define XXXXToYCbCr420_DECLARATION_E(pixelType) \
HWY_DLLEXPORT void pixelType##ToYCbCr420(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                       const uint32_t width, const uint32_t height,\
                                       uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                       uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                       uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                       const float kr, const float kb, const SparkYuvColorRange colorRange) {\
    HWY_DYNAMIC_DISPATCH(pixelType##ToYCbCr420HWY)(src, srcStride,\
                                         width, height,\
                                         yPlane, yStride, uPlane, uStride, vPlane, vStride,\
                                         kr, kb, colorRange);\
  }

XXXXToYCbCr420_DECLARATION_E(RGBA)
XXXXToYCbCr420_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr420_DECLARATION_E(ARGB)
XXXXToYCbCr420_DECLARATION_E(ABGR)
XXXXToYCbCr420_DECLARATION_E(BGRA)
XXXXToYCbCr420_DECLARATION_E(BGR)
#endif

#undef XXXXToYCbCr420_DECLARATION_E

}
#endif
