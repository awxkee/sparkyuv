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


#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "src/YDbDr.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include "TransformMatrix-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
#define PIXEL_TO_YIQ(T, PixelType, bit, yuvname, chroma) \
void PixelType##bit##To##yuvname##P##bit##HWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const SparkYuvColorRange colorRange) {                           \
      TransformPixelToSample<T, sparkyuv::PIXEL_##PixelType, chroma, bit, 8>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               colorRange, kRGBToYDbDrMatrix);\
}

PIXEL_TO_YIQ(uint16_t, RGBA, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, RGB, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YIQ(uint16_t, RGBA, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, RGB, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YIQ(uint16_t, RGBA, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, RGB, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YIQ(uint16_t, RGBA, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, RGB, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YIQ(uint16_t, RGBA, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, RGB, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YIQ(uint16_t, RGBA, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, RGB, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YIQ(uint8_t, RGBA, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint8_t, RGB, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YIQ(uint8_t, RGBA, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint8_t, RGB, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YIQ(uint8_t, RGBA, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint8_t, RGB, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YIQ(uint16_t, BGRA, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, ABGR, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, ARGB, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, BGR, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, BGRA, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, ABGR, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, ARGB, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, BGR, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, BGRA, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, ABGR, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, ARGB, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, BGR, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YIQ(uint8_t, BGRA, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint8_t, ABGR, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint8_t, ARGB, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint8_t, BGR, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint8_t, BGRA, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint8_t, ABGR, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint8_t, ARGB, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint8_t, BGR, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint8_t, BGRA, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint8_t, ABGR, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint8_t, ARGB, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint8_t, BGR, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YIQ(uint16_t, BGRA, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, ABGR, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, ARGB, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, BGR, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YIQ(uint16_t, BGRA, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, ABGR, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, ARGB, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, BGR, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YIQ(uint16_t, BGRA, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, ABGR, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, ARGB, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YIQ(uint16_t, BGR, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef PIXEL_TO_YIQ

#define YIQ_ToXXXX_DECLARATION_R(T, PixelType, bit, yuvname, chroma) \
void yuvname##P##bit##To##PixelType##bit##HWY(T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const sparkyuv::SparkYuvColorRange colorRange) {                           \
      TransformYUVToRGBMatrix<T, sparkyuv::PIXEL_##PixelType, chroma, bit, 10>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               colorRange, kYDbDrToRGBMatrix);  \
}

YIQ_ToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGB, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGB, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGB, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGR, 10, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGR, 10, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGR, 10, YDbDr420, sparkyuv::YUV_SAMPLE_420)
#endif

YIQ_ToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint8_t, RGB, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint8_t, RGB, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint8_t, RGB, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YIQ_ToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint8_t, BGR, 8, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint8_t, BGR, 8, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint8_t, BGR, 8, YDbDr420, sparkyuv::YUV_SAMPLE_420)
#endif

YIQ_ToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGB, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGB, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, RGB, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGR, 12, YDbDr444, sparkyuv::YUV_SAMPLE_444)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGR, 12, YDbDr422, sparkyuv::YUV_SAMPLE_422)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
YIQ_ToXXXX_DECLARATION_R(uint16_t, BGR, 12, YDbDr420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef YIQ_ToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#if HWY_ONCE
namespace sparkyuv {
#define DECLARE_PX_TO_YDBDR_HWY(pixel, bit, yuv) HWY_EXPORT(pixel##bit##To##yuv##P##bit##HWY);

DECLARE_PX_TO_YDBDR_HWY(RGBA, 10, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(RGB, 10, YDbDr444)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 12, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(RGB, 12, YDbDr444)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 10, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(RGB, 10, YDbDr422)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 12, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(RGB, 12, YDbDr422)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 10, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(RGB, 10, YDbDr420)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 12, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(RGB, 12, YDbDr420)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 8, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(RGB, 8, YDbDr420)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 8, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(RGB, 8, YDbDr422)

DECLARE_PX_TO_YDBDR_HWY(RGBA, 8, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(RGB, 8, YDbDr444)

#if SPARKYUV_FULL_CHANNELS
DECLARE_PX_TO_YDBDR_HWY(BGRA, 10, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 10, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 10, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(BGR, 10, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(BGRA, 10, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 10, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 10, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(BGR, 10, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(BGRA, 10, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 10, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 10, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(BGR, 10, YDbDr420)
#endif

#if SPARKYUV_FULL_CHANNELS
DECLARE_PX_TO_YDBDR_HWY(BGRA, 8, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 8, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 8, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(BGR, 8, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(BGRA, 8, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 8, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 8, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(BGR, 8, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(BGRA, 8, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 8, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 8, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(BGR, 8, YDbDr420)
#endif

#if SPARKYUV_FULL_CHANNELS
DECLARE_PX_TO_YDBDR_HWY(BGRA, 12, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 12, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 12, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(BGR, 12, YDbDr444)
DECLARE_PX_TO_YDBDR_HWY(BGRA, 12, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 12, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 12, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(BGR, 12, YDbDr422)
DECLARE_PX_TO_YDBDR_HWY(BGRA, 12, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(ABGR, 12, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(ARGB, 12, YDbDr420)
DECLARE_PX_TO_YDBDR_HWY(BGR, 12, YDbDr420)
#endif

#undef DECLARE_PX_TO_YDBDR_HWY

#define PIXEL_TO_YDBDR_E(T, PixelType, bit, yuvname) \
void PixelType##bit##To##yuvname##P##bit(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(PixelType##bit##To##yuvname##P##bit##HWY)(src, srcStride, width, height, \
          yPlane, yStride, uPlane, uStride, vPlane, vStride, colorRange);\
    }

PIXEL_TO_YDBDR_E(uint16_t, RGBA, 10, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, RGB, 10, YDbDr444)

PIXEL_TO_YDBDR_E(uint16_t, RGBA, 12, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, RGB, 12, YDbDr444)

PIXEL_TO_YDBDR_E(uint16_t, RGBA, 10, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, RGB, 10, YDbDr422)

PIXEL_TO_YDBDR_E(uint16_t, RGBA, 12, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, RGB, 12, YDbDr422)

PIXEL_TO_YDBDR_E(uint16_t, RGBA, 10, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, RGB, 10, YDbDr420)

PIXEL_TO_YDBDR_E(uint16_t, RGBA, 12, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, RGB, 12, YDbDr420)

PIXEL_TO_YDBDR_E(uint8_t, RGBA, 8, YDbDr420)
PIXEL_TO_YDBDR_E(uint8_t, RGB, 8, YDbDr420)

PIXEL_TO_YDBDR_E(uint8_t, RGBA, 8, YDbDr422)
PIXEL_TO_YDBDR_E(uint8_t, RGB, 8, YDbDr422)

PIXEL_TO_YDBDR_E(uint8_t, RGBA, 8, YDbDr444)
PIXEL_TO_YDBDR_E(uint8_t, RGB, 8, YDbDr444)

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_E(uint16_t, BGRA, 10, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, ABGR, 10, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, ARGB, 10, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, BGR, 10, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, BGRA, 10, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, ABGR, 10, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, ARGB, 10, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, BGR, 10, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, BGRA, 10, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, ABGR, 10, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, ARGB, 10, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, BGR, 10, YDbDr420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_E(uint8_t, BGRA, 8, YDbDr444)
PIXEL_TO_YDBDR_E(uint8_t, ABGR, 8, YDbDr444)
PIXEL_TO_YDBDR_E(uint8_t, ARGB, 8, YDbDr444)
PIXEL_TO_YDBDR_E(uint8_t, BGR, 8, YDbDr444)
PIXEL_TO_YDBDR_E(uint8_t, BGRA, 8, YDbDr422)
PIXEL_TO_YDBDR_E(uint8_t, ABGR, 8, YDbDr422)
PIXEL_TO_YDBDR_E(uint8_t, ARGB, 8, YDbDr422)
PIXEL_TO_YDBDR_E(uint8_t, BGR, 8, YDbDr422)
PIXEL_TO_YDBDR_E(uint8_t, BGRA, 8, YDbDr420)
PIXEL_TO_YDBDR_E(uint8_t, ABGR, 8, YDbDr420)
PIXEL_TO_YDBDR_E(uint8_t, ARGB, 8, YDbDr420)
PIXEL_TO_YDBDR_E(uint8_t, BGR, 8, YDbDr420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDBDR_E(uint16_t, BGRA, 12, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, ABGR, 12, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, ARGB, 12, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, BGR, 12, YDbDr444)
PIXEL_TO_YDBDR_E(uint16_t, BGRA, 12, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, ABGR, 12, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, ARGB, 12, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, BGR, 12, YDbDr422)
PIXEL_TO_YDBDR_E(uint16_t, BGRA, 12, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, ABGR, 12, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, ARGB, 12, YDbDr420)
PIXEL_TO_YDBDR_E(uint16_t, BGR, 12, YDbDr420)
#endif

#undef PIXEL_TO_YDBDR_E

#define DECLARE_YCgCo_TO_PX_HWY(pixel, bit, yuv) HWY_EXPORT(yuv##P##bit##To##pixel##bit##HWY);

DECLARE_YCgCo_TO_PX_HWY(RGBA, 10, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(RGB, 10, YDbDr444)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 12, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(RGB, 12, YDbDr444)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 10, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(RGB, 10, YDbDr422)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 12, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(RGB, 12, YDbDr422)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 10, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(RGB, 10, YDbDr420)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 12, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(RGB, 12, YDbDr420)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 8, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(RGB, 8, YDbDr420)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 8, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(RGB, 8, YDbDr422)

DECLARE_YCgCo_TO_PX_HWY(RGBA, 8, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(RGB, 8, YDbDr444)

#if SPARKYUV_FULL_CHANNELS
DECLARE_YCgCo_TO_PX_HWY(BGRA, 10, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 10, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 10, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(BGR, 10, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(BGRA, 10, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 10, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 10, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(BGR, 10, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(BGRA, 10, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 10, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 10, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(BGR, 10, YDbDr420)
#endif

#if SPARKYUV_FULL_CHANNELS
DECLARE_YCgCo_TO_PX_HWY(BGRA, 8, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 8, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 8, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(BGR, 8, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(BGRA, 8, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 8, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 8, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(BGR, 8, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(BGRA, 8, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 8, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 8, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(BGR, 8, YDbDr420)
#endif

#if SPARKYUV_FULL_CHANNELS
DECLARE_YCgCo_TO_PX_HWY(BGRA, 12, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 12, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 12, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(BGR, 12, YDbDr444)
DECLARE_YCgCo_TO_PX_HWY(BGRA, 12, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 12, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 12, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(BGR, 12, YDbDr422)
DECLARE_YCgCo_TO_PX_HWY(BGRA, 12, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(ABGR, 12, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(ARGB, 12, YDbDr420)
DECLARE_YCgCo_TO_PX_HWY(BGR, 12, YDbDr420)
#endif

#undef DECLARE_YCgCo_TO_PX_HWY

#define YCgCo_TO_PX_DECLARATION_E(T, pixel, bit, yuv) \
    void yuv##P##bit##To##pixel##bit(T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                             const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                             const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                             const SparkYuvColorRange colorRange) {\
      HWY_DYNAMIC_DISPATCH(yuv##P##bit##To##pixel##bit##HWY)(src, srcStride, width, height,\
                                                 yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                 colorRange);\
    }

YCgCo_TO_PX_DECLARATION_E(uint16_t, RGBA, 10, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGB, 10, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGBA, 10, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGB, 10, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGBA, 10, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGB, 10, YDbDr420)

#if SPARKYUV_FULL_CHANNELS
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGRA, 10, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ABGR, 10, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ARGB, 10, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGR, 10, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGRA, 10, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ABGR, 10, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ARGB, 10, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGR, 10, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGRA, 10, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ABGR, 10, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ARGB, 10, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGR, 10, YDbDr420)
#endif

YCgCo_TO_PX_DECLARATION_E(uint8_t, RGBA, 8, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint8_t, RGB, 8, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint8_t, RGBA, 8, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint8_t, RGB, 8, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint8_t, RGBA, 8, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint8_t, RGB, 8, YDbDr420)

#if SPARKYUV_FULL_CHANNELS
YCgCo_TO_PX_DECLARATION_E(uint8_t, BGRA, 8, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint8_t, ABGR, 8, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint8_t, ARGB, 8, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint8_t, BGR, 8, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint8_t, BGRA, 8, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint8_t, ABGR, 8, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint8_t, ARGB, 8, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint8_t, BGR, 8, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint8_t, BGRA, 8, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint8_t, ABGR, 8, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint8_t, ARGB, 8, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint8_t, BGR, 8, YDbDr420)
#endif

YCgCo_TO_PX_DECLARATION_E(uint16_t, RGBA, 12, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGB, 12, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGBA, 12, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGB, 12, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGBA, 12, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, RGB, 12, YDbDr420)

#if SPARKYUV_FULL_CHANNELS
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGRA, 12, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ABGR, 12, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ARGB, 12, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGR, 12, YDbDr444)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGRA, 12, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ABGR, 12, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ARGB, 12, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGR, 12, YDbDr422)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGRA, 12, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ABGR, 12, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, ARGB, 12, YDbDr420)
YCgCo_TO_PX_DECLARATION_E(uint16_t, BGR, 12, YDbDr420)
#endif

#undef YCgCo_TO_PX_DECLARATION_E

}
#endif