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
#define HWY_TARGET_INCLUDE "src/YCbCr400.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "sparkyuv.h"
#include "yuv-inl.h"
#include "YCbCr400-inl.h"
#include "YCbCr400P16-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define DECLARE_YCbCr400_TO_RGB(pixel) HWY_EXPORT(YCbCr400To##pixel##HWY);

DECLARE_YCbCr400_TO_RGB(RGBA)
DECLARE_YCbCr400_TO_RGB(RGB)
#if SPARKYUV_FULL_CHANNELS
DECLARE_YCbCr400_TO_RGB(ARGB)
DECLARE_YCbCr400_TO_RGB(ABGR)
DECLARE_YCbCr400_TO_RGB(BGRA)
DECLARE_YCbCr400_TO_RGB(BGR)
#endif

#undef DECLARE_YCbCr400_TO_RGB

#define DECLARE_RGB_TO_YCbCr400(pixel) HWY_EXPORT(pixel##ToYCbCr400HWY);

DECLARE_RGB_TO_YCbCr400(RGBA)
DECLARE_RGB_TO_YCbCr400(RGB)
#if SPARKYUV_FULL_CHANNELS
DECLARE_RGB_TO_YCbCr400(ARGB)
DECLARE_RGB_TO_YCbCr400(ABGR)
DECLARE_RGB_TO_YCbCr400(BGRA)
DECLARE_RGB_TO_YCbCr400(BGR)
#endif

#undef DECLARE_RGB_TO_YCbCr400

#define XXXX_TO_YCbCr400_E(pixel) \
        void pixel##ToYCbCr400(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                               const uint32_t width, const uint32_t height,\
                               uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                               const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          HWY_DYNAMIC_DISPATCH(pixel##ToYCbCr400HWY)(src, srcStride, width, height,\
                                                     yPlane, yStride, kr, kb, colorRange);\
        }

XXXX_TO_YCbCr400_E(RGBA)
XXXX_TO_YCbCr400_E(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXX_TO_YCbCr400_E(ARGB)
XXXX_TO_YCbCr400_E(ABGR)
XXXX_TO_YCbCr400_E(BGRA)
XXXX_TO_YCbCr400_E(BGR)
#endif

#undef XXXX_TO_YCbCr400_E

#define YCbCr400ToXXXX_DECLARATION_E(pixel) \
    void YCbCr400To##pixel(uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                          const uint32_t width, const uint32_t height,\
                          const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                          const float kr, const float kb, const SparkYuvColorRange colorRange) {\
         HWY_DYNAMIC_DISPATCH(YCbCr400To##pixel##HWY)(src, srcStride, width, height,\
                                                      yPlane, yStride, kr, kb, colorRange);\
    }

YCbCr400ToXXXX_DECLARATION_E(RGBA)
YCbCr400ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr400ToXXXX_DECLARATION_E(ARGB)
YCbCr400ToXXXX_DECLARATION_E(ABGR)
YCbCr400ToXXXX_DECLARATION_E(BGRA)
YCbCr400ToXXXX_DECLARATION_E(BGR)
#endif

#undef YCbCr400ToXXXX_DECLARATION_E

// 16 Bit 

#define DECLARE_YCbCr400_EXPORT_TO_YUV(pixelType, bit, yuvname) HWY_EXPORT(pixelType##To##yuvname##P##bit##HWY);

DECLARE_YCbCr400_EXPORT_TO_YUV(RGBA, 10, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
DECLARE_YCbCr400_EXPORT_TO_YUV(ARGB, 10, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(ABGR, 10, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(BGRA, 10, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(BGR, 10, YCbCr400)
#endif

DECLARE_YCbCr400_EXPORT_TO_YUV(RGBA, 12, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
DECLARE_YCbCr400_EXPORT_TO_YUV(ARGB, 12, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(ABGR, 12, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(BGRA, 12, YCbCr400)
DECLARE_YCbCr400_EXPORT_TO_YUV(BGR, 12, YCbCr400)
#endif

#define YCbCr400_TO_RGBX_DECLARATION_E(pixelType, bit, yuvname) HWY_EXPORT(yuvname##P##bit##To##pixelType##HWY);

YCbCr400_TO_RGBX_DECLARATION_E(RGBA, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400_TO_RGBX_DECLARATION_E(ARGB, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(ABGR, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGRA, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGR, 10, YCbCr400)
#endif

YCbCr400_TO_RGBX_DECLARATION_E(RGBA, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400_TO_RGBX_DECLARATION_E(ARGB, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(ABGR, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGRA, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGR, 12, YCbCr400)
#endif

#undef YCbCr400_TO_RGBX_DECLARATION_E

#define RGBX_TO_YCbCr400_DECLARATION_E(pixelType, bit, yuvname) \
 void pixelType##bit##To##yuvname##P##bit(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                          const uint32_t width, const uint32_t height,\
                                          uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                          const float kr, const float kb, const SparkYuvColorRange colorRange) {\
   HWY_DYNAMIC_DISPATCH(pixelType##To##yuvname##P##bit##HWY)(src, srcStride, width, height,\
                                                             yPlane, yStride, kr, kb, colorRange);\
 }

RGBX_TO_YCbCr400_DECLARATION_E(RGBA, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
RGBX_TO_YCbCr400_DECLARATION_E(ARGB, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(ABGR, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(BGRA, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(BGR, 10, YCbCr400)
#endif

RGBX_TO_YCbCr400_DECLARATION_E(RGBA, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
RGBX_TO_YCbCr400_DECLARATION_E(ARGB, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(ABGR, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(BGRA, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_E(BGR, 12, YCbCr400)
#endif

#define YCbCr400_TO_RGBX_DECLARATION_E(pixelType, bit, yuvname) \
    void yuvname##P##bit##To##pixelType##bit(uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange) {\
         HWY_DYNAMIC_DISPATCH(yuvname##P##bit##To##pixelType##HWY)(src, srcStride, width, height, yPlane, yStride, \
                                                                   kr, kb, colorRange);\
    }

YCbCr400_TO_RGBX_DECLARATION_E(RGBA, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400_TO_RGBX_DECLARATION_E(ARGB, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(ABGR, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGRA, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGR, 10, YCbCr400)
#endif

YCbCr400_TO_RGBX_DECLARATION_E(RGBA, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400_TO_RGBX_DECLARATION_E(ARGB, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(ABGR, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGRA, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_E(BGR, 12, YCbCr400)
#endif

#undef YCbCr400_TO_RGBX_DECLARATION_E

}
#endif