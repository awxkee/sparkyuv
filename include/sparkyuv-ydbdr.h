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

//
// Created by Radzivon Bartoshyk on 06/04/2024.
//

#ifndef YUV_INCLUDE_SPARKYUV_YDBDR_H_
#define YUV_INCLUDE_SPARKYUV_YDBDR_H_
namespace sparkyuv {
#define PIXEL_TO_YDBDR_H(T, PixelType, bit, yuvname) \
void PixelType##bit##To##yuvname##P##bit(const T * src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,\
                    T * yPlane, const uint32_t yStride,  \
                    T * uPlane, const uint32_t uStride,\
                    T * vPlane, const uint32_t vStride, \
                    const SparkYuvColorRange colorRange);

PIXEL_TO_YDBDR_H(uint16_t, RGBA, 10, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, RGB, 10, YDbDr444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint16_t, BGRA, 10, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, ABGR, 10, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, ARGB, 10, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, BGR, 10, YDbDr444)
#endif

PIXEL_TO_YDBDR_H(uint16_t, RGBA, 12, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, RGB, 12, YDbDr444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint16_t, BGRA, 12, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, ABGR, 12, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, ARGB, 12, YDbDr444)
PIXEL_TO_YDBDR_H(uint16_t, BGR, 12, YDbDr444)
#endif


PIXEL_TO_YDBDR_H(uint16_t, RGBA, 10, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, RGB, 10, YDbDr422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint16_t, BGRA, 10, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, ABGR, 10, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, ARGB, 10, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, BGR, 10, YDbDr422)
#endif

PIXEL_TO_YDBDR_H(uint16_t, RGBA, 12, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, RGB, 12, YDbDr422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint16_t, BGRA, 12, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, ABGR, 12, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, ARGB, 12, YDbDr422)
PIXEL_TO_YDBDR_H(uint16_t, BGR, 12, YDbDr422)
#endif

PIXEL_TO_YDBDR_H(uint16_t, RGBA, 10, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, RGB, 10, YDbDr420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint16_t, BGRA, 10, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, ABGR, 10, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, ARGB, 10, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, BGR, 10, YDbDr420)
#endif

PIXEL_TO_YDBDR_H(uint16_t, RGBA, 12, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, RGB, 12, YDbDr420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint16_t, BGRA, 12, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, ABGR, 12, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, ARGB, 12, YDbDr420)
PIXEL_TO_YDBDR_H(uint16_t, BGR, 12, YDbDr420)
#endif

PIXEL_TO_YDBDR_H(uint8_t, RGBA, 8, YDbDr420)
PIXEL_TO_YDBDR_H(uint8_t, RGB, 8, YDbDr420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint8_t, BGRA, 8, YDbDr420)
PIXEL_TO_YDBDR_H(uint8_t, ABGR, 8, YDbDr420)
PIXEL_TO_YDBDR_H(uint8_t, ARGB, 8, YDbDr420)
PIXEL_TO_YDBDR_H(uint8_t, BGR, 8, YDbDr420)
#endif

PIXEL_TO_YDBDR_H(uint8_t, RGBA, 8, YDbDr422)
PIXEL_TO_YDBDR_H(uint8_t, RGB, 8, YDbDr422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint8_t, BGRA, 8, YDbDr422)
PIXEL_TO_YDBDR_H(uint8_t, ABGR, 8, YDbDr422)
PIXEL_TO_YDBDR_H(uint8_t, ARGB, 8, YDbDr422)
PIXEL_TO_YDBDR_H(uint8_t, BGR, 8, YDbDr422)
#endif

PIXEL_TO_YDBDR_H(uint8_t, RGBA, 8, YDbDr444)
PIXEL_TO_YDBDR_H(uint8_t, RGB, 8, YDbDr444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_H(uint8_t, BGRA, 8, YDbDr444)
PIXEL_TO_YDBDR_H(uint8_t, ABGR, 8, YDbDr444)
PIXEL_TO_YDBDR_H(uint8_t, ARGB, 8, YDbDr444)
PIXEL_TO_YDBDR_H(uint8_t, BGR, 8, YDbDr444)
#endif

#undef PIXEL_TO_YDBDR_H

#define YDBDR_TO_PX_DECLARATION_H(T, pixel, bit, yuv) \
    void yuv##P##bit##To##pixel##bit(T * src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const T * yPlane, const uint32_t yStride,\
                                             const T * uPlane, const uint32_t uStride,\
                                             const T * vPlane, const uint32_t vStride,\
                                             const SparkYuvColorRange colorRange);

YDBDR_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YDbDr420)

#if SPARKYUV_FULL_CHANNELS
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YDbDr420)
#endif

YDBDR_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YDbDr420)

#if SPARKYUV_FULL_CHANNELS
YDBDR_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YDbDr420)
#endif

YDBDR_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YDbDr420)

#if SPARKYUV_FULL_CHANNELS
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YDbDr444)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YDbDr422)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YDbDr420)
YDBDR_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YDbDr420)
#endif

#undef YDBDR_TO_PX_DECLARATION_H

}
#endif //YUV_INCLUDE_SPARKYUV_YDBDR_H_
