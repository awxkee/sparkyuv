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

#ifndef YUV_INCLUDE_SPARKYUV_YCGCO_H_
#define YUV_INCLUDE_SPARKYUV_YCGCO_H_
namespace sparkyuv {

/**
 * Convert RGB to YCgCo 444 10 bit
 */
void RGBA10ToYCgCo444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB10ToYCgCo444P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYCgCo444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYCgCo444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYCgCo444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYCgCo444P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YCgCo 444 12 bit
 */
void RGBA12ToYCgCo444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB12ToYCgCo444P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYCgCo444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYCgCo444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYCgCo444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYCgCo444P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YCgCo 422 10 bit
 */
void RGBA10ToYCgCo422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB10ToYCgCo422P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYCgCo422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYCgCo422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYCgCo422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYCgCo422P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YCgCo 422 12 bit
 */
void RGBA12ToYCgCo422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB12ToYCgCo422P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYCgCo422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYCgCo422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYCgCo422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYCgCo422P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YCgCo 420 10 bit
 */
void RGBA10ToYCgCo420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB10ToYCgCo420P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYCgCo420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYCgCo420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYCgCo420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYCgCo420P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YCgCo 420 12 bit
 */
void RGBA12ToYCgCo420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB12ToYCgCo420P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYCgCo420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYCgCo420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYCgCo420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYCgCo420P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YCgCo 420 8 bit
 */
void RGBA8ToYCgCo420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB8ToYCgCo420P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYCgCo420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYCgCo420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYCgCo420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYCgCo420P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YCgCo 422 8 bit
 */
void RGBA8ToYCgCo422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB8ToYCgCo422P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYCgCo422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYCgCo422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYCgCo422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYCgCo422P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YCgCo 444 8 bit
 */
void RGBA8ToYCgCo444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB8ToYCgCo444P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYCgCo444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYCgCo444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYCgCo444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYCgCo444P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:4:4 10 bit to RGBA 10 bit
 */
void YCgCo444P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo444P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:4:4 12 bit to RGBA 12 bit
 */
void YCgCo444P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo444P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo444P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:2:2 10 bit to RGBA 10 bit
 */
void YCgCo422P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo422P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:2:2 12 bit to RGBA 12 bit
 */
void YCgCo422P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo422P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo422P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:2:0 10 bit to RGBA 10 bit
 */
void YCgCo420P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo420P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:2:0 12 bit to RGBA 12 bit
 */
void YCgCo420P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo420P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YCgCo420P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:4:4 8 bit to RGBA 8 bit
 */
void YCgCo444P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo444P8ToRGB8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo444P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo444P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo444P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo444P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:2:2 8 bit to RGBA 8 bit
 */
void YCgCo422P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo422P8ToRGB8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo422P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo422P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo422P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo422P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YCgCo 4:2:0 8 bit to RGBA 8 bit
 */
void YCgCo420P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo420P8ToRGB8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YCgCo420P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo420P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo420P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YCgCo420P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif
}
#endif //YUV_INCLUDE_SPARKYUV_YCGCO_H_
