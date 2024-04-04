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

#pragma once

namespace sparkyuv {
#define REFORMAT_TO_RGB565_NBIT(PixelName, Pixel, T) \
    void PixelName##ToRGB565(const T * src, uint32_t srcStride,\
                             uint16_t * dst, uint32_t dstStride,\
                             uint32_t width, uint32_t height);

REFORMAT_TO_RGB565_NBIT(RGB, RGB, uint8_t)
REFORMAT_TO_RGB565_NBIT(BGR, BGR, uint8_t)
REFORMAT_TO_RGB565_NBIT(RGBA, RGBA, uint8_t)
REFORMAT_TO_RGB565_NBIT(BGRA, BGRA, uint8_t)
REFORMAT_TO_RGB565_NBIT(ABGR, ABGR, uint8_t)
REFORMAT_TO_RGB565_NBIT(ARGB, ARGB, uint8_t)
REFORMAT_TO_RGB565_NBIT(RGBA1010102, RGBA1010102, uint8_t)

REFORMAT_TO_RGB565_NBIT(RGBF16, RGB, uint16_t)
REFORMAT_TO_RGB565_NBIT(BGRF16, BGR, uint16_t)
REFORMAT_TO_RGB565_NBIT(RGBAF16, RGBA, uint16_t)
REFORMAT_TO_RGB565_NBIT(BGRAF16, BGRA, uint16_t)
REFORMAT_TO_RGB565_NBIT(ABGRF16, ABGR, uint16_t)
REFORMAT_TO_RGB565_NBIT(ARGBF16, ARGB, uint16_t)

#undef REFORMAT_TO_RGB565

#define REFORMAT_TO_RGB565(PixelName, Pixel, T) \
    void PixelName##ToRGB565(const T * src, const uint32_t srcStride,\
                                uint16_t * dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height, const int bitDepth);

REFORMAT_TO_RGB565(RGB16, RGB, uint16_t)
REFORMAT_TO_RGB565(BGR16, BGR,uint16_t)
REFORMAT_TO_RGB565(RGBA16, RGBA, uint16_t)
REFORMAT_TO_RGB565(BGRA16, BGRA, uint16_t)
REFORMAT_TO_RGB565(ABGR16, ABGR, uint16_t)
REFORMAT_TO_RGB565(ARGB16, ARGB, uint16_t)

#undef REFORMAT_TO_RGB565

#define REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(PixelName, Pixel, T) \
    void RGB565To##PixelName(const uint16_t * src, const uint32_t srcStride,\
                             T * dst, const uint32_t dstStride,\
                             const uint32_t width, const uint32_t height);

REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(RGB, RGB, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(BGR, BGR, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(RGBA, RGBA, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(BGRA, BGRA, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(ABGR, ABGR, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(ARGB, ARGB, uint8_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(RGBA1010102, RGBA1010102, uint8_t)

REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(RGBF16, RGB, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(BGRF16, BGR, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(RGBAF16, RGBA, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(BGRAF16, BGRA, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(ABGRF16, ABGR, uint16_t)
REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H(ARGBF16, ARGB, uint16_t)

#undef REFORMAT_RGB565_TO_NEW_BIT_DEFINED_H

#define REFORMAT_RGB565_TO_NEW_H(PixelName, Pixel, T) \
    void RGB565To##PixelName(const uint16_t * src, const uint32_t srcStride,\
                             T * dst, const uint32_t dstStride,\
                             const uint32_t width, const uint32_t height, const int bitDepth);

REFORMAT_RGB565_TO_NEW_H(RGB, RGB, uint8_t)
REFORMAT_RGB565_TO_NEW_H(BGR, BGR, uint8_t)
REFORMAT_RGB565_TO_NEW_H(RGBA, RGBA, uint8_t)
REFORMAT_RGB565_TO_NEW_H(BGRA, BGRA, uint8_t)
REFORMAT_RGB565_TO_NEW_H(ABGR, ABGR, uint8_t)
REFORMAT_RGB565_TO_NEW_H(ARGB, ARGB, uint8_t)
REFORMAT_RGB565_TO_NEW_H(RGBA1010102, RGBA1010102, uint8_t)

REFORMAT_RGB565_TO_NEW_H(RGB16, RGB, uint16_t)
REFORMAT_RGB565_TO_NEW_H(BGR16, BGR,uint16_t)
REFORMAT_RGB565_TO_NEW_H(RGBA16, RGBA, uint16_t)
REFORMAT_RGB565_TO_NEW_H(BGRA16, BGRA, uint16_t)
REFORMAT_RGB565_TO_NEW_H(ABGR16, ABGR, uint16_t)
REFORMAT_RGB565_TO_NEW_H(ARGB16, ARGB, uint16_t)
}