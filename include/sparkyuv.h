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

#pragma once

#include <cstdint>
#include <vector>
#include "sparkyuv-def.h"
#include "sparkyuv-nv.h"
#include "sparkyuv-rgb565.h"
#include "sparkyuv-eotf.h"
#include "sparkyuv-yccbccrc.h"
#include "sparkyuv-ycgco.h"
#include "sparkyuv-ycgcor.h"
#include "sparkyuv-ydzdx.h"

namespace sparkyuv {

#define YUV420ToXXXX_DECLARATION_H(pixelType) \
  void YUV420To##pixelType(uint8_t * dst, uint32_t rgbaStride,\
                           uint32_t width, uint32_t height,\
                           const uint8_t * ySrc, uint32_t yPlaneStride,\
                           const uint8_t * uSrc, uint32_t uPlaneStride,\
                           const uint8_t * vSrc, uint32_t vPlaneStride,\
                           float kr, float kb, SparkYuvColorRange colorRange);

YUV420ToXXXX_DECLARATION_H(RGBA)
YUV420ToXXXX_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV420ToXXXX_DECLARATION_H(ARGB)
YUV420ToXXXX_DECLARATION_H(ABGR)
YUV420ToXXXX_DECLARATION_H(BGRA)
YUV420ToXXXX_DECLARATION_H(BGR)
#endif

#undef YUV420ToXXXX_DECLARATION_H

#define YUV420ToGEN(pixelType, name, kr, kb, range) \
    static void YUV420##name##To##pixelType(uint8_t * dst, \
                                            uint32_t rgbaStride, uint32_t width, uint32_t height,\
                                            const uint8_t * ySrc, uint32_t yPlaneStride,\
                                            const uint8_t * uSrc, uint32_t uPlaneStride,\
                                            const uint8_t * vSrc, uint32_t vPlaneStride) {\
         YUV420To##pixelType(dst, rgbaStride, width, height,\
                             ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                             kr, kb, range);\
    }

// MARK: YUV420 BT.601

#define YUV420ToGEN601(pixelType) YUV420ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

YUV420ToGEN601(RGBA)
YUV420ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV420ToGEN601(ARGB)
YUV420ToGEN601(ABGR)
YUV420ToGEN601(BGRA)
YUV420ToGEN601(BGR)
#endif

#undef YUV420ToGEN601

// MARK: YUV422 BT.709

#define YUV420ToGEN709(pixelType) YUV420ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

YUV420ToGEN709(RGBA)
YUV420ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV420ToGEN709(ARGB)
YUV420ToGEN709(ABGR)
YUV420ToGEN709(BGRA)
YUV420ToGEN709(BGR)
#endif

#undef YUV420ToGEN709

// MARK: YUV420 BT.2020

#define YUV420ToGEN2020(pixelType) YUV420ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

YUV420ToGEN2020(RGBA)
YUV420ToGEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV420ToGEN2020(ARGB)
YUV420ToGEN2020(ABGR)
YUV420ToGEN2020(BGRA)
YUV420ToGEN2020(BGR)
#endif

#undef YUV420ToGEN2020
#undef YUV420ToGEN

#define YUV422ToXXXX_DECLARATION_H(pixelType) \
void \
YUV422To##pixelType(uint8_t * dst, \
                    uint32_t rgbaStride, uint32_t width, uint32_t height,\
                    const uint8_t * ySrc, uint32_t yPlaneStride,\
                    const uint8_t * uSrc, uint32_t uPlaneStride,\
                    const uint8_t * vSrc, uint32_t vPlaneStride,\
                    float kr, float kb, SparkYuvColorRange colorRange);

YUV422ToXXXX_DECLARATION_H(RGBA)
YUV422ToXXXX_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV422ToXXXX_DECLARATION_H(ARGB)
YUV422ToXXXX_DECLARATION_H(ABGR)
YUV422ToXXXX_DECLARATION_H(BGRA)
YUV422ToXXXX_DECLARATION_H(BGR)
#endif

#undef YUV422ToXXXX_DECLARATION_H

