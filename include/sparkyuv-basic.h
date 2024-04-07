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

#ifndef YUV_INCLUDE_SPARKYUV_BASIC_H_
#define YUV_INCLUDE_SPARKYUV_BASIC_H_

namespace sparkyuv {
#define WideXXXXFrom8ToN_DECLARATION_H(pixelType, targetBitDepth) \
   void Wide##pixelType##8To##targetBitDepth(uint8_t* src, const uint32_t srcStride, \
                                             uint16_t* dst, const uint32_t dstStride,\
                                             const uint32_t width, const uint32_t height);

WideXXXXFrom8ToN_DECLARATION_H(RGBA, 10)
WideXXXXFrom8ToN_DECLARATION_H(RGB, 10)
#if SPARKYUV_FULL_CHANNELS
WideXXXXFrom8ToN_DECLARATION_H(BGRA, 10)
WideXXXXFrom8ToN_DECLARATION_H(ABGR, 10)
WideXXXXFrom8ToN_DECLARATION_H(ARGB, 10)
WideXXXXFrom8ToN_DECLARATION_H(BGR, 10)
#endif

WideXXXXFrom8ToN_DECLARATION_H(RGBA, 12)
WideXXXXFrom8ToN_DECLARATION_H(RGB, 12)
#if SPARKYUV_FULL_CHANNELS
WideXXXXFrom8ToN_DECLARATION_H(BGRA, 12)
WideXXXXFrom8ToN_DECLARATION_H(ABGR, 12)
WideXXXXFrom8ToN_DECLARATION_H(ARGB, 12)
WideXXXXFrom8ToN_DECLARATION_H(BGR, 12)
#endif

WideXXXXFrom8ToN_DECLARATION_H(RGBA, 16)
WideXXXXFrom8ToN_DECLARATION_H(RGB, 16)
#if SPARKYUV_FULL_CHANNELS
WideXXXXFrom8ToN_DECLARATION_H(BGRA, 16)
WideXXXXFrom8ToN_DECLARATION_H(ABGR, 16)
WideXXXXFrom8ToN_DECLARATION_H(ARGB, 16)
WideXXXXFrom8ToN_DECLARATION_H(BGR, 16)
#endif

#undef WideXXXXFrom8ToN_DECLARATION_H

#define SaturateXXXXFrom8ToN_DECLARATION_H(pixelType, sourceBitDepth) \
   void Saturate##pixelType##sourceBitDepth##To8(const uint16_t* src, const uint32_t srcStride, \
                                                 uint8_t* dst, const uint32_t dstStride,\
                                                 const uint32_t width, const uint32_t height);

SaturateXXXXFrom8ToN_DECLARATION_H(RGBA, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(RGB, 10)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFrom8ToN_DECLARATION_H(BGRA, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(ABGR, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(ARGB, 10)
SaturateXXXXFrom8ToN_DECLARATION_H(BGR, 10)
#endif

SaturateXXXXFrom8ToN_DECLARATION_H(RGBA, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(RGB, 12)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFrom8ToN_DECLARATION_H(BGRA, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(ABGR, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(ARGB, 12)
SaturateXXXXFrom8ToN_DECLARATION_H(BGR, 12)
#endif

SaturateXXXXFrom8ToN_DECLARATION_H(RGBA, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(RGB, 16)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFrom8ToN_DECLARATION_H(BGRA, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(ABGR, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(ARGB, 16)
SaturateXXXXFrom8ToN_DECLARATION_H(BGR, 16)
#endif

#undef SaturateXXXXFrom8ToN_DECLARATION_H

#define WIDE8_XXXX_DECLARATION_H(pixelType) \
 void Wide##pixelType##8(uint8_t * src, const uint32_t srcStride, \
                         uint16_t * dst, const uint32_t dstStride,\
                         const uint32_t width, const uint32_t height, const int bitDepth);

WIDE8_XXXX_DECLARATION_H(RGBA)
WIDE8_XXXX_DECLARATION_H(RGB)
#if SPARKYUV_FULL_CHANNELS
WIDE8_XXXX_DECLARATION_H(BGRA)
WIDE8_XXXX_DECLARATION_H(ABGR)
WIDE8_XXXX_DECLARATION_H(ARGB)
WIDE8_XXXX_DECLARATION_H(BGR)
#endif

#undef WIDE8_XXXX_DECLARATION_H

#define SATURATE_XXXX16_DYNAMIC_H(pixelType) \
  void Saturate##pixelType##To8(const uint16_t * src, const uint32_t srcStride, \
                                    uint8_t * dst, const uint32_t dstStride,\
                                    const uint32_t width, const uint32_t height, const int bitDepth);

SATURATE_XXXX16_DYNAMIC_H(RGBA)
SATURATE_XXXX16_DYNAMIC_H(RGB)
#if SPARKYUV_FULL_CHANNELS
SATURATE_XXXX16_DYNAMIC_H(BGR)
SATURATE_XXXX16_DYNAMIC_H(BGRA)
SATURATE_XXXX16_DYNAMIC_H(ABGR)
SATURATE_XXXX16_DYNAMIC_H(ARGB)
#endif

#undef SATURATE_XXXX16_DYNAMIC_E
}

#endif //YUV_INCLUDE_SPARKYUV_BASIC_H_
