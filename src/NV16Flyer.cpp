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
#define HWY_TARGET_INCLUDE "src/NV16Flyer.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "yuv-inl.h"
#include "NV16-inl.h"

#if HWY_ONCE
namespace sparkyuv {

#define NV16ToXXXX_DECLARATION_HWY(pixel, NV) HWY_EXPORT(NV##To##pixel##HWY);

NV16ToXXXX_DECLARATION_HWY(RGBA, NV61)
NV16ToXXXX_DECLARATION_HWY(RGB, NV61)
#if SPARKYUV_FULL_CHANNELS
NV16ToXXXX_DECLARATION_HWY(ARGB, NV61)
NV16ToXXXX_DECLARATION_HWY(ABGR, NV61)
NV16ToXXXX_DECLARATION_HWY(BGRA, NV61)
NV16ToXXXX_DECLARATION_HWY(BGR, NV61)
#endif

NV16ToXXXX_DECLARATION_HWY(RGBA, NV16)
NV16ToXXXX_DECLARATION_HWY(RGB, NV16)
#if SPARKYUV_FULL_CHANNELS
NV16ToXXXX_DECLARATION_HWY(ARGB, NV16)
NV16ToXXXX_DECLARATION_HWY(ABGR, NV16)
NV16ToXXXX_DECLARATION_HWY(BGRA, NV16)
NV16ToXXXX_DECLARATION_HWY(BGR, NV16)
#endif

#undef NV16ToXXXX_DECLARATION_HWY

// NV61

#define NV61ToXXXX_DECLARATION_E(pixelType, NV) \
    HWY_DLLEXPORT void \
    NV##To##pixelType(uint8_t *dst, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride, \
               const uint8_t *uv, uint32_t uvStride, const float kr, const float kb, const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(NV##To##pixelType##HWY)(dst, dstStride, width, height, ySrc, yStride, uv, uvStride, kr, kb, colorRange); \
    }

NV61ToXXXX_DECLARATION_E(RGBA, NV61)
NV61ToXXXX_DECLARATION_E(RGB, NV61)
#if SPARKYUV_FULL_CHANNELS
NV61ToXXXX_DECLARATION_E(ARGB, NV61)
NV61ToXXXX_DECLARATION_E(ABGR, NV61)
NV61ToXXXX_DECLARATION_E(BGRA, NV61)
NV61ToXXXX_DECLARATION_E(BGR, NV61)
#endif

NV61ToXXXX_DECLARATION_E(RGBA, NV16)
NV61ToXXXX_DECLARATION_E(RGB, NV16)
#if SPARKYUV_FULL_CHANNELS
NV61ToXXXX_DECLARATION_E(ARGB, NV16)
NV61ToXXXX_DECLARATION_E(ABGR, NV16)
NV61ToXXXX_DECLARATION_E(BGRA, NV16)
NV61ToXXXX_DECLARATION_E(BGR, NV16)
#endif

#undef NV61ToXXXX_DECLARATION

#define XXXToNVXX_DECLARATION_HWY(Pixel, NV) HWY_EXPORT(Pixel##To##NV##HWY);

XXXToNVXX_DECLARATION_HWY(RGBA, NV16)
XXXToNVXX_DECLARATION_HWY(RGB, NV16)
#if SPARKYUV_FULL_CHANNELS
XXXToNVXX_DECLARATION_HWY(ARGB, NV16)
XXXToNVXX_DECLARATION_HWY(ABGR, NV16)
XXXToNVXX_DECLARATION_HWY(BGRA, NV16)
XXXToNVXX_DECLARATION_HWY(BGR, NV16)
#endif

XXXToNVXX_DECLARATION_HWY(RGBA, NV61)
XXXToNVXX_DECLARATION_HWY(RGB, NV61)
#if SPARKYUV_FULL_CHANNELS
XXXToNVXX_DECLARATION_HWY(ARGB, NV61)
XXXToNVXX_DECLARATION_HWY(ABGR, NV61)
XXXToNVXX_DECLARATION_HWY(BGRA, NV61)
XXXToNVXX_DECLARATION_HWY(BGR, NV61)
#endif

#undef NV16ToXXXX_DECLARATION_HWY

#define XXXXToNVXX_DECLARATION_E(Pixel, NV) \
    HWY_DLLEXPORT void \
    Pixel##To##NV(const uint8_t *dst, uint32_t dstStride, uint32_t width, uint32_t height, uint8_t *ySrc, uint32_t yStride, \
                  uint8_t *uv, uint32_t uvStride, const float kr, const float kb, const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(Pixel##To##NV##HWY)(dst, dstStride, width, height, ySrc, yStride, uv, uvStride, kr, kb, colorRange); \
    }

XXXXToNVXX_DECLARATION_E(RGBA, NV16)
XXXXToNVXX_DECLARATION_E(RGB, NV16)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_E(ARGB, NV16)
XXXXToNVXX_DECLARATION_E(ABGR, NV16)
XXXXToNVXX_DECLARATION_E(BGRA, NV16)
XXXXToNVXX_DECLARATION_E(BGR, NV16)
#endif

XXXXToNVXX_DECLARATION_E(RGBA, NV61)
XXXXToNVXX_DECLARATION_E(RGB, NV61)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_E(ARGB, NV61)
XXXXToNVXX_DECLARATION_E(ABGR, NV61)
XXXXToNVXX_DECLARATION_E(BGRA, NV61)
XXXXToNVXX_DECLARATION_E(BGR, NV61)
#endif

#undef XXXXToNVXX_DECLARATION_E
}
#endif