#define YUV422ToGEN(pixelType, name, kr, kb, range) \
    static void YUV422##name##To##pixelType(uint8_t * dst, \
                                            uint32_t rgbaStride, uint32_t width, uint32_t height,\
                                            const uint8_t * ySrc, uint32_t yPlaneStride,\
                                            const uint8_t * uSrc, uint32_t uPlaneStride,\
                                            const uint8_t * vSrc, uint32_t vPlaneStride) {  \
         YUV422To##pixelType(dst, rgbaStride, width, height,\
                             ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                             kr, kb, range); \
    }

// MARK: YUV422 BT.601

#define YUV422ToGEN601(pixelType) YUV422ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

YUV422ToGEN601(RGBA)
YUV422ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV422ToGEN601(ARGB)
YUV422ToGEN601(ABGR)
YUV422ToGEN601(BGRA)
YUV422ToGEN601(BGR)
#endif

#undef YUV422ToGEN601

// MARK: YUV422 BT.709

#define YUV422ToGEN709(pixelType) YUV422ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

YUV422ToGEN709(RGBA)
YUV422ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV422ToGEN709(ARGB)
YUV422ToGEN709(ABGR)
YUV422ToGEN709(BGRA)
YUV422ToGEN709(BGR)
#endif

#undef YUV422ToGEN709

// MARK: YUV422 BT.2020

#define YUV422ToGEN2020(pixelType) YUV422ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

YUV422ToGEN2020(RGBA)
YUV422ToGEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV422ToGEN2020(ARGB)
YUV422ToGEN2020(ABGR)
YUV422ToGEN2020(BGRA)
YUV422ToGEN2020(BGR)
#endif

#undef YUV422ToGEN2020
#undef YUV422ToGEN

//MARK: YUV444 To RGBX

#define YUV444ToXXXX_DECLARATION_H(pixelType) \
    void YUV444To##pixelType(uint8_t * dst, uint32_t srcStride, \
                             uint32_t width, uint32_t height, \
                             const uint8_t * yPlane, uint32_t yStride, \
                             const uint8_t * uPlane, uint32_t uStride, \
                             const uint8_t * vPlane, uint32_t vStride, \
                             float kr, float kb, SparkYuvColorRange colorRange);

YUV444ToXXXX_DECLARATION_H(RGBA)
YUV444ToXXXX_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV444ToXXXX_DECLARATION_H(ARGB)
YUV444ToXXXX_DECLARATION_H(ABGR)
YUV444ToXXXX_DECLARATION_H(BGRA)
YUV444ToXXXX_DECLARATION_H(BGR)
#endif

#undef YUV444ToXXXX_DECLARATION_H

#define YUV444ToGEN(pixelType, name, kr, kb, range) \
    static void YUV444##name##To##pixelType(uint8_t* dst, \
                                            uint32_t rgbaStride, uint32_t width, uint32_t height,\
                                            const uint8_t * ySrc, uint32_t yPlaneStride,\
                                            const uint8_t * uSrc, uint32_t uPlaneStride,\
                                            const uint8_t * vSrc, uint32_t vPlaneStride) {  \
         YUV444To##pixelType(dst, rgbaStride, width, height,\
                             ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                             kr, kb, range); \
    }

// MARK: YUV444 BT.601

#define YUV444ToGEN601(pixelType) YUV444ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

YUV444ToGEN601(RGBA)
YUV444ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV444ToGEN601(ARGB)
YUV444ToGEN601(ABGR)
YUV444ToGEN601(BGRA)
YUV444ToGEN601(BGR)
#endif

#undef YUV444ToGEN601

// MARK: YUV444 BT.709

#define YUV444ToGEN709(pixelType) YUV444ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

YUV444ToGEN709(RGBA)
YUV444ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV444ToGEN709(ARGB)
YUV444ToGEN709(ABGR)
YUV444ToGEN709(BGRA)
YUV444ToGEN709(BGR)
#endif

#undef YUV444ToGEN709

// MARK: YUV444 BT.2020

#define YUV444ToGEN2020(pixelType) YUV444ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

