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

void RGBToRGB565(const uint8_t *src, uint32_t srcStride,
                 uint16_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height);
void BGRToRGB565(const uint8_t *src, uint32_t srcStride,
                 uint16_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height);
void RGBAToRGB565(const uint8_t *src, uint32_t srcStride,
                  uint16_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void BGRAToRGB565(const uint8_t *src, uint32_t srcStride,
                  uint16_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void ABGRToRGB565(const uint8_t *src, uint32_t srcStride,
                  uint16_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void ARGBToRGB565(const uint8_t *src, uint32_t srcStride,
                  uint16_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void RGBA1010102ToRGB565(const uint8_t *src, uint32_t srcStride,
                         uint16_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height);

void RGBF16ToRGB565(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height);
void BGRF16ToRGB565(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height);
void RGBAF16ToRGB565(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void BGRAF16ToRGB565(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void ABGRF16ToRGB565(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void ARGBF16ToRGB565(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void RGB16ToRGB565(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int bitDepth);
void BGR16ToRGB565(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int bitDepth);
void RGBA16ToRGB565(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);
void BGRA16ToRGB565(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);
void ABGR16ToRGB565(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);
void ARGB16ToRGB565(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);

void RGB565ToRGB(const uint16_t *src, uint32_t srcStride,
                 uint8_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height);
void RGB565ToBGR(const uint16_t *src, uint32_t srcStride,
                 uint8_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height);
void RGB565ToRGBA(const uint16_t *src, uint32_t srcStride,
                  uint8_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void RGB565ToBGRA(const uint16_t *src, uint32_t srcStride,
                  uint8_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void RGB565ToABGR(const uint16_t *src, uint32_t srcStride,
                  uint8_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void RGB565ToARGB(const uint16_t *src, uint32_t srcStride,
                  uint8_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height);
void RGB565ToRGBA1010102(const uint16_t *src, uint32_t srcStride,
                         uint8_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height);

void RGB565ToRGBF16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height);
void RGB565ToBGRF16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height);
void RGB565ToRGBAF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void RGB565ToBGRAF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void RGB565ToABGRF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void RGB565ToARGBF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void RGB565ToRGB16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int bitDepth);
void RGB565ToBGR16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int bitDepth);
void RGB565ToRGBA16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int bitDepth);
void RGB565ToBGRA16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);
void RGB565ToABGR16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);
void RGB565ToARGB16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);
}