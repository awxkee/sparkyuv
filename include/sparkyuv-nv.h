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

namespace sparkyuv {
// MARK: NV21 Declarations

void NV21ToRGBA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);

void NV21ToRGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);

#if SPARKYUV_FULL_CHANNELS
void NV21ToABGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV21ToARGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV21ToBGRA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV21ToBGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#endif

#define NV21ToGEN(pixelType, name, kr, kb, range) \
    static void NV21##name##To##pixelType(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, \
                                   const uint8_t *ySrc, uint32_t yStride, \
                                   const uint8_t *uv, uint32_t uvStride) {  \
         NV21To##pixelType(src, dstStride, width, height, ySrc, yStride, \
                           uv, uvStride, kr, kb, range); \
    }

// MARK: NV21 BT.601

#define NV21ToGEN601(pixelType) NV21ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

NV21ToGEN601(RGBA)
NV21ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
NV21ToGEN601(BGRA)
NV21ToGEN601(ARGB)
NV21ToGEN601(ABGR)
NV21ToGEN601(BGR)
#endif

#undef NV21ToGEN601

// MARK: NV21 BT.709

#define NV21ToGEN709(pixelType) NV21ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

NV21ToGEN709(RGBA)
NV21ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
NV21ToGEN709(BGRA)
NV21ToGEN709(ARGB)
NV21ToGEN709(ABGR)
NV21ToGEN709(BGR)
#endif

#undef NV21ToGEN709

// MARK: NV21 BT.2020 NCL

#define NV21ToGEN2020NCL(pixelType) NV21ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

NV21ToGEN2020NCL(RGBA)
NV21ToGEN2020NCL(RGB)
#if SPARKYUV_FULL_CHANNELS
NV21ToGEN2020NCL(BGRA)
NV21ToGEN2020NCL(ARGB)
NV21ToGEN2020NCL(ABGR)
NV21ToGEN2020NCL(BGR)
#endif

#undef NV21ToGEN2020NCL

#undef NV21ToGEN

// MARK: NV12 Declarations

void NV12ToRGBA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV12ToRGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void NV12ToABGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV12ToARGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV12ToBGRA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV12ToBGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#endif

#define NV12ToGEN(pixelType, name, kr, kb, range) \
    static void NV12##name##To##pixelType(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, \
                                          const uint8_t *ySrc, uint32_t yStride, \
                                          const uint8_t *uv, uint32_t uvStride) {  \
         NV12To##pixelType(src, dstStride, width, height, ySrc, yStride, \
                           uv, uvStride, kr, kb, range); \
    }

// MARK: NV12 BT.601

#define NV12ToGEN601(pixelType) NV12ToGEN(pixelType, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

NV12ToGEN601(RGBA)
NV12ToGEN601(RGB)
#if SPARKYUV_FULL_CHANNELS
NV12ToGEN601(ARGB)
NV12ToGEN601(ABGR)
NV12ToGEN601(BGRA)
NV12ToGEN601(BGR)
#endif

#undef NV12ToGEN601

// MARK: NV12 BT.709

#define NV12ToGEN709(pixelType) NV12ToGEN(pixelType, BT709, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV)

NV12ToGEN709(RGBA)
NV12ToGEN709(RGB)
#if SPARKYUV_FULL_CHANNELS
NV12ToGEN709(ARGB)
NV12ToGEN709(ABGR)
NV12ToGEN709(BGRA)
NV12ToGEN709(BGR)
#endif

#undef NV12ToGEN709

// MARK: NV12 BT.2020 NCL

#define NV12ToGEN2020NCL(pixelType) NV12ToGEN(pixelType, BT2020, 0.2627f, 0.0593f, sparkyuv::YUV_RANGE_TV)

NV12ToGEN2020NCL(RGBA)
NV12ToGEN2020NCL(RGB)
#if SPARKYUV_FULL_CHANNELS
NV12ToGEN2020NCL(ARGB)
NV12ToGEN2020NCL(ABGR)
NV12ToGEN2020NCL(BGRA)
NV12ToGEN2020NCL(BGR)
#endif

#undef NV12ToGEN2020NCL
#undef NV12ToGEN

#define XXXXToNVXX_DECLARATION_H(Pixel, NV) \
    void Pixel##To##NV(const uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, uint8_t *ySrc, uint32_t yStride, \
                  uint8_t *uv, uint32_t uvStride, const float kr, const float kb, const SparkYuvColorRange colorRange);

XXXXToNVXX_DECLARATION_H(RGBA, NV21)
XXXXToNVXX_DECLARATION_H(RGB, NV21)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_H(ARGB, NV21)
XXXXToNVXX_DECLARATION_H(ABGR, NV21)
XXXXToNVXX_DECLARATION_H(BGRA, NV21)
XXXXToNVXX_DECLARATION_H(BGR, NV21)
#endif