YUV444ToGEN2020(RGBA)
YUV444ToGEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV444ToGEN2020(ARGB)
YUV444ToGEN2020(ABGR)
YUV444ToGEN2020(BGRA)
YUV444ToGEN2020(BGR)
#endif

#undef YUV444ToGEN2020
#undef YUV444ToGEN

// MARK: RGBX To YUV

#define XXXXToYUV444_DECLARATION_H(pixelType) \
    void pixelType##ToYUV444(const uint8_t * src, uint32_t srcStride,\
                             uint32_t width, uint32_t height,\
                             uint8_t * yPlane, uint32_t yStride,\
                             uint8_t * uPlane, uint32_t uStride,\
                             uint8_t * vPlane, uint32_t vStride,\
                             float kr, float kb, SparkYuvColorRange colorRange);

XXXXToYUV444_DECLARATION_H(RGBA)
XXXXToYUV444_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV444_DECLARATION_H(ARGB)
XXXXToYUV444_DECLARATION_H(ABGR)
XXXXToYUV444_DECLARATION_H(BGRA)
XXXXToYUV444_DECLARATION_H(BGR)
#endif

#undef XXXXToYUV444_DECLARATION_H

#define XXXXToYUV444GEN(pixelType, name, kr, kb, range) \
    static void pixelType##ToYUV444##name(const uint8_t * src, uint32_t srcStride,\
                                          uint32_t width, uint32_t height,\
                                          uint8_t * yPlane, uint32_t yStride,\
                                          uint8_t * uPlane, uint32_t uStride,\
                                          uint8_t * vPlane, uint32_t vStride) {\
          pixelType##ToYUV444(src, srcStride, width, height,                                  \
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,              \
                              kr, kb, range); \
    }

#define XXXToYUV444GEN601(pixelType) XXXXToYUV444GEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

XXXToYUV444GEN601(RGBA)
XXXToYUV444GEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXToYUV444GEN601(ARGB)
XXXToYUV444GEN601(ABGR)
XXXToYUV444GEN601(BGRA)
XXXToYUV444GEN601(BGR)
#endif

#undef XXXToYUV444GEN601

#define XXXToYUV444GEN709(pixelType) XXXXToYUV444GEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

XXXToYUV444GEN709(RGBA)
XXXToYUV444GEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXToYUV444GEN709(ARGB)
XXXToYUV444GEN709(ABGR)
XXXToYUV444GEN709(BGRA)
XXXToYUV444GEN709(BGR)
#endif

#undef XXXToYUV444GEN709

#define XXXToYUV444GEN2020(pixelType) XXXXToYUV444GEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

XXXToYUV444GEN2020(RGBA)
XXXToYUV444GEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXToYUV444GEN2020(ARGB)
XXXToYUV444GEN2020(ABGR)
XXXToYUV444GEN2020(BGRA)
XXXToYUV444GEN2020(BGR)
#endif

#undef XXXToYUV444GEN709
#undef XXXXToYUV444GEN

// MARK: RGB to YUV422

#define XXXXToYUV422_DECLARATION_H(pixelType) \
        void pixelType##ToYUV422(const uint8_t * src, uint32_t srcStride,\
                                 uint32_t width, uint32_t height,\
                                 uint8_t * yPlane, uint32_t yStride,\
                                 uint8_t * uPlane, uint32_t uStride,\
                                 uint8_t * vPlane, uint32_t vStride,\
                                 float kr, float kb, SparkYuvColorRange colorRange);

XXXXToYUV422_DECLARATION_H(RGBA)
XXXXToYUV422_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV422_DECLARATION_H(ARGB)
XXXXToYUV422_DECLARATION_H(ABGR)
XXXXToYUV422_DECLARATION_H(BGRA)
XXXXToYUV422_DECLARATION_H(BGR)
#endif

#undef XXXXToYUV422_DECLARATION_H

