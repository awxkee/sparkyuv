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
#define PIXEL_TO_YCGCO_H(T, PixelType, bit, yuvname) \
void PixelType##bit##To##yuvname##P##bit(const T * src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,\
                    T * yPlane, const uint32_t yStride,  \
                    T * uPlane, const uint32_t uStride,\
                    T * vPlane, const uint32_t vStride, \
                    const SparkYuvColorRange colorRange);

PIXEL_TO_YCGCO_H(uint16_t, RGBA, 10, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, RGB, 10, YCgCo444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint16_t, BGRA, 10, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, ABGR, 10, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, ARGB, 10, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, BGR, 10, YCgCo444)
#endif

PIXEL_TO_YCGCO_H(uint16_t, RGBA, 12, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, RGB, 12, YCgCo444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint16_t, BGRA, 12, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, ABGR, 12, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, ARGB, 12, YCgCo444)
PIXEL_TO_YCGCO_H(uint16_t, BGR, 12, YCgCo444)
#endif


PIXEL_TO_YCGCO_H(uint16_t, RGBA, 10, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, RGB, 10, YCgCo422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint16_t, BGRA, 10, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, ABGR, 10, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, ARGB, 10, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, BGR, 10, YCgCo422)
#endif

PIXEL_TO_YCGCO_H(uint16_t, RGBA, 12, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, RGB, 12, YCgCo422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint16_t, BGRA, 12, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, ABGR, 12, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, ARGB, 12, YCgCo422)
PIXEL_TO_YCGCO_H(uint16_t, BGR, 12, YCgCo422)
#endif

PIXEL_TO_YCGCO_H(uint16_t, RGBA, 10, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, RGB, 10, YCgCo420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint16_t, BGRA, 10, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, ABGR, 10, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, ARGB, 10, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, BGR, 10, YCgCo420)
#endif

PIXEL_TO_YCGCO_H(uint16_t, RGBA, 12, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, RGB, 12, YCgCo420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint16_t, BGRA, 12, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, ABGR, 12, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, ARGB, 12, YCgCo420)
PIXEL_TO_YCGCO_H(uint16_t, BGR, 12, YCgCo420)
#endif

PIXEL_TO_YCGCO_H(uint8_t, RGBA, 8, YCgCo420)
PIXEL_TO_YCGCO_H(uint8_t, RGB, 8, YCgCo420)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint8_t, BGRA, 8, YCgCo420)
PIXEL_TO_YCGCO_H(uint8_t, ABGR, 8, YCgCo420)
PIXEL_TO_YCGCO_H(uint8_t, ARGB, 8, YCgCo420)
PIXEL_TO_YCGCO_H(uint8_t, BGR, 8, YCgCo420)
#endif

PIXEL_TO_YCGCO_H(uint8_t, RGBA, 8, YCgCo422)
PIXEL_TO_YCGCO_H(uint8_t, RGB, 8, YCgCo422)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint8_t, BGRA, 8, YCgCo422)
PIXEL_TO_YCGCO_H(uint8_t, ABGR, 8, YCgCo422)
PIXEL_TO_YCGCO_H(uint8_t, ARGB, 8, YCgCo422)
PIXEL_TO_YCGCO_H(uint8_t, BGR, 8, YCgCo422)
#endif

PIXEL_TO_YCGCO_H(uint8_t, RGBA, 8, YCgCo444)
PIXEL_TO_YCGCO_H(uint8_t, RGB, 8, YCgCo444)
#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO_H(uint8_t, BGRA, 8, YCgCo444)
PIXEL_TO_YCGCO_H(uint8_t, ABGR, 8, YCgCo444)
PIXEL_TO_YCGCO_H(uint8_t, ARGB, 8, YCgCo444)
PIXEL_TO_YCGCO_H(uint8_t, BGR, 8, YCgCo444)
#endif

#undef PIXEL_TO_YCGCO_H

#define YCGCO_TO_PX_DECLARATION_H(T, pixel, bit, yuv) \
    void yuv##P##bit##To##pixel##bit(T * src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const T * yPlane, const uint32_t yStride,\
                                             const T * uPlane, const uint32_t uStride,\
                                             const T * vPlane, const uint32_t vStride,\
                                             const SparkYuvColorRange colorRange);

YCGCO_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGBA, 10, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGB, 10, YCgCo420)

#if SPARKYUV_FULL_CHANNELS
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGRA, 10, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ABGR, 10, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ARGB, 10, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGR, 10, YCgCo420)
#endif

YCGCO_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint8_t, RGBA, 8, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint8_t, RGB, 8, YCgCo420)

#if SPARKYUV_FULL_CHANNELS
YCGCO_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint8_t, BGRA, 8, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint8_t, ABGR, 8, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint8_t, ARGB, 8, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint8_t, BGR, 8, YCgCo420)
#endif

YCGCO_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGBA, 12, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, RGB, 12, YCgCo420)

#if SPARKYUV_FULL_CHANNELS
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YCgCo444)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YCgCo422)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGRA, 12, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ABGR, 12, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, ARGB, 12, YCgCo420)
YCGCO_TO_PX_DECLARATION_H(uint16_t, BGR, 12, YCgCo420)
#endif

#undef YCGCO_TO_PX_DECLARATION_H

}
#endif //YUV_INCLUDE_SPARKYUV_YCGCO_H_
