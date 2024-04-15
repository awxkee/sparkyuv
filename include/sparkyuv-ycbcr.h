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

#ifndef YUV_INCLUDE_SPARKYUV_YCBCR_H_
#define YUV_INCLUDE_SPARKYUV_YCBCR_H_

namespace sparkyuv {
#define YCbCr400ToXXXX_DECLARATION_H(pixel) \
    void YCbCr400To##pixel(uint8_t * src, const uint32_t srcStride,\
                           const uint32_t width, const uint32_t height,\
                           const uint8_t * yPlane, const uint32_t yStride,\
                           const float kr, const float kb, const SparkYuvColorRange colorRange);

YCbCr400ToXXXX_DECLARATION_H(RGBA)
YCbCr400ToXXXX_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr400ToXXXX_DECLARATION_H(ARGB)
YCbCr400ToXXXX_DECLARATION_H(ABGR)
YCbCr400ToXXXX_DECLARATION_H(BGRA)
YCbCr400ToXXXX_DECLARATION_H(BGR)
#endif

#undef YCbCr400ToXXXX_DECLARATION_H

#define XXXX_TO_YCbCr400_H(pixel) \
        void pixel##ToYCbCr400(const uint8_t * src, const uint32_t srcStride,\
                               const uint32_t width, const uint32_t height,\
                               uint8_t * yPlane, const uint32_t yStride,\
                               const float kr, const float kb, const SparkYuvColorRange colorRange);

XXXX_TO_YCbCr400_H(RGBA)
XXXX_TO_YCbCr400_H(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXX_TO_YCbCr400_H(ARGB)
XXXX_TO_YCbCr400_H(ABGR)
XXXX_TO_YCbCr400_H(BGRA)
XXXX_TO_YCbCr400_H(BGR)
#endif

#undef XXXX_TO_YCbCr400_H

#define RGBX_TO_YCbCr400_DECLARATION_H(pixelType, bit, yuvname) \
        void pixelType##bit##To##yuvname##P##bit(const uint16_t * src, const uint32_t srcStride,\
                                                 const uint32_t width, const uint32_t height,\
                                                 uint16_t * yPlane, const uint32_t yStride,\
                                                 const float kr, const float kb, const SparkYuvColorRange colorRange);

