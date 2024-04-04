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
#define PIXEL_TO_YCCBCCRC_H(T, PixelType, bit, yuvname) \
void PixelType##bit##To##yuvname##P##bit(const T * src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,\
                    T * yPlane, const uint32_t yStride,  \
                    T * uPlane, const uint32_t uStride,\
                    T * vPlane, const uint32_t vStride, \
                    const float kr, const float kb,\
                    const SparkYuvColorRange colorRange,\
                    const SparkYuvTransferFunction transferFunction);

PIXEL_TO_YCCBCCRC_H(uint16_t, RGBA, 10, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, RGB, 10, YcCbcCrc444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint16_t, BGRA, 10, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, ABGR, 10, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, ARGB, 10, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, BGR, 10, YcCbcCrc444)
#endif

PIXEL_TO_YCCBCCRC_H(uint16_t, RGBA, 12, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, RGB, 12, YcCbcCrc444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint16_t, BGRA, 12, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, ABGR, 12, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, ARGB, 12, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint16_t, BGR, 12, YcCbcCrc444)
#endif


PIXEL_TO_YCCBCCRC_H(uint16_t, RGBA, 10, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, RGB, 10, YcCbcCrc422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint16_t, BGRA, 10, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, ABGR, 10, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, ARGB, 10, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, BGR, 10, YcCbcCrc422)
#endif

PIXEL_TO_YCCBCCRC_H(uint16_t, RGBA, 12, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, RGB, 12, YcCbcCrc422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint16_t, BGRA, 12, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, ABGR, 12, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, ARGB, 12, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint16_t, BGR, 12, YcCbcCrc422)
#endif

PIXEL_TO_YCCBCCRC_H(uint16_t, RGBA, 10, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, RGB, 10, YcCbcCrc420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint16_t, BGRA, 10, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, ABGR, 10, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, ARGB, 10, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, BGR, 10, YcCbcCrc420)
#endif

PIXEL_TO_YCCBCCRC_H(uint16_t, RGBA, 12, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, RGB, 12, YcCbcCrc420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint16_t, BGRA, 12, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, ABGR, 12, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, ARGB, 12, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint16_t, BGR, 12, YcCbcCrc420)
#endif

PIXEL_TO_YCCBCCRC_H(uint8_t, RGBA, 8, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint8_t, RGB, 8, YcCbcCrc420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint8_t, BGRA, 8, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint8_t, ABGR, 8, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint8_t, ARGB, 8, YcCbcCrc420)
PIXEL_TO_YCCBCCRC_H(uint8_t, BGR, 8, YcCbcCrc420)
#endif

PIXEL_TO_YCCBCCRC_H(uint8_t, RGBA, 8, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint8_t, RGB, 8, YcCbcCrc422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint8_t, BGRA, 8, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint8_t, ABGR, 8, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint8_t, ARGB, 8, YcCbcCrc422)
PIXEL_TO_YCCBCCRC_H(uint8_t, BGR, 8, YcCbcCrc422)
#endif

PIXEL_TO_YCCBCCRC_H(uint8_t, RGBA, 8, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint8_t, RGB, 8, YcCbcCrc444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC_H(uint8_t, BGRA, 8, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint8_t, ABGR, 8, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint8_t, ARGB, 8, YcCbcCrc444)
PIXEL_TO_YCCBCCRC_H(uint8_t, BGR, 8, YcCbcCrc444)
#endif

#undef PIXEL_TO_YCCBCCRC_H

#define YCCBCCRC_TO_PX_DECLARATION_H(T, pixel, bit, yuv) \
    void yuv##P##bit##To##pixel##bit(T * src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const T * yPlane, const uint32_t yStride,\
                                             const T * uPlane, const uint32_t uStride,\
                                             const T * vPlane, const uint32_t vStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange, \
                                             const SparkYuvTransferFunction transferFunction);

YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YcCbcCrc420)

#if SPARKYUV_FULL_CHANNELS
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YcCbcCrc420)
#endif

YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YcCbcCrc420)

#if SPARKYUV_FULL_CHANNELS
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YcCbcCrc420)
#endif

YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YcCbcCrc420)

#if SPARKYUV_FULL_CHANNELS
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YcCbcCrc444)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YcCbcCrc422)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YcCbcCrc420)
YCCBCCRC_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YcCbcCrc420)
#endif

#undef YCCBCCRC_TO_PX_DECLARATION_H

}

#endif //YUV_INCLUDE_SPARKYUV_YCCBCCRC_H_
