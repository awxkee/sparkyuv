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


#ifndef YUV_INCLUDE_SPARKYUV_YCCBCCRC_H_
#define YUV_INCLUDE_SPARKYUV_YCCBCCRC_H_

namespace sparkyuv {
/**
 * Convert RGB to YcCbcCrc 444 10 bit
 */
void RGBA10ToYcCbcCrc444P10(const uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            uint16_t *yPlane,
                            uint32_t yStride,
                            uint16_t *uPlane,
                            uint32_t uStride,
                            uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void RGB10ToYcCbcCrc444P10(const uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           uint16_t *yPlane,
                           uint32_t yStride,
                           uint16_t *uPlane,
                           uint32_t uStride,
                           uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYcCbcCrc444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR10ToYcCbcCrc444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB10ToYcCbcCrc444P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR10ToYcCbcCrc444P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif
/**
 * Convert RGB to YcCbcCrc 444 12 bit
 */
void RGBA12ToYcCbcCrc444P12(const uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            uint16_t *yPlane,
                            uint32_t yStride,
                            uint16_t *uPlane,
                            uint32_t uStride,
                            uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void RGB12ToYcCbcCrc444P12(const uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           uint16_t *yPlane,
                           uint32_t yStride,
                           uint16_t *uPlane,
                           uint32_t uStride,
                           uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYcCbcCrc444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR12ToYcCbcCrc444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB12ToYcCbcCrc444P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR12ToYcCbcCrc444P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert RGB to YcCbcCrc 422 10 bit
 */
void RGBA10ToYcCbcCrc422P10(const uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            uint16_t *yPlane,
                            uint32_t yStride,
                            uint16_t *uPlane,
                            uint32_t uStride,
                            uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void RGB10ToYcCbcCrc422P10(const uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           uint16_t *yPlane,
                           uint32_t yStride,
                           uint16_t *uPlane,
                           uint32_t uStride,
                           uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYcCbcCrc422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR10ToYcCbcCrc422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB10ToYcCbcCrc422P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR10ToYcCbcCrc422P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif
/**
 * Convert RGB to YcCbcCrc 422 12 bit
 */
void RGBA12ToYcCbcCrc422P12(const uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            uint16_t *yPlane,
                            uint32_t yStride,
                            uint16_t *uPlane,
                            uint32_t uStride,
                            uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void RGB12ToYcCbcCrc422P12(const uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           uint16_t *yPlane,
                           uint32_t yStride,
                           uint16_t *uPlane,
                           uint32_t uStride,
                           uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYcCbcCrc422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR12ToYcCbcCrc422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB12ToYcCbcCrc422P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR12ToYcCbcCrc422P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert RGB to YcCbcCrc 420 10 bit
 */
void RGBA10ToYcCbcCrc420P10(const uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            uint16_t *yPlane,
                            uint32_t yStride,
                            uint16_t *uPlane,
                            uint32_t uStride,
                            uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void RGB10ToYcCbcCrc420P10(const uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           uint16_t *yPlane,
                           uint32_t yStride,
                           uint16_t *uPlane,
                           uint32_t uStride,
                           uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA10ToYcCbcCrc420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR10ToYcCbcCrc420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB10ToYcCbcCrc420P10(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR10ToYcCbcCrc420P10(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif
/**
 * Convert RGB to YcCbcCrc 420 12 bit
 */
void RGBA12ToYcCbcCrc420P12(const uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            uint16_t *yPlane,
                            uint32_t yStride,
                            uint16_t *uPlane,
                            uint32_t uStride,
                            uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void RGB12ToYcCbcCrc420P12(const uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           uint16_t *yPlane,
                           uint32_t yStride,
                           uint16_t *uPlane,
                           uint32_t uStride,
                           uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA12ToYcCbcCrc420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR12ToYcCbcCrc420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB12ToYcCbcCrc420P12(const uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint16_t *yPlane, uint32_t yStride,
                         uint16_t *uPlane, uint32_t uStride,
                         uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR12ToYcCbcCrc420P12(const uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        uint16_t *yPlane, uint32_t yStride,
                        uint16_t *uPlane, uint32_t uStride,
                        uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert RGB to YcCbcCrc 420 8 bit
 */
void RGBA8ToYcCbcCrc420P8(const uint8_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint8_t *yPlane, uint32_t yStride,
                          uint8_t *uPlane, uint32_t uStride,
                          uint8_t *vPlane, uint32_t vStride,
                          float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void RGB8ToYcCbcCrc420P8(const uint8_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint8_t *yPlane, uint32_t yStride,
                         uint8_t *uPlane, uint32_t uStride,
                         uint8_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYcCbcCrc420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR8ToYcCbcCrc420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB8ToYcCbcCrc420P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR8ToYcCbcCrc420P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert RGB to YcCbcCrc 422 8 bit
 */
void RGBA8ToYcCbcCrc422P8(const uint8_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint8_t *yPlane, uint32_t yStride,
                          uint8_t *uPlane, uint32_t uStride,
                          uint8_t *vPlane, uint32_t vStride,
                          float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void RGB8ToYcCbcCrc422P8(const uint8_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint8_t *yPlane, uint32_t yStride,
                         uint8_t *uPlane, uint32_t uStride,
                         uint8_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYcCbcCrc422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR8ToYcCbcCrc422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB8ToYcCbcCrc422P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR8ToYcCbcCrc422P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert RGB to YcCbcCrc 444 8 bit
 */
void RGBA8ToYcCbcCrc444P8(const uint8_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          uint8_t *yPlane, uint32_t yStride,
                          uint8_t *uPlane, uint32_t uStride,
                          uint8_t *vPlane, uint32_t vStride,
                          float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void RGB8ToYcCbcCrc444P8(const uint8_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         uint8_t *yPlane, uint32_t yStride,
                         uint8_t *uPlane, uint32_t uStride,
                         uint8_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void BGRA8ToYcCbcCrc444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ABGR8ToYcCbcCrc444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void ARGB8ToYcCbcCrc444P8(const uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       uint8_t *yPlane, uint32_t yStride,
                       uint8_t *uPlane, uint32_t uStride,
                       uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void BGR8ToYcCbcCrc444P8(const uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      uint8_t *yPlane, uint32_t yStride,
                      uint8_t *uPlane, uint32_t uStride,
                      uint8_t *vPlane, uint32_t vStride,
                      float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:4:4 10 bit to RGBA 10 bit
 */
void YcCbcCrc444P10ToRGBA10(uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            const uint16_t *yPlane,
                            uint32_t yStride,
                            const uint16_t *uPlane,
                            uint32_t uStride,
                            const uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P10ToRGB10(uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           const uint16_t *yPlane,
                           uint32_t yStride,
                           const uint16_t *uPlane,
                           uint32_t uStride,
                           const uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc444P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:4:4 12 bit to RGBA 12 bit
 */
void YcCbcCrc444P12ToRGBA12(uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            const uint16_t *yPlane,
                            uint32_t yStride,
                            const uint16_t *uPlane,
                            uint32_t uStride,
                            const uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P12ToRGB12(uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           const uint16_t *yPlane,
                           uint32_t yStride,
                           const uint16_t *uPlane,
                           uint32_t uStride,
                           const uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc444P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:2:2 10 bit to RGBA 10 bit
 */
void YcCbcCrc422P10ToRGBA10(uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            const uint16_t *yPlane,
                            uint32_t yStride,
                            const uint16_t *uPlane,
                            uint32_t uStride,
                            const uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P10ToRGB10(uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           const uint16_t *yPlane,
                           uint32_t yStride,
                           const uint16_t *uPlane,
                           uint32_t uStride,
                           const uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc422P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:2:2 12 bit to RGBA 12 bit
 */
void YcCbcCrc422P12ToRGBA12(uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            const uint16_t *yPlane,
                            uint32_t yStride,
                            const uint16_t *uPlane,
                            uint32_t uStride,
                            const uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P12ToRGB12(uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           const uint16_t *yPlane,
                           uint32_t yStride,
                           const uint16_t *uPlane,
                           uint32_t uStride,
                           const uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc422P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:2:0 10 bit to RGBA 10 bit
 */
void YcCbcCrc420P10ToRGBA10(uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            const uint16_t *yPlane,
                            uint32_t yStride,
                            const uint16_t *uPlane,
                            uint32_t uStride,
                            const uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P10ToRGB10(uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           const uint16_t *yPlane,
                           uint32_t yStride,
                           const uint16_t *uPlane,
                           uint32_t uStride,
                           const uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc420P10ToBGRA10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P10ToABGR10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P10ToARGB10(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P10ToBGR10(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:2:0 12 bit to RGBA 12 bit
 */
void YcCbcCrc420P12ToRGBA12(uint16_t *src,
                            uint32_t srcStride,
                            uint32_t width,
                            uint32_t height,
                            const uint16_t *yPlane,
                            uint32_t yStride,
                            const uint16_t *uPlane,
                            uint32_t uStride,
                            const uint16_t *vPlane,
                            uint32_t vStride,
                            float kr,
                            float kb,
                            SparkYuvColorRange colorRange,
                            SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P12ToRGB12(uint16_t *src,
                           uint32_t srcStride,
                           uint32_t width,
                           uint32_t height,
                           const uint16_t *yPlane,
                           uint32_t yStride,
                           const uint16_t *uPlane,
                           uint32_t uStride,
                           const uint16_t *vPlane,
                           uint32_t vStride,
                           float kr,
                           float kb,
                           SparkYuvColorRange colorRange,
                           SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc420P12ToBGRA12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P12ToABGR12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P12ToARGB12(uint16_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint16_t *yPlane, uint32_t yStride,
                         const uint16_t *uPlane, uint32_t uStride,
                         const uint16_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P12ToBGR12(uint16_t *src, uint32_t srcStride,
                        uint32_t width, uint32_t height,
                        const uint16_t *yPlane, uint32_t yStride,
                        const uint16_t *uPlane, uint32_t uStride,
                        const uint16_t *vPlane, uint32_t vStride,
                        float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:4:4 8 bit to RGBA 8 bit
 */
void YcCbcCrc444P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint8_t *yPlane, uint32_t yStride,
                          const uint8_t *uPlane, uint32_t uStride,
                          const uint8_t *vPlane, uint32_t vStride,
                          float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P8ToRGB8(uint8_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint8_t *yPlane, uint32_t yStride,
                         const uint8_t *uPlane, uint32_t uStride,
                         const uint8_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc444P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc444P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:2:2 8 bit to RGBA 8 bit
 */
void YcCbcCrc422P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint8_t *yPlane, uint32_t yStride,
                          const uint8_t *uPlane, uint32_t uStride,
                          const uint8_t *vPlane, uint32_t vStride,
                          float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P8ToRGB8(uint8_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint8_t *yPlane, uint32_t yStride,
                         const uint8_t *uPlane, uint32_t uStride,
                         const uint8_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc422P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc422P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

/**
 * Convert YcCbcCrc 4:2:0 8 bit to RGBA 8 bit
 */
void YcCbcCrc420P8ToRGBA8(uint8_t *src, uint32_t srcStride,
                          uint32_t width, uint32_t height,
                          const uint8_t *yPlane, uint32_t yStride,
                          const uint8_t *uPlane, uint32_t uStride,
                          const uint8_t *vPlane, uint32_t vStride,
                          float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P8ToRGB8(uint8_t *src, uint32_t srcStride,
                         uint32_t width, uint32_t height,
                         const uint8_t *yPlane, uint32_t yStride,
                         const uint8_t *uPlane, uint32_t uStride,
                         const uint8_t *vPlane, uint32_t vStride,
                         float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#if SPARKYUV_FULL_CHANNELS
void YcCbcCrc420P8ToBGRA8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P8ToABGR8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P8ToARGB8(uint8_t *src, uint32_t srcStride,
                       uint32_t width, uint32_t height,
                       const uint8_t *yPlane, uint32_t yStride,
                       const uint8_t *uPlane, uint32_t uStride,
                       const uint8_t *vPlane, uint32_t vStride,
                       float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
void YcCbcCrc420P8ToBGR8(uint8_t *src, uint32_t srcStride,
                      uint32_t width, uint32_t height,
                      const uint8_t *yPlane, uint32_t yStride,
                      const uint8_t *uPlane, uint32_t uStride,
                      const uint8_t *vPlane, uint32_t vStride,
                      float kr, float kb, SparkYuvColorRange colorRange, SparkYuvTransferFunction transferFunction);
#endif

}

#endif //YUV_INCLUDE_SPARKYUV_YCCBCCRC_H_