XXXXToNVXX_DECLARATION_H(RGBA, NV12)
XXXXToNVXX_DECLARATION_H(RGB, NV12)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_H(ARGB, NV12)
XXXXToNVXX_DECLARATION_H(ABGR, NV12)
XXXXToNVXX_DECLARATION_H(BGRA, NV12)
XXXXToNVXX_DECLARATION_H(BGR, NV12)
#endif

XXXXToNVXX_DECLARATION_H(RGBA, NV16)
XXXXToNVXX_DECLARATION_H(RGB, NV16)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_H(ARGB, NV16)
XXXXToNVXX_DECLARATION_H(ABGR, NV16)
XXXXToNVXX_DECLARATION_H(BGRA, NV16)
XXXXToNVXX_DECLARATION_H(BGR, NV16)
#endif

XXXXToNVXX_DECLARATION_H(RGBA, NV61)
XXXXToNVXX_DECLARATION_H(RGB, NV61)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_H(ARGB, NV61)
XXXXToNVXX_DECLARATION_H(ABGR, NV61)
XXXXToNVXX_DECLARATION_H(BGRA, NV61)
XXXXToNVXX_DECLARATION_H(BGR, NV61)
#endif

XXXXToNVXX_DECLARATION_H(RGBA, NV24)
XXXXToNVXX_DECLARATION_H(RGB, NV24)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_H(ARGB, NV24)
XXXXToNVXX_DECLARATION_H(ABGR, NV24)
XXXXToNVXX_DECLARATION_H(BGRA, NV24)
XXXXToNVXX_DECLARATION_H(BGR, NV24)
#endif

XXXXToNVXX_DECLARATION_H(RGBA, NV42)
XXXXToNVXX_DECLARATION_H(RGB, NV42)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_H(ARGB, NV42)
XXXXToNVXX_DECLARATION_H(ABGR, NV42)
XXXXToNVXX_DECLARATION_H(BGRA, NV42)
XXXXToNVXX_DECLARATION_H(BGR, NV42)
#endif

#undef XXXXToNVXX_DECLARATION_H

void NV16ToRGBA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV16ToRGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void NV16ToABGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV16ToARGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV16ToBGRA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV16ToBGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#endif

void NV61ToRGBA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV61ToRGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void NV61ToABGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV61ToARGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV61ToBGRA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV61ToBGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#endif

void NV24ToRGBA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV24ToRGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void NV24ToABGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV24ToARGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV24ToBGRA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV24ToBGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#endif

void NV42ToRGBA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV42ToRGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void NV42ToABGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV42ToARGB(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV42ToBGRA(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
                const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
void NV42ToBGR(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride,
               const uint8_t *uv, uint32_t uvStride, float kr, float kb, SparkYuvColorRange colorRange);
#endif

#define XXXXToNVXXNAME_DECLARATION_H(Pixel, NV, Name, kr, kb, range) \
     static void Pixel##To##NV##Name(const uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, \
                              uint8_t *ySrc, uint32_t yStride, uint8_t *uv, uint32_t uvStride) {\
          Pixel##To##NV(src, dstStride, width, height, ySrc, yStride, \
                        uv, uvStride, kr, kb, range); \
     }

#define XXXXToNVXXBT601NAME_DECLARATION_H(Pixel, NV) XXXXToNVXXNAME_DECLARATION_H(Pixel, NV, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

XXXXToNVXXBT601NAME_DECLARATION_H(RGBA, NV21)
XXXXToNVXXBT601NAME_DECLARATION_H(RGB, NV21)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXXBT601NAME_DECLARATION_H(ARGB, NV21)
XXXXToNVXXBT601NAME_DECLARATION_H(ABGR, NV21)
XXXXToNVXXBT601NAME_DECLARATION_H(BGRA, NV21)
XXXXToNVXXBT601NAME_DECLARATION_H(BGR, NV21)
#endif

XXXXToNVXXBT601NAME_DECLARATION_H(RGBA, NV12)
XXXXToNVXXBT601NAME_DECLARATION_H(RGB, NV12)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXXBT601NAME_DECLARATION_H(ARGB, NV12)
XXXXToNVXXBT601NAME_DECLARATION_H(ABGR, NV12)
XXXXToNVXXBT601NAME_DECLARATION_H(BGRA, NV12)
XXXXToNVXXBT601NAME_DECLARATION_H(BGR, NV12)
#endif

