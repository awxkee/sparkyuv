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
#include "sparkyuv-basic.h"
#include "sparkyuv-rgb565.h"
#include "sparkyuv-eotf.h"
#include "sparkyuv-yccbccrc.h"
#include "sparkyuv-ycgco.h"
#include "sparkyuv-ycgcor.h"
#include "sparkyuv-ydzdx.h"
#include "sparkyuv-yiq.h"
#include "sparkyuv-ydbdr.h"
#include "sparkyuv-ycbcr.h"

namespace sparkyuv {

void YCbCr420ToRGBA(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);

void YCbCr420ToRGB(uint8_t *dst, uint32_t rgbaStride,
                   uint32_t width, uint32_t height,
                   const uint8_t *ySrc, uint32_t yPlaneStride,
                   const uint8_t *uSrc, uint32_t uPlaneStride,
                   const uint8_t *vSrc, uint32_t vPlaneStride,
                   float kr, float kb, SparkYuvColorRange colorRange);

#if SPARKYUV_FULL_CHANNELS
void YCbCr420ToABGR(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr420ToARGB(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr420ToBGRA(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr420ToBGR(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr422ToRGBA(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);

void YCbCr422ToRGB(uint8_t *dst, uint32_t rgbaStride,
                   uint32_t width, uint32_t height,
                   const uint8_t *ySrc, uint32_t yPlaneStride,
                   const uint8_t *uSrc, uint32_t uPlaneStride,
                   const uint8_t *vSrc, uint32_t vPlaneStride,
                   float kr, float kb, SparkYuvColorRange colorRange);

#if SPARKYUV_FULL_CHANNELS
void YCbCr422ToABGR(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr422ToARGB(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr422ToBGRA(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr422ToBGR(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr444ToRGBA(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);

void YCbCr444ToRGB(uint8_t *dst, uint32_t rgbaStride,
                   uint32_t width, uint32_t height,
                   const uint8_t *ySrc, uint32_t yPlaneStride,
                   const uint8_t *uSrc, uint32_t uPlaneStride,
                   const uint8_t *vSrc, uint32_t vPlaneStride,
                   float kr, float kb, SparkYuvColorRange colorRange);

#if SPARKYUV_FULL_CHANNELS
void YCbCr444ToABGR(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444ToARGB(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444ToBGRA(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444ToBGR(uint8_t *dst, uint32_t rgbaStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *ySrc, uint32_t yPlaneStride,
                    const uint8_t *uSrc, uint32_t uPlaneStride,
                    const uint8_t *vSrc, uint32_t vPlaneStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBAToYCbCr420(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void RGBToYCbCr420(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ABGRToYCbCr420(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void ARGBToYCbCr420(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
void BGRAToYCbCr420(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void BGRToYCbCr420(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBAToYCbCr422(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void RGBToYCbCr422(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ABGRToYCbCr422(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void ARGBToYCbCr422(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
void BGRAToYCbCr422(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void BGRToYCbCr422(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBAToYCbCr444(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void RGBToYCbCr444(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ABGRToYCbCr444(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void ARGBToYCbCr444(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
void BGRAToYCbCr444(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void BGRToYCbCr444(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

/**
 * High bit depth YCbCr
 */

void YCbCr444P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr444P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P10ToABGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr444P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr444P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P12ToABGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr444P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

#define YCbCr420ToGEN(pixelType, name, kr, kb, range) \
    static void YCbCr420##name##To##pixelType(uint8_t * dst, \
                                            uint32_t rgbaStride, uint32_t width, uint32_t height,\
                                            const uint8_t * ySrc, uint32_t yPlaneStride,\
                                            const uint8_t * uSrc, uint32_t uPlaneStride,\
                                            const uint8_t * vSrc, uint32_t vPlaneStride) {\
         YCbCr420To##pixelType(dst, rgbaStride, width, height,\
                             ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                             kr, kb, range);\
    }

// MARK: YCbCr420 BT.601

#define YCbCr420ToGEN601(pixelType) YCbCr420ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

YCbCr420ToGEN601(RGBA)
YCbCr420ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr420ToGEN601(ARGB)
YCbCr420ToGEN601(ABGR)
YCbCr420ToGEN601(BGRA)
YCbCr420ToGEN601(BGR)
#endif

#undef YCbCr420ToGEN601

// MARK: YCbCr422 BT.709

#define YCbCr420ToGEN709(pixelType) YCbCr420ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

YCbCr420ToGEN709(RGBA)
YCbCr420ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr420ToGEN709(ARGB)
YCbCr420ToGEN709(ABGR)
YCbCr420ToGEN709(BGRA)
YCbCr420ToGEN709(BGR)
#endif

#undef YCbCr420ToGEN709

// MARK: YCbCr420 BT.2020

#define YCbCr420ToGEN2020(pixelType) YCbCr420ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

YCbCr420ToGEN2020(RGBA)
YCbCr420ToGEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr420ToGEN2020(ARGB)
YCbCr420ToGEN2020(ABGR)
YCbCr420ToGEN2020(BGRA)
YCbCr420ToGEN2020(BGR)
#endif

#undef YCbCr420ToGEN2020
#undef YCbCr420ToGEN

#define YCbCr422ToGEN(pixelType, name, kr, kb, range) \
    static void YCbCr422##name##To##pixelType(uint8_t * dst, \
                                            uint32_t rgbaStride, uint32_t width, uint32_t height,\
                                            const uint8_t * ySrc, uint32_t yPlaneStride,\
                                            const uint8_t * uSrc, uint32_t uPlaneStride,\
                                            const uint8_t * vSrc, uint32_t vPlaneStride) {  \
         YCbCr422To##pixelType(dst, rgbaStride, width, height,\
                             ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                             kr, kb, range); \
    }

// MARK: YCbCr422 BT.601

#define YCbCr422ToGEN601(pixelType) YCbCr422ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

YCbCr422ToGEN601(RGBA)
YCbCr422ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr422ToGEN601(ARGB)
YCbCr422ToGEN601(ABGR)
YCbCr422ToGEN601(BGRA)
YCbCr422ToGEN601(BGR)
#endif

#undef YCbCr422ToGEN601

// MARK: YCbCr422 BT.709

#define YCbCr422ToGEN709(pixelType) YCbCr422ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

YCbCr422ToGEN709(RGBA)
YCbCr422ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr422ToGEN709(ARGB)
YCbCr422ToGEN709(ABGR)
YCbCr422ToGEN709(BGRA)
YCbCr422ToGEN709(BGR)
#endif

#undef YCbCr422ToGEN709

// MARK: YCbCr422 BT.2020

#define YCbCr422ToGEN2020(pixelType) YCbCr422ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

YCbCr422ToGEN2020(RGBA)
YCbCr422ToGEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr422ToGEN2020(ARGB)
YCbCr422ToGEN2020(ABGR)
YCbCr422ToGEN2020(BGRA)
YCbCr422ToGEN2020(BGR)
#endif

#undef YCbCr422ToGEN2020
#undef YCbCr422ToGEN

//MARK: YCbCr444 To RGBX

#define YCbCr444ToGEN(pixelType, name, kr, kb, range) \
    static void YCbCr444##name##To##pixelType(uint8_t* dst, \
                                            uint32_t rgbaStride, uint32_t width, uint32_t height,\
                                            const uint8_t * ySrc, uint32_t yPlaneStride,\
                                            const uint8_t * uSrc, uint32_t uPlaneStride,\
                                            const uint8_t * vSrc, uint32_t vPlaneStride) {  \
         YCbCr444To##pixelType(dst, rgbaStride, width, height,\
                             ySrc, yPlaneStride, uSrc, uPlaneStride, vSrc, vPlaneStride,\
                             kr, kb, range); \
    }

// MARK: YCbCr444 BT.601

#define YCbCr444ToGEN601(pixelType) YCbCr444ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

YCbCr444ToGEN601(RGBA)
YCbCr444ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr444ToGEN601(ARGB)
YCbCr444ToGEN601(ABGR)
YCbCr444ToGEN601(BGRA)
YCbCr444ToGEN601(BGR)
#endif

#undef YCbCr444ToGEN601

// MARK: YCbCr444 BT.709

#define YCbCr444ToGEN709(pixelType) YCbCr444ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

YCbCr444ToGEN709(RGBA)
YCbCr444ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr444ToGEN709(ARGB)
YCbCr444ToGEN709(ABGR)
YCbCr444ToGEN709(BGRA)
YCbCr444ToGEN709(BGR)
#endif

#undef YCbCr444ToGEN709

// MARK: YCbCr444 BT.2020

#define YCbCr444ToGEN2020(pixelType) YCbCr444ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

YCbCr444ToGEN2020(RGBA)
YCbCr444ToGEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr444ToGEN2020(ARGB)
YCbCr444ToGEN2020(ABGR)
YCbCr444ToGEN2020(BGRA)
YCbCr444ToGEN2020(BGR)
#endif

#undef YCbCr444ToGEN2020
#undef YCbCr444ToGEN

#define XXXXToYCbCr444GEN(pixelType, name, kr, kb, range) \
    static void pixelType##ToYCbCr444##name(const uint8_t * src, uint32_t srcStride,\
                                          uint32_t width, uint32_t height,\
                                          uint8_t * yPlane, uint32_t yStride,\
                                          uint8_t * uPlane, uint32_t uStride,\
                                          uint8_t * vPlane, uint32_t vStride) {\
          pixelType##ToYCbCr444(src, srcStride, width, height,                                  \
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,              \
                              kr, kb, range); \
    }

#define XXXToYCbCr444GEN601(pixelType) XXXXToYCbCr444GEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

XXXToYCbCr444GEN601(RGBA)
XXXToYCbCr444GEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXToYCbCr444GEN601(ARGB)
XXXToYCbCr444GEN601(ABGR)
XXXToYCbCr444GEN601(BGRA)
XXXToYCbCr444GEN601(BGR)
#endif

#undef XXXToYCbCr444GEN601

#define XXXToYCbCr444GEN709(pixelType) XXXXToYCbCr444GEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

XXXToYCbCr444GEN709(RGBA)
XXXToYCbCr444GEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXToYCbCr444GEN709(ARGB)
XXXToYCbCr444GEN709(ABGR)
XXXToYCbCr444GEN709(BGRA)
XXXToYCbCr444GEN709(BGR)
#endif

#undef XXXToYCbCr444GEN709

#define XXXToYCbCr444GEN2020(pixelType) XXXXToYCbCr444GEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

XXXToYCbCr444GEN2020(RGBA)
XXXToYCbCr444GEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXToYCbCr444GEN2020(ARGB)
XXXToYCbCr444GEN2020(ABGR)
XXXToYCbCr444GEN2020(BGRA)
XXXToYCbCr444GEN2020(BGR)
#endif

#undef XXXToYCbCr444GEN709
#undef XXXXToYCbCr444GEN

// MARK: RGB to YCbCr422

#define XXXXToYCbCr422GEN(pixelType, name, kr, kb, range) \
    static void pixelType##ToYCbCr422##name(const uint8_t * src, uint32_t srcStride,\
                                          uint32_t width, uint32_t height,\
                                          uint8_t * yPlane, uint32_t yStride,\
                                          uint8_t * uPlane, uint32_t uStride,\
                                          uint8_t * vPlane, uint32_t vStride) {\
          pixelType##ToYCbCr422(src, srcStride, width, height,                                  \
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,              \
                              kr, kb, range); \
    }

#define XXXXToYCbCr422GEN601(pixelType) XXXXToYCbCr422GEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

XXXXToYCbCr422GEN601(RGBA)
XXXXToYCbCr422GEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr422GEN601(ARGB)
XXXXToYCbCr422GEN601(ABGR)
XXXXToYCbCr422GEN601(BGRA)
XXXXToYCbCr422GEN601(BGR)
#endif

#undef XXXXToYCbCr422GEN601

#define XXXXToYCbCr422GEN709(pixelType) XXXXToYCbCr422GEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

XXXXToYCbCr422GEN709(RGBA)
XXXXToYCbCr422GEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr422GEN709(ARGB)
XXXXToYCbCr422GEN709(ABGR)
XXXXToYCbCr422GEN709(BGRA)
XXXXToYCbCr422GEN709(BGR)
#endif

#undef XXXXToYCbCr422GEN709

#define XXXXToYCbCr422GEN2020(pixelType) XXXXToYCbCr422GEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

XXXXToYCbCr422GEN2020(RGBA)
XXXXToYCbCr422GEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr422GEN2020(ARGB)
XXXXToYCbCr422GEN2020(ABGR)
XXXXToYCbCr422GEN2020(BGRA)
XXXXToYCbCr422GEN2020(BGR)
#endif

#undef XXXXToYCbCr422GEN2020
#undef XXXXToYCbCr422GEN

// MARK: RGB to YCbCr420

#define XXXXToYCbCr420GEN(pixelType, name, kr, kb, range) \
    static void pixelType##ToYCbCr420##name(const uint8_t * src, uint32_t srcStride,\
                                          uint32_t width, uint32_t height,\
                                          uint8_t * yPlane, uint32_t yStride,\
                                          uint8_t * uPlane, uint32_t uStride,\
                                          uint8_t * vPlane, uint32_t vStride) {\
          pixelType##ToYCbCr420(src, srcStride, width, height,                                  \
                              yPlane, yStride, uPlane, uStride, vPlane, vStride,              \
                              kr, kb, range); \
    }

// MARK: YCbCr420 BT.601

#define XXXXToYCbCr420GEN601(pixelType) XXXXToYCbCr420GEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

XXXXToYCbCr420GEN601(RGBA)
XXXXToYCbCr420GEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr420GEN601(ARGB)
XXXXToYCbCr420GEN601(ABGR)
XXXXToYCbCr420GEN601(BGRA)
XXXXToYCbCr420GEN601(BGR)
#endif

#undef XXXXToYCbCr420GEN601

// MARK: YCbCr420 BT.709

#define XXXXToYCbCr420GEN709(pixelType) XXXXToYCbCr420GEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

XXXXToYCbCr420GEN709(RGBA)
XXXXToYCbCr420GEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr420GEN709(ARGB)
XXXXToYCbCr420GEN709(ABGR)
XXXXToYCbCr420GEN709(BGRA)
XXXXToYCbCr420GEN709(BGR)
#endif

#undef XXXXToYCbCr420GEN709

// MARK: YCbCr420 BT.2020

#define XXXXToYCbCr420GEN2020(pixelType) XXXXToYCbCr420GEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

XXXXToYCbCr420GEN2020(RGBA)
XXXXToYCbCr420GEN2020(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr420GEN2020(ARGB)
XXXXToYCbCr420GEN2020(ABGR)
XXXXToYCbCr420GEN2020(BGRA)
XXXXToYCbCr420GEN2020(BGR)
#endif

#undef XXXXToYCbCr420GEN2020
#undef XXXXToYCbCr420GEN

#define YCbCr444PXToXXXX_DECLARATION_H(yuvname, pixelType, bit) \
    void yuvname##P##bit##To##pixelType##bit(uint16_t * src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t * yPlane, const uint32_t yStride,\
                                             const uint16_t * uPlane, const uint32_t uStride,\
                                             const uint16_t * vPlane, const uint32_t vStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange);

YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, RGBA, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, RGB, 10)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, ARGB, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, ABGR, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, BGRA, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, BGR, 10)
#endif

YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, RGBA, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, RGB, 12)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, ARGB, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, ABGR, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, BGRA, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr422, BGR, 12)
#endif

YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, RGBA, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, RGB, 10)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, ARGB, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, ABGR, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, BGRA, 10)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, BGR, 10)
#endif

YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, RGBA, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, RGB, 12)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, ARGB, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, ABGR, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, BGRA, 12)
YCbCr444PXToXXXX_DECLARATION_H(YCbCr420, BGR, 12)
#endif

#undef YCbCr444PXToXXXX_DECLARATION_R

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

PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 10, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 10, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 10, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 10, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 10, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 10, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 12, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 12, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 12, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 12, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 12, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr444, 12, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 10, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 10, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 10, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 10, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 10, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 10, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 12, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 12, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 12, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 12, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 12, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr422, 12, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 10, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 10, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 10, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 10, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 10, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 10, BGR)
#endif

PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 12, RGBA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 12, RGB)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 12, BGRA)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 12, ABGR)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 12, ARGB)
PIXEL_TO_YUV_P16_DECLARATION_H(YCbCr420, 12, BGR)
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

}