#define XXXXToYUV422GEN(pixelType, name, kr, kb, range) \
    static void pixelType##ToYUV422##name(const uint8_t * src, uint32_t srcStride,\
                                          uint32_t width, uint32_t height,\
                                          uint8_t * yPlane, uint32_t yStride,\
                                          uint8_t * uPlane, uint32_t uStride,\
                                          uint8_t * vPlane, uint32_t vStride) {\
          pixelType##ToYUV422(src, srcStride, width, height,                                  \
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,              \
                              kr, kb, range); \
    }

#define XXXXToYUV422GEN601(pixelType) XXXXToYUV422GEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

XXXXToYUV422GEN601(RGBA)
XXXXToYUV422GEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV422GEN601(ARGB)
XXXXToYUV422GEN601(ABGR)
XXXXToYUV422GEN601(BGRA)
XXXXToYUV422GEN601(BGR)
#endif

#undef XXXXToYUV422GEN601

#define XXXXToYUV422GEN709(pixelType) XXXXToYUV422GEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

XXXXToYUV422GEN709(RGBA)
XXXXToYUV422GEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV422GEN709(ARGB)
XXXXToYUV422GEN709(ABGR)
XXXXToYUV422GEN709(BGRA)
XXXXToYUV422GEN709(BGR)
#endif

#undef XXXXToYUV422GEN709

#define XXXXToYUV422GEN2020(pixelType) XXXXToYUV422GEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

XXXXToYUV422GEN2020(RGBA)
XXXXToYUV422GEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV422GEN2020(ARGB)
XXXXToYUV422GEN2020(ABGR)
XXXXToYUV422GEN2020(BGRA)
XXXXToYUV422GEN2020(BGR)
#endif

#undef XXXXToYUV422GEN2020
#undef XXXXToYUV422GEN

// MARK: RGB to YUV420

#define XXXXToYUV420_DECLARATION_H(pixelType) \
        void pixelType##ToYUV420(const uint8_t * src, uint32_t srcStride,\
                                 uint32_t width, uint32_t height,\
                                 uint8_t * yPlane, uint32_t yStride,\
                                 uint8_t * uPlane, uint32_t uStride,\
                                 uint8_t * vPlane, uint32_t vStride,\
                                 float kr, float kb, SparkYuvColorRange colorRange);

XXXXToYUV420_DECLARATION_H(RGBA)
XXXXToYUV420_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV420_DECLARATION_H(ARGB)
XXXXToYUV420_DECLARATION_H(ABGR)
XXXXToYUV420_DECLARATION_H(BGRA)
XXXXToYUV420_DECLARATION_H(BGR)
#endif

#undef XXXXToYUV420_DECLARATION_H

#define XXXXToYUV420GEN(pixelType, name, kr, kb, range) \
    static void pixelType##ToYUV420##name(const uint8_t * src, uint32_t srcStride,\
                                          uint32_t width, uint32_t height,\
                                          uint8_t * yPlane, uint32_t yStride,\
                                          uint8_t * uPlane, uint32_t uStride,\
                                          uint8_t * vPlane, uint32_t vStride) {\
          pixelType##ToYUV420(src, srcStride, width, height,                                  \
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,              \
                              kr, kb, range); \
    }

// MARK: YUV420 BT.601

#define XXXXToYUV420GEN601(pixelType) XXXXToYUV420GEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

XXXXToYUV420GEN601(RGBA)
XXXXToYUV420GEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV420GEN601(ARGB)
XXXXToYUV420GEN601(ABGR)
XXXXToYUV420GEN601(BGRA)
XXXXToYUV420GEN601(BGR)
#endif

#undef XXXXToYUV420GEN601

// MARK: YUV420 BT.709

#define XXXXToYUV420GEN709(pixelType) XXXXToYUV420GEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

XXXXToYUV420GEN709(RGBA)
XXXXToYUV420GEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV420GEN709(ARGB)
XXXXToYUV420GEN709(ABGR)
XXXXToYUV420GEN709(BGRA)
XXXXToYUV420GEN709(BGR)
#endif

#undef XXXXToYUV420GEN709

// MARK: YUV420 BT.2020

#define XXXXToYUV420GEN2020(pixelType) XXXXToYUV420GEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

