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

#ifndef YUV_INCLUDE_SPARKYUV_YDBDR_H_
#define YUV_INCLUDE_SPARKYUV_YDBDR_H_

#include <cstdint>
#include "sparkyuv-def.h"

namespace sparkyuv {
/**
 * Convert RGB to YDbDr 444 10 bit
 */
void RGBA10ToYDbDr444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB10ToYDbDr444P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYDbDr444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYDbDr444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYDbDr444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYDbDr444P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YDbDr 444 12 bit
 */
void RGBA12ToYDbDr444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB12ToYDbDr444P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYDbDr444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYDbDr444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYDbDr444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYDbDr444P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YDbDr 422 10 bit
 */
void RGBA10ToYDbDr422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB10ToYDbDr422P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYDbDr422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYDbDr422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYDbDr422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYDbDr422P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YDbDr 422 12 bit
 */
void RGBA12ToYDbDr422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB12ToYDbDr422P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYDbDr422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYDbDr422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYDbDr422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYDbDr422P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YDbDr 420 10 bit
 */
void RGBA10ToYDbDr420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB10ToYDbDr420P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYDbDr420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR10ToYDbDr420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB10ToYDbDr420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR10ToYDbDr420P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif
/**
 * Convert RGB to YDbDr 420 12 bit
 */
void RGBA12ToYDbDr420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void RGB12ToYDbDr420P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYDbDr420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ABGR12ToYDbDr420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void ARGB12ToYDbDr420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void BGR12ToYDbDr420P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YDbDr 420 8 bit
 */
void RGBA8ToYDbDr420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB8ToYDbDr420P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYDbDr420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYDbDr420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYDbDr420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYDbDr420P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YDbDr 422 8 bit
 */
void RGBA8ToYDbDr422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB8ToYDbDr422P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYDbDr422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYDbDr422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYDbDr422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYDbDr422P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert RGB to YDbDr 444 8 bit
 */
void RGBA8ToYDbDr444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void RGB8ToYDbDr444P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYDbDr444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ABGR8ToYDbDr444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void ARGB8ToYDbDr444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void BGR8ToYDbDr444P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:4:4 10 bit to RGBA 10 bit
 */
void YDbDr444P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr444P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:4:4 12 bit to RGBA 12 bit
 */
void YDbDr444P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr444P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr444P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:2:2 10 bit to RGBA 10 bit
 */
void YDbDr422P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr422P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:2:2 12 bit to RGBA 12 bit
 */
void YDbDr422P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr422P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr422P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:2:0 10 bit to RGBA 10 bit
 */
void YDbDr420P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P10ToRGB10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr420P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:2:0 12 bit to RGBA 12 bit
 */
void YDbDr420P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P12ToRGB12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr420P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvColorRange colorRange);
void YDbDr420P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:4:4 8 bit to RGBA 8 bit
 */
void YDbDr444P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr444P8ToRGB8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr444P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr444P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr444P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr444P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:2:2 8 bit to RGBA 8 bit
 */
void YDbDr422P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr422P8ToRGB8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr422P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr422P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr422P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr422P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif

/**
 * Convert YDbDr 4:2:0 8 bit to RGBA 8 bit
 */
void YDbDr420P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr420P8ToRGB8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#if SPARKYUV_FULL_CHANNELS
void YDbDr420P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr420P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr420P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvColorRange colorRange);
void YDbDr420P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvColorRange colorRange);
#endif
}
#endif //YUV_INCLUDE_SPARKYUV_YDBDR_H_