RGBX_TO_YCbCr400_DECLARATION_H(RGBA, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
RGBX_TO_YCbCr400_DECLARATION_H(ARGB, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(ABGR, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(BGRA, 10, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(BGR, 10, YCbCr400)
#endif

RGBX_TO_YCbCr400_DECLARATION_H(RGBA, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
RGBX_TO_YCbCr400_DECLARATION_H(ARGB, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(ABGR, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(BGRA, 12, YCbCr400)
RGBX_TO_YCbCr400_DECLARATION_H(BGR, 12, YCbCr400)
#endif

#define YCbCr400_TO_RGBX_DECLARATION_H(pixelType, bit, yuvname) \
    void yuvname##P##bit##To##pixelType##bit(uint16_t * src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t * yPlane, const uint32_t yStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange);

YCbCr400_TO_RGBX_DECLARATION_H(RGBA, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400_TO_RGBX_DECLARATION_H(ARGB, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(ABGR, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(BGRA, 10, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(BGR, 10, YCbCr400)
#endif

YCbCr400_TO_RGBX_DECLARATION_H(RGBA, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400_TO_RGBX_DECLARATION_H(ARGB, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(ABGR, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(BGRA, 12, YCbCr400)
YCbCr400_TO_RGBX_DECLARATION_H(BGR, 12, YCbCr400)
#endif

#undef YCbCr400_TO_RGBX_DECLARATION_H

#define XXXX_TO_YCbCr411_DECLARATION_H(pixelType) \
        void pixelType##ToYCbCr411(const uint8_t * src, const uint32_t srcStride,\
                                   const uint32_t width, const uint32_t height,\
                                   uint8_t * yPlane, const uint32_t yStride,\
                                   uint8_t * uPlane, const uint32_t uStride,\
                                   uint8_t * vPlane, const uint32_t vStride,\
                                   const float kr, const float kb, const SparkYuvColorRange colorRange);

XXXX_TO_YCbCr411_DECLARATION_H(RGBA)
XXXX_TO_YCbCr411_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXX_TO_YCbCr411_DECLARATION_H(ARGB)
XXXX_TO_YCbCr411_DECLARATION_H(ABGR)
XXXX_TO_YCbCr411_DECLARATION_H(BGRA)
XXXX_TO_YCbCr411_DECLARATION_H(BGR)
#endif

#undef XXXX_TO_YCbCr411_DECLARATION_H

#define YCbCr411_TO_RGBX_DECLARATION_H(pixelType) \
        void YCbCr411To##pixelType(uint8_t * dst,const uint32_t dstStride,\
                                   const uint32_t width,const uint32_t height,\
                                   const uint8_t * yPlane,const uint32_t yStride,\
                                   const uint8_t * uPlane,const uint32_t uStride,\
                                   const uint8_t * vPlane,const uint32_t vStride,\
                                   const float kr, const float kb, const SparkYuvColorRange colorRange);

YCbCr411_TO_RGBX_DECLARATION_H(RGBA)
YCbCr411_TO_RGBX_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr411_TO_RGBX_DECLARATION_H(ARGB)
YCbCr411_TO_RGBX_DECLARATION_H(ABGR)
YCbCr411_TO_RGBX_DECLARATION_H(BGRA)
YCbCr411_TO_RGBX_DECLARATION_H(BGR)
#endif

#undef YCbCr411_TO_RGBX_DECLARATION_H

#define XXXXToYCbCr411PHWY_DECLARATION_H(pixelType, bit, yuvname) \
 void pixelType##bit##To##yuvname##P##bit(const uint16_t * src, const uint32_t srcStride,\
                                          const uint32_t width, const uint32_t height,\
                                          uint16_t * yPlane, const uint32_t yStride,\
                                          uint16_t * uPlane, const uint32_t uStride,\
                                          uint16_t * vPlane, const uint32_t vStride,\
                                          const float kr, const float kb, const SparkYuvColorRange colorRange);

XXXXToYCbCr411PHWY_DECLARATION_H(RGBA, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(RGB, 10, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr411PHWY_DECLARATION_H(ARGB, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(ABGR, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(BGRA, 10, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(BGR, 10, YCbCr411)
#endif

XXXXToYCbCr411PHWY_DECLARATION_H(RGBA, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(RGB, 12, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr411PHWY_DECLARATION_H(ARGB, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(ABGR, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(BGRA, 12, YCbCr411)
XXXXToYCbCr411PHWY_DECLARATION_H(BGR, 12, YCbCr411)
#endif

#undef XXXXToYCbCr411PHWY_DECLARATION_H

#define YCbCr411PXToXXXX_DECLARATION_H(pixelType, bit, yuvname) \
 void yuvname##P##bit##To##pixelType##bit(uint16_t * src, const uint32_t srcStride,\
                                          const uint32_t width, const uint32_t height,\
                                          const uint16_t * yPlane, const uint32_t yStride,\
                                          const uint16_t * uPlane, const uint32_t uStride,\
                                          const uint16_t * vPlane, const uint32_t vStride,\
                                          const float kr, const float kb, const SparkYuvColorRange colorRange);

YCbCr411PXToXXXX_DECLARATION_H(RGBA, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(RGB, 10, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_H(ARGB, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(ABGR, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(BGRA, 10, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(BGR, 10, YCbCr411)
#endif

YCbCr411PXToXXXX_DECLARATION_H(RGBA, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(RGB, 12, YCbCr411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_H(ARGB, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(ABGR, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(BGRA, 12, YCbCr411)
YCbCr411PXToXXXX_DECLARATION_H(BGR, 12, YCbCr411)
#endif

#undef YCbCr411PXToXXXX_DECLARATION_H
}

#endif //YUV_INCLUDE_SPARKYUV_YCBCR_H_
