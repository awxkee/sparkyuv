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
#define YUV420ToXXXX_DECLARATION_E(pixelType) \
  HWY_DLLEXPORT void \
  YUV420To##pixelType(uint8_t *SPARKYUV_RESTRICT rgba,const uint32_t rgbaStride,\
                      const uint32_t width, const uint32_t height,\
                      const uint8_t *SPARKYUV_RESTRICT ySrc, const uint32_t yPlaneStride,\
                      const uint8_t *SPARKYUV_RESTRICT uSrc, const uint32_t uPlaneStride,\
                      const uint8_t *SPARKYUV_RESTRICT vSrc, const uint32_t vPlaneStride,\
                      const float kr, const float kb, const SparkYuvColorRange colorRange) {\
    HWY_DYNAMIC_DISPATCH(YUV420To##pixelType##HWY)(rgba, rgbaStride,\
                                                   width, height,\
                                                   ySrc, yPlaneStride,\
                                                   uSrc, uPlaneStride,\
                                                   vSrc, vPlaneStride,\
                                                   kr, kb, colorRange); \
  }

#define YUV420ToXXXX_DECLARATION_HWY(pixelType) HWY_EXPORT(YUV420To##pixelType##HWY);

YUV420ToXXXX_DECLARATION_HWY(RGBA)
YUV420ToXXXX_DECLARATION_HWY(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV420ToXXXX_DECLARATION_HWY(ARGB)
YUV420ToXXXX_DECLARATION_HWY(ABGR)
YUV420ToXXXX_DECLARATION_HWY(BGRA)
YUV420ToXXXX_DECLARATION_HWY(BGR)
#endif

#undef YUV420ToXXXX_DECLARATION_HWY

YUV420ToXXXX_DECLARATION_E(RGBA)
YUV420ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV420ToXXXX_DECLARATION_E(ARGB)
YUV420ToXXXX_DECLARATION_E(ABGR)
YUV420ToXXXX_DECLARATION_E(BGRA)
YUV420ToXXXX_DECLARATION_E(BGR)
#endif

#undef YUV420ToXXXX_DECLARATION_E

// MARK: YUV444 To RGBX

HWY_EXPORT(YUV444ToRGBAHWY);
HWY_EXPORT(YUV444ToRGBHWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(YUV444ToBGRAHWY);
HWY_EXPORT(YUV444ToABGRHWY);
HWY_EXPORT(YUV444ToARGBHWY);
HWY_EXPORT(YUV444ToBGRHWY);
#endif

#define YUV444ToXXXX_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void YUV444To##pixelType(uint8_t *SPARKYUV_RESTRICT src, uint32_t srcStride, \
                                            uint32_t width, uint32_t height, \
                                            const uint8_t *SPARKYUV_RESTRICT yPlane, uint32_t yStride, \
                                            const uint8_t *SPARKYUV_RESTRICT uPlane, uint32_t uStride, \
                                            const uint8_t *SPARKYUV_RESTRICT vPlane, uint32_t vStride, \
                                            float kr, float kb, SparkYuvColorRange colorRange) { \
        HWY_DYNAMIC_DISPATCH(YUV444To##pixelType##HWY)(src, srcStride, width, height, yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                   kr, kb, colorRange); \
    }

YUV444ToXXXX_DECLARATION_E(RGBA)
YUV444ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV444ToXXXX_DECLARATION_E(ARGB)
YUV444ToXXXX_DECLARATION_E(ABGR)
YUV444ToXXXX_DECLARATION_E(BGRA)
YUV444ToXXXX_DECLARATION_E(BGR)
#endif

#undef YUV444ToXXXX_DECLARATION

// MARK: YUV422

HWY_EXPORT(YUV422ToRGBAHWY);
HWY_EXPORT(YUV422ToRGBHWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(YUV422ToBGRAHWY);
HWY_EXPORT(YUV422ToABGRHWY);
HWY_EXPORT(YUV422ToARGBHWY);
HWY_EXPORT(YUV422ToBGRHWY);
#endif

#define YUV422ToXXXX_DECLARATION_E(pixelType) \
HWY_DLLEXPORT void \
YUV422To##pixelType(uint8_t *SPARKYUV_RESTRICT rgba, const uint32_t rgbaStride, \
                    const uint32_t width, const uint32_t height, \
                    const uint8_t *SPARKYUV_RESTRICT ySrc, uint32_t yPlaneStride, \
                    const uint8_t *SPARKYUV_RESTRICT uSrc, uint32_t uPlaneStride, \
                    const uint8_t *SPARKYUV_RESTRICT vSrc, uint32_t vPlaneStride, \
                    const float kr, const float kb, const SparkYuvColorRange colorRange) { \
                       HWY_DYNAMIC_DISPATCH(YUV422To##pixelType##HWY)(rgba, rgbaStride, width, height, \
                                                           ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                                                           kr, kb, colorRange);\
                    }

YUV422ToXXXX_DECLARATION_E(RGBA)
YUV422ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV422ToXXXX_DECLARATION_E(ARGB)
YUV422ToXXXX_DECLARATION_E(ABGR)
YUV422ToXXXX_DECLARATION_E(BGRA)
YUV422ToXXXX_DECLARATION_E(BGR)
#endif

#undef YUV422ToXXXX_DECLARATION_E

// MARK: RGBX To YUV444

HWY_EXPORT(RGBAToYUV444HWY);
HWY_EXPORT(RGBToYUV444HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(ARGBToYUV444HWY);
HWY_EXPORT(BGRAToYUV444HWY);
HWY_EXPORT(ABGRToYUV444HWY);
HWY_EXPORT(BGRToYUV444HWY);
#endif

#define XXXXToYUV444_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void pixelType##ToYUV444(const uint8_t *SPARKYUV_RESTRICT src, \
                                           const uint32_t srcStride, const uint32_t width, const uint32_t height, \
                                           uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride, \
                                           uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride, \
                                           uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride, \
                                           const float kr, const float kb, const SparkYuvColorRange colorRange) { \
            HWY_DYNAMIC_DISPATCH(pixelType##ToYUV444HWY)(src, srcStride, width, height, \
                                                          yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                          kr, kb, colorRange); \
    }

XXXXToYUV444_DECLARATION_E(RGBA)
XXXXToYUV444_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV444_DECLARATION_E(ARGB)
XXXXToYUV444_DECLARATION_E(ABGR)
XXXXToYUV444_DECLARATION_E(BGRA)
XXXXToYUV444_DECLARATION_E(BGR)
#endif

#undef XXXXToYUV444_DECLARATION_E

// MARK: RGBX To YUV422

HWY_EXPORT(RGBAToYUV422HWY);
HWY_EXPORT(RGBToYUV422HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(ARGBToYUV422HWY);
HWY_EXPORT(BGRAToYUV422HWY);
HWY_EXPORT(ABGRToYUV422HWY);
HWY_EXPORT(BGRToYUV422HWY);
#endif

#define XXXXToYUV422_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void pixelType##ToYUV422(const uint8_t *SPARKYUV_RESTRICT src, \
                                           const uint32_t srcStride, const uint32_t width, const uint32_t height, \
                                           uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride, \
                                           uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride, \
                                           uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride, \
                                           const float kr, const float kb, const SparkYuvColorRange colorRange) { \
            HWY_DYNAMIC_DISPATCH(pixelType##ToYUV422HWY)(src, srcStride, width, height, \
                                                         yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                         kr, kb, colorRange); \
    }

XXXXToYUV422_DECLARATION_E(RGBA)
XXXXToYUV422_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV422_DECLARATION_E(ARGB)
XXXXToYUV422_DECLARATION_E(ABGR)
XXXXToYUV422_DECLARATION_E(BGRA)
XXXXToYUV422_DECLARATION_E(BGR)
#endif

#undef XXXXToYUV422_DECLARATION_E

HWY_EXPORT(RGBToYUV420HWY);
HWY_EXPORT(RGBAToYUV420HWY);
#if SPARKYUV_FULL_CHANNELS
HWY_EXPORT(BGRToYUV420HWY);
HWY_EXPORT(BGRAToYUV420HWY);
HWY_EXPORT(ABGRToYUV420HWY);
HWY_EXPORT(ARGBToYUV420HWY);
#endif

#define XXXXToYUV420_DECLARATION_E(pixelType) \
HWY_DLLEXPORT void pixelType##ToYUV420(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                       const uint32_t width, const uint32_t height,\
                                       uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                       uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                       uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                       const float kr, const float kb, const SparkYuvColorRange colorRange) {\
    HWY_DYNAMIC_DISPATCH(pixelType##ToYUV420HWY)(src, srcStride,\
                                         width, height,\
                                         yPlane, yStride, uPlane, uStride, vPlane, vStride,\
                                         kr, kb, colorRange);\
  }

XXXXToYUV420_DECLARATION_E(RGBA)
XXXXToYUV420_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV420_DECLARATION_E(ARGB)
XXXXToYUV420_DECLARATION_E(ABGR)
XXXXToYUV420_DECLARATION_E(BGRA)
XXXXToYUV420_DECLARATION_E(BGR)
#endif

#undef XXXXToYUV420_DECLARATION_E

}
#endif
