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
#define HWY_TARGET_INCLUDE "src/NV24Flyer.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "yuv-inl.h"
#include "NV24-inl.h"

#if HWY_ONCE
namespace sparkyuv {

#define NV24ToXXXX_DECLARATION_HWY(pixel, NV) HWY_EXPORT(NV##To##pixel##HWY);

NV24ToXXXX_DECLARATION_HWY(RGBA, NV42)
NV24ToXXXX_DECLARATION_HWY(RGB, NV42)
#if SPARKYUV_FULL_CHANNELS
NV24ToXXXX_DECLARATION_HWY(ARGB, NV42)
NV24ToXXXX_DECLARATION_HWY(ABGR, NV42)
NV24ToXXXX_DECLARATION_HWY(BGRA, NV42)
NV24ToXXXX_DECLARATION_HWY(BGR, NV42)
#endif

NV24ToXXXX_DECLARATION_HWY(RGBA, NV24)
NV24ToXXXX_DECLARATION_HWY(RGB, NV24)
#if SPARKYUV_FULL_CHANNELS
NV24ToXXXX_DECLARATION_HWY(ARGB, NV24)
NV24ToXXXX_DECLARATION_HWY(ABGR, NV24)
NV24ToXXXX_DECLARATION_HWY(BGRA, NV24)
NV24ToXXXX_DECLARATION_HWY(BGR, NV24)
#endif

#undef NV24ToXXXX_DECLARATION_HWY

// NV42

#define NV42ToXXXX_DECLARATION_E(pixelType, NV) \
    HWY_DLLEXPORT void \
    NV##To##pixelType(uint8_t *dst, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride, \
               const uint8_t *uv, uint32_t uvStride, const float kr, const float kb, const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(NV##To##pixelType##HWY)(dst, dstStride, width, height, ySrc, yStride, uv, uvStride, kr, kb, colorRange); \
    }

NV42ToXXXX_DECLARATION_E(RGBA, NV42)
NV42ToXXXX_DECLARATION_E(RGB, NV42)
#if SPARKYUV_FULL_CHANNELS
NV42ToXXXX_DECLARATION_E(ARGB, NV42)
NV42ToXXXX_DECLARATION_E(ABGR, NV42)
NV42ToXXXX_DECLARATION_E(BGRA, NV42)
NV42ToXXXX_DECLARATION_E(BGR, NV42)
#endif

NV42ToXXXX_DECLARATION_E(RGBA, NV24)
NV42ToXXXX_DECLARATION_E(RGB, NV24)
#if SPARKYUV_FULL_CHANNELS
NV42ToXXXX_DECLARATION_E(ARGB, NV24)
NV42ToXXXX_DECLARATION_E(ABGR, NV24)
NV42ToXXXX_DECLARATION_E(BGRA, NV24)
NV42ToXXXX_DECLARATION_E(BGR, NV24)
#endif

#undef NV42ToXXXX_DECLARATION

#define XXXToNVXX_DECLARATION_HWY(Pixel, NV) HWY_EXPORT(Pixel##To##NV##HWY);

XXXToNVXX_DECLARATION_HWY(RGBA, NV24)
XXXToNVXX_DECLARATION_HWY(RGB, NV24)
#if SPARKYUV_FULL_CHANNELS
XXXToNVXX_DECLARATION_HWY(ARGB, NV24)
XXXToNVXX_DECLARATION_HWY(ABGR, NV24)
XXXToNVXX_DECLARATION_HWY(BGRA, NV24)
XXXToNVXX_DECLARATION_HWY(BGR, NV24)
#endif

XXXToNVXX_DECLARATION_HWY(RGBA, NV42)
XXXToNVXX_DECLARATION_HWY(RGB, NV42)
#if SPARKYUV_FULL_CHANNELS
XXXToNVXX_DECLARATION_HWY(ARGB, NV42)
XXXToNVXX_DECLARATION_HWY(ABGR, NV42)
XXXToNVXX_DECLARATION_HWY(BGRA, NV42)
XXXToNVXX_DECLARATION_HWY(BGR, NV42)
#endif

#undef NV24ToXXXX_DECLARATION_HWY

#define XXXXToNVXX_DECLARATION_E(Pixel, NV) \
    HWY_DLLEXPORT void \
    Pixel##To##NV(const uint8_t *dst, uint32_t dstStride, uint32_t width, uint32_t height, \
                  uint8_t *ySrc, uint32_t yStride, uint8_t *uv, uint32_t uvStride,         \
                  const float kr, const float kb, const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(Pixel##To##NV##HWY)(dst, dstStride, width, height,              \
                                               ySrc, yStride, uv, uvStride, kr, kb, colorRange); \
    }

XXXXToNVXX_DECLARATION_E(RGBA, NV24)
XXXXToNVXX_DECLARATION_E(RGB, NV24)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_E(ARGB, NV24)
XXXXToNVXX_DECLARATION_E(ABGR, NV24)
XXXXToNVXX_DECLARATION_E(BGRA, NV24)
XXXXToNVXX_DECLARATION_E(BGR, NV24)
#endif

XXXXToNVXX_DECLARATION_E(RGBA, NV42)
XXXXToNVXX_DECLARATION_E(RGB, NV42)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_E(ARGB, NV42)
XXXXToNVXX_DECLARATION_E(ABGR, NV42)
XXXXToNVXX_DECLARATION_E(BGRA, NV42)
XXXXToNVXX_DECLARATION_E(BGR, NV42)
#endif

#undef XXXXToNVXX_DECLARATION_E
}
#endif