XXXXToNVXXBT601NAME_DECLARATION_H(RGBA, NV16)
XXXXToNVXXBT601NAME_DECLARATION_H(RGB, NV16)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXXBT601NAME_DECLARATION_H(ARGB, NV16)
XXXXToNVXXBT601NAME_DECLARATION_H(ABGR, NV16)
XXXXToNVXXBT601NAME_DECLARATION_H(BGRA, NV16)
XXXXToNVXXBT601NAME_DECLARATION_H(BGR, NV16)
#endif

XXXXToNVXXBT601NAME_DECLARATION_H(RGBA, NV61)
XXXXToNVXXBT601NAME_DECLARATION_H(RGB, NV61)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXXBT601NAME_DECLARATION_H(ARGB, NV61)
XXXXToNVXXBT601NAME_DECLARATION_H(ABGR, NV61)
XXXXToNVXXBT601NAME_DECLARATION_H(BGRA, NV61)
XXXXToNVXXBT601NAME_DECLARATION_H(BGR, NV61)
#endif

XXXXToNVXXBT601NAME_DECLARATION_H(RGBA, NV24)
XXXXToNVXXBT601NAME_DECLARATION_H(RGB, NV24)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXXBT601NAME_DECLARATION_H(ARGB, NV24)
XXXXToNVXXBT601NAME_DECLARATION_H(ABGR, NV24)
XXXXToNVXXBT601NAME_DECLARATION_H(BGRA, NV24)
XXXXToNVXXBT601NAME_DECLARATION_H(BGR, NV24)
#endif

XXXXToNVXXBT601NAME_DECLARATION_H(RGBA, NV42)
XXXXToNVXXBT601NAME_DECLARATION_H(RGB, NV42)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXXBT601NAME_DECLARATION_H(ARGB, NV42)
XXXXToNVXXBT601NAME_DECLARATION_H(ABGR, NV42)
XXXXToNVXXBT601NAME_DECLARATION_H(BGRA, NV42)
XXXXToNVXXBT601NAME_DECLARATION_H(BGR, NV42)
#endif

#undef XXXXToNVXXNAME_DECLARATION_H
#undef XXXXToNVXXBT601NAME_DECLARATION_H

#define NVXXToXXXXNAME_DECLARATION_H(NVType, Pixel, Name, kr, kb, range) \
     static void NVType##Name##To##Pixel(uint8_t *src, uint32_t dstStride, uint32_t width, uint32_t height, \
                                     const uint8_t *ySrc, uint32_t yStride, const uint8_t *uv, uint32_t uvStride) { \
              NVType##To##Pixel(src, dstStride, width, height, ySrc, yStride, \
                            uv, uvStride, kr, kb, range); \
      }

#define NVXXToXXXXBT601NAME_DECLARATION_H(NV, Pixel) NVXXToXXXXNAME_DECLARATION_H(NV, Pixel, BT601, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV)

NVXXToXXXXBT601NAME_DECLARATION_H(NV16, RGBA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV16, RGB)
#if SPARKYUV_FULL_CHANNELS
NVXXToXXXXBT601NAME_DECLARATION_H(NV16, ARGB)
NVXXToXXXXBT601NAME_DECLARATION_H(NV16, ABGR)
NVXXToXXXXBT601NAME_DECLARATION_H(NV16, BGRA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV16, BGR)
#endif

NVXXToXXXXBT601NAME_DECLARATION_H(NV61, RGBA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV61, RGB)
#if SPARKYUV_FULL_CHANNELS
NVXXToXXXXBT601NAME_DECLARATION_H(NV61, ARGB)
NVXXToXXXXBT601NAME_DECLARATION_H(NV61, ABGR)
NVXXToXXXXBT601NAME_DECLARATION_H(NV61, BGRA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV61, BGR)
#endif

NVXXToXXXXBT601NAME_DECLARATION_H(NV24, RGBA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV24, RGB)
#if SPARKYUV_FULL_CHANNELS
NVXXToXXXXBT601NAME_DECLARATION_H(NV24, ARGB)
NVXXToXXXXBT601NAME_DECLARATION_H(NV24, ABGR)
NVXXToXXXXBT601NAME_DECLARATION_H(NV24, BGRA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV24, BGR)
#endif

NVXXToXXXXBT601NAME_DECLARATION_H(NV42, RGBA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV42, RGB)
#if SPARKYUV_FULL_CHANNELS
NVXXToXXXXBT601NAME_DECLARATION_H(NV42, ARGB)
NVXXToXXXXBT601NAME_DECLARATION_H(NV42, ABGR)
NVXXToXXXXBT601NAME_DECLARATION_H(NV42, BGRA)
NVXXToXXXXBT601NAME_DECLARATION_H(NV42, BGR)
#endif

#undef NVXXToXXXXNAME_DECLARATION_H

}