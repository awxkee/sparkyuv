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

#ifndef YUV_INCLUDE_SPARKYUV_YCgCoRR_H_
#define YUV_INCLUDE_SPARKYUV_YCgCoRR_H_

#include <cstdint>
#include "sparkyuv-def.h"

namespace sparkyuv {
/**
 * Convert RGB to YCgCoR 444 10 bit
 */
void RGBA10ToYCgCoR444P10(const uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint16_t *yPlane, uint32_t yStride,
                          uint16_t *uPlane, uint32_t uStride,
                          uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void RGB10ToYCgCoR444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYCgCoR444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ABGR10ToYCgCoR444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ARGB10ToYCgCoR444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void BGR10ToYCgCoR444P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif
/**
 * Convert RGB to YCgCoR 444 12 bit
 */
void RGBA12ToYCgCoR444P12(const uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint16_t *yPlane, uint32_t yStride,
                          uint16_t *uPlane, uint32_t uStride,
                          uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void RGB12ToYCgCoR444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYCgCoR444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ABGR12ToYCgCoR444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ARGB12ToYCgCoR444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void BGR12ToYCgCoR444P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert RGB to YCgCoR 422 10 bit
 */
void RGBA10ToYCgCoR422P10(const uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint16_t *yPlane, uint32_t yStride,
                          uint16_t *uPlane, uint32_t uStride,
                          uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void RGB10ToYCgCoR422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYCgCoR422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ABGR10ToYCgCoR422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ARGB10ToYCgCoR422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void BGR10ToYCgCoR422P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif
/**
 * Convert RGB to YCgCoR 422 12 bit
 */
void RGBA12ToYCgCoR422P12(const uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint16_t *yPlane, uint32_t yStride,
                          uint16_t *uPlane, uint32_t uStride,
                          uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void RGB12ToYCgCoR422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYCgCoR422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ABGR12ToYCgCoR422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ARGB12ToYCgCoR422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void BGR12ToYCgCoR422P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert RGB to YCgCoR 420 10 bit
 */
void RGBA10ToYCgCoR420P10(const uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint16_t *yPlane, uint32_t yStride,
                          uint16_t *uPlane, uint32_t uStride,
                          uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void RGB10ToYCgCoR420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYCgCoR420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ABGR10ToYCgCoR420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ARGB10ToYCgCoR420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void BGR10ToYCgCoR420P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif
/**
 * Convert RGB to YCgCoR 420 12 bit
 */
void RGBA12ToYCgCoR420P12(const uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint16_t *yPlane, uint32_t yStride,
                          uint16_t *uPlane, uint32_t uStride,
                          uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void RGB12ToYCgCoR420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYCgCoR420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ABGR12ToYCgCoR420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void ARGB12ToYCgCoR420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void BGR12ToYCgCoR420P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert RGB to YCgCoR 420 8 bit
 */
void RGBA8ToYCgCoR420P8(const uint8_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint8_t *yPlane, uint32_t yStride,
                        uint8_t *uPlane, uint32_t uStride,
                        uint8_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
void RGB8ToYCgCoR420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYCgCoR420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void ABGR8ToYCgCoR420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void ARGB8ToYCgCoR420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void BGR8ToYCgCoR420P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvYCgCoRType rType);
#endif

/**
 * Convert RGB to YCgCoR 422 8 bit
 */
void RGBA8ToYCgCoR422P8(const uint8_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint8_t *yPlane, uint32_t yStride,
                        uint8_t *uPlane, uint32_t uStride,
                        uint8_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
void RGB8ToYCgCoR422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYCgCoR422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void ABGR8ToYCgCoR422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void ARGB8ToYCgCoR422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void BGR8ToYCgCoR422P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvYCgCoRType rType);
#endif

/**
 * Convert RGB to YCgCoR 444 8 bit
 */
void RGBA8ToYCgCoR444P8(const uint8_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint8_t *yPlane, uint32_t yStride,
                        uint8_t *uPlane, uint32_t uStride,
                        uint8_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
void RGB8ToYCgCoR444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYCgCoR444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void ABGR8ToYCgCoR444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void ARGB8ToYCgCoR444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void BGR8ToYCgCoR444P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:4:4 10 bit to RGBA 10 bit
 */
void YCgCoR444P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint16_t *yPlane, uint32_t yStride,
                          const uint16_t *uPlane, uint32_t uStride,
                          const uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void YCgCoR444P10ToRGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR444P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR444P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR444P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR444P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:4:4 12 bit to RGBA 12 bit
 */
void YCgCoR444P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint16_t *yPlane, uint32_t yStride,
                          const uint16_t *uPlane, uint32_t uStride,
                          const uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void YCgCoR444P12ToRGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR444P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR444P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR444P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR444P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:2:2 10 bit to RGBA 10 bit
 */
void YCgCoR422P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint16_t *yPlane, uint32_t yStride,
                          const uint16_t *uPlane, uint32_t uStride,
                          const uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void YCgCoR422P10ToRGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR422P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR422P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR422P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR422P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:2:2 12 bit to RGBA 12 bit
 */
void YCgCoR422P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint16_t *yPlane, uint32_t yStride,
                          const uint16_t *uPlane, uint32_t uStride,
                          const uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void YCgCoR422P12ToRGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR422P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR422P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR422P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR422P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:2:0 10 bit to RGBA 10 bit
 */
void YCgCoR420P10ToRGBA10(uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint16_t *yPlane, uint32_t yStride,
                          const uint16_t *uPlane, uint32_t uStride,
                          const uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void YCgCoR420P10ToRGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR420P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR420P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR420P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR420P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:2:0 12 bit to RGBA 12 bit
 */
void YCgCoR420P12ToRGBA12(uint16_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint16_t *yPlane, uint32_t yStride,
                          const uint16_t *uPlane, uint32_t uStride,
                          const uint16_t *vPlane, uint32_t vStride,
                          SparkYuvYCgCoRType rType);
void YCgCoR420P12ToRGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR420P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR420P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR420P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         SparkYuvYCgCoRType rType);
void YCgCoR420P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:4:4 8 bit to RGBA 8 bit
 */
void YCgCoR444P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint8_t *yPlane, uint32_t yStride,
                        const uint8_t *uPlane, uint32_t uStride,
                        const uint8_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
void YCgCoR444P8ToRGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR444P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR444P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR444P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR444P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:2:2 8 bit to RGBA 8 bit
 */
void YCgCoR422P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint8_t *yPlane, uint32_t yStride,
                        const uint8_t *uPlane, uint32_t uStride,
                        const uint8_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
void YCgCoR422P8ToRGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR422P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR422P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR422P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR422P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvYCgCoRType rType);
#endif

/**
 * Convert YCgCoR 4:2:0 8 bit to RGBA 8 bit
 */
void YCgCoR420P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint8_t *yPlane, uint32_t yStride,
                        const uint8_t *uPlane, uint32_t uStride,
                        const uint8_t *vPlane, uint32_t vStride,
                        SparkYuvYCgCoRType rType);
void YCgCoR420P8ToRGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
#if SPARKYUV_FULL_CHANNELS
void YCgCoR420P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR420P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR420P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       SparkYuvYCgCoRType rType);
void YCgCoR420P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      SparkYuvYCgCoRType rType);
#endif

}
#endif //YUV_INCLUDE_SPARKYUV_YCgCoRR_H_
