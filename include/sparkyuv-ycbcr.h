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

void YCbCr400ToRGBA(uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *yPlane, uint32_t yStride,
                    float kr, float kb, SparkYuvColorRange colorRange);

void YCbCr400ToRGB(const uint8_t *src,
                   uint32_t srcStride,
                   uint32_t width,
                   uint32_t height,
                   const uint8_t *yPlane,
                   uint32_t yStride,
                   float kr,
                   float kb,
                   SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr400ToARGB(uint8_t * src, uint32_t srcStride,uint32_t width, uint32_t height,
                    uint8_t * yPlane, uint32_t yStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400ToABGR(uint8_t * src, uint32_t srcStride, uint32_t width, uint32_t height,
                    const uint8_t * yPlane,  uint32_t yStride,
                    float kr,float kb,SparkYuvColorRange colorRange);
void YCbCr400ToBGRA(uint8_t * src, uint32_t srcStride, uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400ToBGR(uint8_t * src, uint32_t srcStride, uint32_t width, uint32_t height,
                   const uint8_t * yPlane, uint32_t yStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBAToYCbCr400(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void RGBToYCbCr400(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ARGBToYCbCr400(const uint8_t * src,  uint32_t srcStride,
                    uint32_t width,  uint32_t height,
                    uint8_t * yPlane,  uint32_t yStride,
                    float kr,  float kb, SparkYuvColorRange colorRange);
void ABGRToYCbCr400(const uint8_t * src,  uint32_t srcStride,
                    uint32_t width,  uint32_t height,
                    uint8_t * yPlane,  uint32_t yStride,
                    float kr,  float kb, SparkYuvColorRange colorRange);
void BGRAToYCbCr400(const uint8_t * src,  uint32_t srcStride,
                    uint32_t width,  uint32_t height,
                    uint8_t * yPlane,  uint32_t yStride,
                    float kr,  float kb, SparkYuvColorRange colorRange);
void BGRToYCbCr400(const uint8_t * src,  uint32_t srcStride,
                    uint32_t width,  uint32_t height,
                    uint8_t * yPlane,  uint32_t yStride,
                    float kr,  float kb, SparkYuvColorRange colorRange);
#endif

void RGBA10ToYCbCr400P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height, uint16_t *yPlane, uint32_t yStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void RGB10ToYCbCr400P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ABGR10ToYCbCr400P10bit(const uint16_t * src, uint32_t srcStride,
                           uint32_t width, uint32_t height,
                           uint16_t * yPlane, uint32_t yStride,
                           float kr, float kb, SparkYuvColorRange colorRange);
void ARGB10ToYCbCr400P10bit(const uint16_t * src, uint32_t srcStride,
                            uint32_t width, uint32_t height,
                            uint16_t * yPlane, uint32_t yStride,
                            float kr, float kb, SparkYuvColorRange colorRange);
void BGRA10ToYCbCr400P10bit(const uint16_t * src, uint32_t srcStride,
                            uint32_t width, uint32_t height,
                            uint16_t * yPlane, uint32_t yStride,
                            float kr, float kb, SparkYuvColorRange colorRange);
void BGR10ToYCbCr400P10bit(const uint16_t * src, uint32_t srcStride,
                            uint32_t width, uint32_t height,
                            uint16_t * yPlane, uint32_t yStride,
                            float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBA12ToYCbCr400P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height, uint16_t *yPlane, uint32_t yStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void RGB12ToYCbCr400P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ABGR12ToYCbCr400P12bit(const uint16_t * src, uint32_t srcStride,
                           uint32_t width, uint32_t height,
                           uint16_t * yPlane, uint32_t yStride,
                           float kr, float kb, SparkYuvColorRange colorRange);
void ARGB12ToYCbCr400P12bit(const uint16_t * src, uint32_t srcStride,
                            uint32_t width, uint32_t height,
                            uint16_t * yPlane, uint32_t yStride,
                            float kr, float kb, SparkYuvColorRange colorRange);
void BGRA12ToYCbCr400P12bit(const uint16_t * src, uint32_t srcStride,
                            uint32_t width, uint32_t height,
                            uint16_t * yPlane, uint32_t yStride,
                            float kr, float kb, SparkYuvColorRange colorRange);
void BGR12ToYCbCr400P12bit(const uint16_t * src, uint32_t srcStride,
                            uint32_t width, uint32_t height,
                            uint16_t * yPlane, uint32_t yStride,
                            float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr400P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr400P10ToARGB10(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P10ToABGR10(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P10ToBGRA10(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P10ToBGR10(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr400P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr400P12ToARGB12(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P12ToABGR12(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P12ToBGRA12(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr400P12ToBGR12(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBAToYCbCr411(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void RGBToYCbCr411(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);

#if SPARKYUV_FULL_CHANNELS
void ARGBToYCbCr411(const uint8_t * src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t * yPlane, uint32_t yStride,
                   uint8_t * uPlane, uint32_t uStride,
                   uint8_t * vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
void ABGRToYCbCr411(const uint8_t * src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t * yPlane, uint32_t yStride,
                   uint8_t * uPlane, uint32_t uStride,
                   uint8_t * vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
void BGRAToYCbCr411(const uint8_t * src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t * yPlane, uint32_t yStride,
                   uint8_t * uPlane, uint32_t uStride,
                   uint8_t * vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
void BGRToYCbCr411(const uint8_t * src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t * yPlane, uint32_t yStride,
                   uint8_t * uPlane, uint32_t uStride,
                   uint8_t * vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr411ToRGBA(uint8_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *yPlane, uint32_t yStride,
                    const uint8_t *uPlane, uint32_t uStride,
                    const uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411ToRGB(uint8_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height,
                   const uint8_t *yPlane, uint32_t yStride,
                   const uint8_t *uPlane, uint32_t uStride,
                   const uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);

#if SPARKYUV_FULL_CHANNELS
void YCbCr411ToARGB(uint8_t * dst, uint32_t dstStride,
                   uint32_t width, uint32_t height,
                   const uint8_t * yPlane, uint32_t yStride,
                   const uint8_t * uPlane, uint32_t uStride,
                   const uint8_t * vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411ToABGR(uint8_t * dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    const uint8_t * uPlane, uint32_t uStride,
                    const uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411ToBGRA(uint8_t * dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    const uint8_t * uPlane, uint32_t uStride,
                    const uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411ToBGR(uint8_t * dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    const uint8_t * uPlane, uint32_t uStride,
                    const uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBA10ToYCbCr411P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);

void RGB10ToYCbCr411P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ABGR10ToYCbCr411P10(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void ARGB10ToYCbCr411P10(const uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t * yPlane, uint32_t yStride,
                        uint16_t * uPlane, uint32_t uStride,
                        uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void BGRA10ToYCbCr411P10(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void BGR10ToYCbCr411P10(const uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t * yPlane, uint32_t yStride,
                        uint16_t * uPlane, uint32_t uStride,
                        uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBA12ToYCbCr411P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);

void RGB12ToYCbCr411P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ABGR12ToYCbCr411P12(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void ARGB12ToYCbCr411P12(const uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t * yPlane, uint32_t yStride,
                        uint16_t * uPlane, uint32_t uStride,
                        uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void BGRA12ToYCbCr411P12(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void BGR12ToYCbCr411P12(const uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t * yPlane, uint32_t yStride,
                        uint16_t * uPlane, uint32_t uStride,
                        uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr411P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr411P10ToARGB10(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P10ToABGR10(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P10ToBGRA10(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P10ToBGR10(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr411P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr411P12ToARGB12(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P12ToABGR12(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P12ToBGRA12(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr411P12ToBGR12(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
#endif

// MARK: YCbCr410

void RGBAToYCbCr410(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void RGBToYCbCr410(const uint8_t *src, uint32_t srcStride,
                   uint32_t width, uint32_t height,
                   uint8_t *yPlane, uint32_t yStride,
                   uint8_t *uPlane, uint32_t uStride,
                   uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);

#if SPARKYUV_FULL_CHANNELS
void ABGRToYCbCr410(const uint8_t * src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t * yPlane, uint32_t yStride,
                    uint8_t * uPlane, uint32_t uStride,
                    uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void ARGBToYCbCr410(const uint8_t * src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t * yPlane, uint32_t yStride,
                    uint8_t * uPlane, uint32_t uStride,
                    uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void BGRAToYCbCr410(const uint8_t * src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t * yPlane, uint32_t yStride,
                    uint8_t * uPlane, uint32_t uStride,
                    uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void BGRToYCbCr410(const uint8_t * src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t * yPlane, uint32_t yStride,
                    uint8_t * uPlane, uint32_t uStride,
                    uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr410ToRGBA(uint8_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *yPlane, uint32_t yStride,
                    const uint8_t *uPlane, uint32_t uStride,
                    const uint8_t *vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410ToRGB(uint8_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height,
                   const uint8_t *yPlane, uint32_t yStride,
                   const uint8_t *uPlane, uint32_t uStride,
                   const uint8_t *vPlane, uint32_t vStride,
                   float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr410ToARGB(uint8_t * dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    const uint8_t * uPlane, uint32_t uStride,
                    const uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410ToABGR(uint8_t * dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    const uint8_t * uPlane, uint32_t uStride,
                    const uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410ToBGRA(uint8_t * dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    const uint8_t * uPlane, uint32_t uStride,
                    const uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410ToBGR(uint8_t * dst, uint32_t dstStride,
                    uint32_t width, uint32_t height,
                    const uint8_t * yPlane, uint32_t yStride,
                    const uint8_t * uPlane, uint32_t uStride,
                    const uint8_t * vPlane, uint32_t vStride,
                    float kr, float kb, SparkYuvColorRange colorRange);
#endif

// MAKR: YCbCr410 P16

void RGBA10ToYCbCr410P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void RGB10ToYCbCr410P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ARGB10ToYCbCr410P10(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void ABGR10ToYCbCr410P10(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void BGRA10ToYCbCr410P10(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void BGR10ToYCbCr410P10(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
#endif

void RGBA12ToYCbCr410P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void RGB12ToYCbCr410P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void ARGB12ToYCbCr410P12(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void ABGR12ToYCbCr410P12(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void BGRA12ToYCbCr410P12(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void BGR12ToYCbCr410P12(const uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t * yPlane, uint32_t yStride,
                         uint16_t * uPlane, uint32_t uStride,
                         uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr410P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr410P10ToABGR10(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P10ToARGB10(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        const uint16_t * uPlane, uint32_t uStride,
                        const uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P10ToBGRA10(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P10ToBGR10(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        const uint16_t * uPlane, uint32_t uStride,
                        const uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

void YCbCr410P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCbCr410P12ToABGR12(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P12ToARGB12(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        const uint16_t * uPlane, uint32_t uStride,
                        const uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P12ToBGRA12(uint16_t * src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t * yPlane, uint32_t yStride,
                         const uint16_t * uPlane, uint32_t uStride,
                         const uint16_t * vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange);
void YCbCr410P12ToBGR12(uint16_t * src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t * yPlane, uint32_t yStride,
                        const uint16_t * uPlane, uint32_t uStride,
                        const uint16_t * vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange);
#endif

}

#endif //YUV_INCLUDE_SPARKYUV_YCBCR_H_