XXXXToYUV420GEN2020(RGBA)
XXXXToYUV420GEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV420GEN2020(ARGB)
XXXXToYUV420GEN2020(ABGR)
XXXXToYUV420GEN2020(BGRA)
XXXXToYUV420GEN2020(BGR)
#endif

#undef XXXXToYUV420GEN2020
#undef XXXXToYUV420GEN

#define WideXXXXFrom8ToN_DECLARATION_H(pixelType, targetBitDepth) \
   void Wide##pixelType##8To##targetBitDepth(uint8_t* src, const uint32_t srcStride, \
                                             uint16_t* dst, const uint32_t dstStride,\
                                             const uint32_t width, const uint32_t height);

WideXXXXFrom8ToN_DECLARATION_H(RGBA, 10)
WideXXXXFrom8ToN_DECLARATION_H(BGRA, 10)
WideXXXXFrom8ToN_DECLARATION_H(ABGR, 10)
WideXXXXFrom8ToN_DECLARATION_H(ARGB, 10)
WideXXXXFrom8ToN_DECLARATION_H(RGB, 10)
WideXXXXFrom8ToN_DECLARATION_H(BGR, 10)

WideXXXXFrom8ToN_DECLARATION_H(RGBA, 12)
WideXXXXFrom8ToN_DECLARATION_H(BGRA, 12)
WideXXXXFrom8ToN_DECLARATION_H(ABGR, 12)
WideXXXXFrom8ToN_DECLARATION_H(ARGB, 12)
WideXXXXFrom8ToN_DECLARATION_H(RGB, 12)
WideXXXXFrom8ToN_DECLARATION_H(BGR, 12)

WideXXXXFrom8ToN_DECLARATION_H(RGBA, 16)
WideXXXXFrom8ToN_DECLARATION_H(BGRA, 16)
WideXXXXFrom8ToN_DECLARATION_H(ABGR, 16)
WideXXXXFrom8ToN_DECLARATION_H(ARGB, 16)
WideXXXXFrom8ToN_DECLARATION_H(RGB, 16)
WideXXXXFrom8ToN_DECLARATION_H(BGR, 16)

#undef WideXXXXFrom8ToN_DECLARATION_H

#define SaturateXXXXFrom8ToN_DECLARATION_H(pixelType, sourceBitDepth) \
   void Saturate##pixelType##sourceBitDepth##To8(const uint16_t* src, const uint32_t srcStride, \
                                                 uint8_t* dst, const uint32_t dstStride,\
                                                 const uint32_t width, const uint32_t height);

