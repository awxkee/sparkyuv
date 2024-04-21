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


#ifndef YUV_INCLUDE_SPARKYUV_YIQ_H_
#define YUV_INCLUDE_SPARKYUV_YIQ_H_

#include <cstdint>
#include "sparkyuv-def.h"

namespace sparkyuv {
/**
 * Convert RGB to YIQ 444 10 bit
 */
void RGBA10ToYIQ444P10(const uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint16_t *yPlane, uint32_t yStride,
                       uint16_t *uPlane, uint32_t uStride,
                       uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB10ToYIQ444P10(const uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint16_t *yPlane, uint32_t yStride,
                      uint16_t *uPlane, uint32_t uStride,
                      uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYIQ444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYIQ444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYIQ444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYIQ444P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YIQ 444 12 bit
 */
void RGBA12ToYIQ444P12(const uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint16_t *yPlane, uint32_t yStride,
                       uint16_t *uPlane, uint32_t uStride,
                       uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB12ToYIQ444P12(const uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint16_t *yPlane, uint32_t yStride,
                      uint16_t *uPlane, uint32_t uStride,
                      uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYIQ444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYIQ444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYIQ444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYIQ444P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YIQ 422 10 bit
 */
void RGBA10ToYIQ422P10(const uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint16_t *yPlane, uint32_t yStride,
                       uint16_t *uPlane, uint32_t uStride,
                       uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB10ToYIQ422P10(const uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint16_t *yPlane, uint32_t yStride,
                      uint16_t *uPlane, uint32_t uStride,
                      uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYIQ422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYIQ422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYIQ422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYIQ422P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YIQ 422 12 bit
 */
void RGBA12ToYIQ422P12(const uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint16_t *yPlane, uint32_t yStride,
                       uint16_t *uPlane, uint32_t uStride,
                       uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB12ToYIQ422P12(const uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint16_t *yPlane, uint32_t yStride,
                      uint16_t *uPlane, uint32_t uStride,
                      uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYIQ422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYIQ422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYIQ422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYIQ422P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YIQ 420 10 bit
 */
void RGBA10ToYIQ420P10(const uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint16_t *yPlane, uint32_t yStride,
                       uint16_t *uPlane, uint32_t uStride,
                       uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB10ToYIQ420P10(const uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint16_t *yPlane, uint32_t yStride,
                      uint16_t *uPlane, uint32_t uStride,
                      uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYIQ420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYIQ420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYIQ420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYIQ420P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YIQ 420 12 bit
 */
void RGBA12ToYIQ420P12(const uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint16_t *yPlane, uint32_t yStride,
                       uint16_t *uPlane, uint32_t uStride,
                       uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB12ToYIQ420P12(const uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint16_t *yPlane, uint32_t yStride,
                      uint16_t *uPlane, uint32_t uStride,
                      uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYIQ420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYIQ420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYIQ420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYIQ420P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YIQ 420 8 bit
 */
void RGBA8ToYIQ420P8(const uint8_t *src, uint32_t srcStride,
                     uint32_t width, uint32_t height,
                     uint8_t *yPlane, uint32_t yStride,
                     uint8_t *uPlane, uint32_t uStride,
                     uint8_t *vPlane, uint32_t vStride,
                     SparkYuvColorRange colorRange);
void RGB8ToYIQ420P8(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYIQ420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYIQ420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYIQ420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYIQ420P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YIQ 422 8 bit
 */
void RGBA8ToYIQ422P8(const uint8_t *src, uint32_t srcStride,
                     uint32_t width, uint32_t height,
                     uint8_t *yPlane, uint32_t yStride,
                     uint8_t *uPlane, uint32_t uStride,
                     uint8_t *vPlane, uint32_t vStride,
                     SparkYuvColorRange colorRange);
void RGB8ToYIQ422P8(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYIQ422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYIQ422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYIQ422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYIQ422P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YIQ 444 8 bit
 */
void RGBA8ToYIQ444P8(const uint8_t *src, uint32_t srcStride,
                     uint32_t width, uint32_t height,
                     uint8_t *yPlane, uint32_t yStride,
                     uint8_t *uPlane, uint32_t uStride,
                     uint8_t *vPlane, uint32_t vStride,
                     SparkYuvColorRange colorRange);
void RGB8ToYIQ444P8(const uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    uint8_t *yPlane, uint32_t yStride,
                    uint8_t *uPlane, uint32_t uStride,
                    uint8_t *vPlane, uint32_t vStride,
                    SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYIQ444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYIQ444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYIQ444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYIQ444P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:4:4 10 bit to RGBA 10 bit
 */
void YIQ444P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint16_t *yPlane, uint32_t yStride,
                       const uint16_t *uPlane, uint32_t uStride,
                       const uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ444P10ToRGB10(uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint16_t *yPlane, uint32_t yStride,
                      const uint16_t *uPlane, uint32_t uStride,
                      const uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ444P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ444P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ444P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ444P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:4:4 12 bit to RGBA 12 bit
 */
void YIQ444P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint16_t *yPlane, uint32_t yStride,
                       const uint16_t *uPlane, uint32_t uStride,
                       const uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ444P12ToRGB12(uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint16_t *yPlane, uint32_t yStride,
                      const uint16_t *uPlane, uint32_t uStride,
                      const uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ444P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ444P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ444P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ444P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:2:2 10 bit to RGBA 10 bit
 */
void YIQ422P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint16_t *yPlane, uint32_t yStride,
                       const uint16_t *uPlane, uint32_t uStride,
                       const uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ422P10ToRGB10(uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint16_t *yPlane, uint32_t yStride,
                      const uint16_t *uPlane, uint32_t uStride,
                      const uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ422P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ422P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ422P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ422P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:2:2 12 bit to RGBA 12 bit
 */
void YIQ422P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint16_t *yPlane, uint32_t yStride,
                       const uint16_t *uPlane, uint32_t uStride,
                       const uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ422P12ToRGB12(uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint16_t *yPlane, uint32_t yStride,
                      const uint16_t *uPlane, uint32_t uStride,
                      const uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ422P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ422P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ422P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ422P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:2:0 10 bit to RGBA 10 bit
 */
void YIQ420P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint16_t *yPlane, uint32_t yStride,
                       const uint16_t *uPlane, uint32_t uStride,
                       const uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ420P10ToRGB10(uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint16_t *yPlane, uint32_t yStride,
                      const uint16_t *uPlane, uint32_t uStride,
                      const uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ420P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ420P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ420P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ420P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:2:0 12 bit to RGBA 12 bit
 */
void YIQ420P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint16_t *yPlane, uint32_t yStride,
                       const uint16_t *uPlane, uint32_t uStride,
                       const uint16_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ420P12ToRGB12(uint16_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint16_t *yPlane, uint32_t yStride,
                      const uint16_t *uPlane, uint32_t uStride,
                      const uint16_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ420P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ420P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ420P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YIQ420P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:4:4 8 bit to RGBA 8 bit
 */
void YIQ444P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                     uint32_t width, uint32_t height,
                     const uint8_t *yPlane, uint32_t yStride,
                     const uint8_t *uPlane, uint32_t uStride,
                     const uint8_t *vPlane, uint32_t vStride,
                     SparkYuvColorRange colorRange);
void YIQ444P8ToRGB8(uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *yPlane, uint32_t yStride,
                    const uint8_t *uPlane, uint32_t uStride,
                    const uint8_t *vPlane, uint32_t vStride,
                    SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ444P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ444P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ444P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ444P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:2:2 8 bit to RGBA 8 bit
 */
void YIQ422P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                     uint32_t width, uint32_t height,
                     const uint8_t *yPlane, uint32_t yStride,
                     const uint8_t *uPlane, uint32_t uStride,
                     const uint8_t *vPlane, uint32_t vStride,
                     SparkYuvColorRange colorRange);
void YIQ422P8ToRGB8(uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *yPlane, uint32_t yStride,
                    const uint8_t *uPlane, uint32_t uStride,
                    const uint8_t *vPlane, uint32_t vStride,
                    SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ422P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ422P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ422P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ422P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YIQ 4:2:0 8 bit to RGBA 8 bit
 */
void YIQ420P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                     uint32_t width, uint32_t height,
                     const uint8_t *yPlane, uint32_t yStride,
                     const uint8_t *uPlane, uint32_t uStride,
                     const uint8_t *vPlane, uint32_t vStride,
                     SparkYuvColorRange colorRange);
void YIQ420P8ToRGB8(uint8_t *src, uint32_t srcStride,
                    uint32_t width, uint32_t height,
                    const uint8_t *yPlane, uint32_t yStride,
                    const uint8_t *uPlane, uint32_t uStride,
                    const uint8_t *vPlane, uint32_t vStride,
                    SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YIQ420P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ420P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ420P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YIQ420P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif
}
#endif //YUV_INCLUDE_SPARKYUV_YIQ_H_