SaturateXXXXFrom8ToN_DECLARATION_H(RGBA, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(BGRA, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(ABGR, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(ARGB, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(RGB, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(BGR, 10)

SaturateXXXXFrom8ToN_DECLARATION_H(RGBA, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(BGRA, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(ABGR, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(ARGB, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(RGB, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(BGR, 12)

SaturateXXXXFrom8ToN_DECLARATION_H(RGBA, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(BGRA, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(ABGR, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(ARGB, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(RGB, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(BGR, 16)

#undef SaturateXXXXFrom8ToN_DECLARATION_H

#define YUV444PXToXXXX_DECLARATION_H(yuvname, pixelType, bit) \
    void yuvname##P##bit##To##pixelType##bit(uint16_t * src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t * yPlane, const uint32_t yStride,\
                                             const uint16_t * uPlane, const uint32_t uStride,\
                                             const uint16_t * vPlane, const uint32_t vStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange);

YUV444PXToXXXX_DECLARATION_H(YUV444, RGBA, 10)
YUV444PXToXXXX_DECLARATION_H(YUV444, RGB, 10)
#if SPARKYUV_FULL_CHANNELS
YUV444PXToXXXX_DECLARATION_H(YUV444, ARGB, 10)
YUV444PXToXXXX_DECLARATION_H(YUV444, ABGR, 10)
YUV444PXToXXXX_DECLARATION_H(YUV444, BGRA, 10)
YUV444PXToXXXX_DECLARATION_H(YUV444, BGR, 10)
#endif

YUV444PXToXXXX_DECLARATION_H(YUV444, RGBA, 12)
YUV444PXToXXXX_DECLARATION_H(YUV444, RGB, 12)
#if SPARKYUV_FULL_CHANNELS
YUV444PXToXXXX_DECLARATION_H(YUV444, ARGB, 12)
YUV444PXToXXXX_DECLARATION_H(YUV444, ABGR, 12)
YUV444PXToXXXX_DECLARATION_H(YUV444, BGRA, 12)
YUV444PXToXXXX_DECLARATION_H(YUV444, BGR, 12)
#endif

YUV444PXToXXXX_DECLARATION_H(YUV422, RGBA, 10)
YUV444PXToXXXX_DECLARATION_H(YUV422, RGB, 10)
#if SPARKYUV_FULL_CHANNELS
YUV444PXToXXXX_DECLARATION_H(YUV422, ARGB, 10)
YUV444PXToXXXX_DECLARATION_H(YUV422, ABGR, 10)
YUV444PXToXXXX_DECLARATION_H(YUV422, BGRA, 10)
YUV444PXToXXXX_DECLARATION_H(YUV422, BGR, 10)
#endif

YUV444PXToXXXX_DECLARATION_H(YUV422, RGBA, 12)
YUV444PXToXXXX_DECLARATION_H(YUV422, RGB, 12)
#if SPARKYUV_FULL_CHANNELS
YUV444PXToXXXX_DECLARATION_H(YUV422, ARGB, 12)
YUV444PXToXXXX_DECLARATION_H(YUV422, ABGR, 12)
YUV444PXToXXXX_DECLARATION_H(YUV422, BGRA, 12)
YUV444PXToXXXX_DECLARATION_H(YUV422, BGR, 12)
#endif

YUV444PXToXXXX_DECLARATION_H(YUV420, RGBA, 10)
YUV444PXToXXXX_DECLARATION_H(YUV420, RGB, 10)
#if SPARKYUV_FULL_CHANNELS
YUV444PXToXXXX_DECLARATION_H(YUV420, ARGB, 10)
YUV444PXToXXXX_DECLARATION_H(YUV420, ABGR, 10)
YUV444PXToXXXX_DECLARATION_H(YUV420, BGRA, 10)
YUV444PXToXXXX_DECLARATION_H(YUV420, BGR, 10)
#endif

YUV444PXToXXXX_DECLARATION_H(YUV420, RGBA, 12)
YUV444PXToXXXX_DECLARATION_H(YUV420, RGB, 12)
#if SPARKYUV_FULL_CHANNELS
YUV444PXToXXXX_DECLARATION_H(YUV420, ARGB, 12)
YUV444PXToXXXX_DECLARATION_H(YUV420, ABGR, 12)
YUV444PXToXXXX_DECLARATION_H(YUV420, BGRA, 12)
YUV444PXToXXXX_DECLARATION_H(YUV420, BGR, 12)
#endif

#undef YUV444PXToXXXX_DECLARATION_R

#define PIXEL_TO_YUV_P16_DECLARATION_H(yuvname, bit, pixelType) \
 /** \
 * All RGBA pixels are stored in uint16_t. RGBA10, RGBA12 corresponds a bit depth of the image  \
 */                                               \
        void pixelType##bit##To##yuvname##P##bit(const uint16_t * src, const uint32_t srcStride,\
                                                 const uint32_t width, const uint32_t height,\
                                                 uint16_t * yPlane, const uint32_t yStride,\
                                                 uint16_t * uPlane, const uint32_t uStride,\
                                                 uint16_t * vPlane, const uint32_t vStride,\
                                                 const float kr, const float kb, const SparkYuvColorRange colorRange);

PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 10, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 10, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 10, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 10, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 10, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 10, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 12, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 12, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 12, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 12, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 12, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV444, 12, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 10, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 10, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 10, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 10, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 10, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 10, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 12, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 12, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 12, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 12, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 12, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV422, 12, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 10, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 10, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 10, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 10, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 10, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 10, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 12, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 12, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 12, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 12, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 12, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YUV420, 12, BGR)
#endif

#undef PIXEL_TO_YUV_P16_DECLARATION_H

// MARK: Channels reformat 8 bit

#define CHANNEL_XXX_REFORMAT_DECLARATION_H(srcPixelType, dstPixelType) \
   /** \
   * @brief Reformat from source pixel format to another using uint8_t pixel storage type \
   * If alpha channel will be added it will be set to peak value of 8 bit.
   * \
   */ \
   void srcPixelType##To##dstPixelType(const uint8_t* src, const uint32_t srcStride,\
                                        uint8_t* dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height);

CHANNEL_XXX_REFORMAT_DECLARATION_H(RGBA, RGBA1010102)
CHANNEL_XXX_REFORMAT_DECLARATION_H(RGBA1010102, RGBA)

CHANNEL_XXX_REFORMAT_DECLARATION_H(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_H(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_H(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_H(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_H(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_H(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_H(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_H(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_H(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_H(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_H(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_H(ARGB, BGR)

CHANNEL_XXX_REFORMAT_DECLARATION_H(BGRA, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_H(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_H(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_H(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_H(ARGB, BGRA)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_H

// MARK: Channels reformat 16 bit with dynamic bit depth

#define CHANNEL_XXX_REFORMAT_DECLARATION_1010102_H(srcPixelType, dstPixelType) \
    void srcPixelType##16To##dstPixelType(const uint16_t * src, const uint32_t srcStride,\
                                          uint16_t * dst, const uint32_t dstStride,\
                                          const uint32_t width, const uint32_t height, const int depth);

CHANNEL_XXX_REFORMAT_DECLARATION_1010102_H(RGBA, RGBA1010102)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_1010102_H

#define CHANNEL_XXX_REFORMAT_DECLARATION_16_H(srcPixelType, dstPixelType) \
   /** \
   * @brief Reformat from source pixel format to another using uint16_t pixel storage type \
   * \
   * This function takes two integers as input and returns their sum.\
   * \
   * @param depth Bit depth of the image, will be used to set alpha channel at peak of the current bit depth if alpha \
   * channel will be added \
   */ \
    void srcPixelType##16To##dstPixelType##16(const uint16_t* src, const uint32_t srcStride,\
                                              uint16_t* dst, const uint32_t dstStride,\
                                              const uint32_t width, const uint32_t height, const int depth);

CHANNEL_XXX_REFORMAT_DECLARATION_16_H(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_16_H(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_16_H(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(ARGB, BGR)

CHANNEL_XXX_REFORMAT_DECLARATION_16_H(BGRA, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16_H(ARGB, BGRA)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_16_H

// MARK: Reformat to F16

#define CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_H(srcPixel, dstPixel, storageType) \
    /** \
   * @brief Converts an image to F16 type, channel order is preserved \
   * @param depth Bit depth of the image \
   */ \
    void srcPixel##To##dstPixel##F16(const storageType * src, const uint32_t srcStride,\
                                       uint16_t * dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height);

CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_H(RGBA, RGBA, uint8_t)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_H(RGB, RGB, uint8_t)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_H(RGBA1010102, RGBA, uint8_t)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_H(Channel, Channel, uint8_t)

#undef CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION_H

#define CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_H(pixelType, declareType, storageType) \
    /** \
    * @brief Converts an image to F16 (float16) type, channel order is preserved \
    * @param depth Bit depth of the image \
    */ \
    void pixelType##To##declareType##F16(const storageType * src, const uint32_t srcStride,\
                                       uint16_t * dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height, const int depth);

CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_H(RGBA16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_H(RGB16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_H(Channel16, Channel, uint16_t)

#undef CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION_H

// MARK: Reformat image from F16

#define CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_H(srcPixel, dstPixel, storageType) \
    /** \
    * @brief Converts an image from F16 (float16) type to uint8_t 8 bit, channel order is preserved \
    * @param depth Bit depth of the image \
    */ \
    void srcPixel##F16To##dstPixel(const uint16_t * src, const uint32_t srcStride,\
                                   storageType * dst, const uint32_t dstStride,\
                                   const uint32_t width, const uint32_t height);

CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_H(RGBA, RGBA, uint8_t)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_H(RGB, RGB, uint8_t)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_H(Channel, Channel, uint8_t)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_H(RGBA, RGBA1010102, uint8_t)

#undef CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION_H

#define CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_H(pixelType, declareType, storageType) \
    /** \
    * @brief Converts an image from F16 (float16) type to uint16_t with provided bit depth, channel order is preserved \
    * @param depth Bit depth of the image \
    */ \
    void declareType##F16To##pixelType(const uint16_t * src, const uint32_t srcStride,\
                                       storageType * dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height, const int depth);

CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_H(RGBA16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_H(RGB16, RGBA, uint16_t)
CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_H(Channel16, Channel, uint16_t)

#undef CHANNEL_XXX_REFORMAT_F16_TO_U16_DECLARATION_H

#define PREMULTIPLY_ALPHA_DECLARATION_H(pixelType) \
    /** \
    * @brief Pre multiplies alpha for rendering \
    */ \
    void pixelType##PremultiplyAlpha(const uint8_t * src, const uint32_t srcStride, \
                                        uint8_t * dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height);

PREMULTIPLY_ALPHA_DECLARATION_H(RGBA)
PREMULTIPLY_ALPHA_DECLARATION_H(ARGB)
PREMULTIPLY_ALPHA_DECLARATION_H(ABGR)
PREMULTIPLY_ALPHA_DECLARATION_H(BGRA)

#undef PREMULTIPLY_ALPHA_DECLARATION_H

// Mark scale U8

#define SCALE_CHANNEL_DECLARE_H(channelName) \
      void Scale##channelName(const uint8_t *input, uint32_t srcStride,\
                              uint32_t inputWidth, uint32_t inputHeight,\
                              uint8_t *output, uint32_t dstStride,\
                              uint32_t outputWidth, uint32_t outputHeight,\
                              SparkYuvSampler option);

SCALE_CHANNEL_DECLARE_H(Channel)
SCALE_CHANNEL_DECLARE_H(RGB)
SCALE_CHANNEL_DECLARE_H(RGBA)

#undef SCALE_CHANNEL_DECLARE_H

// Mark scale F16

#define SCALE_CHANNEL_F16_DECLARE_H(channelName) \
      void Scale##channelName(const uint16_t *input, uint32_t srcStride,\
                              uint32_t inputWidth, uint32_t inputHeight,\
                              uint16_t *output, uint32_t dstStride,\
                              uint32_t outputWidth, uint32_t outputHeight,\
                              SparkYuvSampler option);

SCALE_CHANNEL_F16_DECLARE_H(ChannelF16)
SCALE_CHANNEL_F16_DECLARE_H(RGBF16)
SCALE_CHANNEL_F16_DECLARE_H(RGBAF16)

#undef SCALE_CHANNEL_F16_DECLARE_H

// Mark: Scale RGBA1010102

void ScaleRGBA1010102(const uint8_t *input, uint32_t srcStride,
                      uint32_t inputWidth, uint32_t inputHeight,
                      uint8_t *output, uint32_t dstStride,
                      uint32_t outputWidth, uint32_t outputHeight,
                      SparkYuvSampler option);

#define SCALE_CHANNEL_16_TYPE_H(channelName) \
      void Scale##channelName(const uint16_t *input, const uint32_t srcStride,\
                              uint32_t inputWidth, uint32_t inputHeight,\
                              uint16_t *output, const uint32_t dstStride,\
                              uint32_t outputWidth, uint32_t outputHeight,    \
                              const int depth, const SparkYuvSampler option);

SCALE_CHANNEL_16_TYPE_H(Channel16)
SCALE_CHANNEL_16_TYPE_H(RGB16)
SCALE_CHANNEL_16_TYPE_H(RGBA16)

#undef SCALE_CHANNEL_16_TYPE_H

}