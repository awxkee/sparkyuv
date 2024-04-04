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
#define HWY_TARGET_INCLUDE "src/NV12Flyer.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "hwy/cache_control.h"
#include "yuv-inl.h"
#include "NV12-inl.h"

#if HWY_ONCE
namespace sparkyuv {
#define NV21ToXXXX_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void \
    NV21To##pixelType(uint8_t *dst, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride, \
               const uint8_t *uv, uint32_t uvStride, const float kr, const float kb, const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(NV21To##pixelType##HWY)(dst, dstStride, width, height, ySrc, yStride, uv, uvStride, kr, kb, colorRange); \
    }

#define NV21ToXXXX_DECLARATION_HWY(pixel) HWY_EXPORT(NV21To##pixel##HWY);

NV21ToXXXX_DECLARATION_HWY(RGBA)
NV21ToXXXX_DECLARATION_HWY(RGB)
#if SPARKYUV_FULL_CHANNELS
NV21ToXXXX_DECLARATION_HWY(ARGB)
NV21ToXXXX_DECLARATION_HWY(ABGR)
NV21ToXXXX_DECLARATION_HWY(BGRA)
NV21ToXXXX_DECLARATION_HWY(BGR)
#endif

#undef NV21ToXXXX_DECLARATION_HWY

NV21ToXXXX_DECLARATION_E(RGBA)
NV21ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
NV21ToXXXX_DECLARATION_E(ARGB)
NV21ToXXXX_DECLARATION_E(ABGR)
NV21ToXXXX_DECLARATION_E(BGRA)
NV21ToXXXX_DECLARATION_E(BGR)
#endif

#undef NV21ToXXXX_DECLARATION

// NV12

#define NV12ToXXXX_DECLARATION_E(pixelType) \
    HWY_DLLEXPORT void \
    NV12To##pixelType(uint8_t *dst, uint32_t dstStride, uint32_t width, uint32_t height, const uint8_t *ySrc, uint32_t yStride, \
               const uint8_t *uv, uint32_t uvStride, const float kr, const float kb, const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(NV12To##pixelType##HWY)(dst, dstStride, width, height, ySrc, yStride, uv, uvStride, kr, kb, colorRange); \
    }

#define NV12ToXXXX_DECLARATION_HWY(pixel) HWY_EXPORT(NV12To##pixel##HWY);

NV12ToXXXX_DECLARATION_HWY(RGBA)
NV12ToXXXX_DECLARATION_HWY(RGB)
#if SPARKYUV_FULL_CHANNELS
NV12ToXXXX_DECLARATION_HWY(ARGB)
NV12ToXXXX_DECLARATION_HWY(ABGR)
NV12ToXXXX_DECLARATION_HWY(BGRA)
NV12ToXXXX_DECLARATION_HWY(BGR)
#endif

#undef NV12ToXXXX_DECLARATION_HWY

NV12ToXXXX_DECLARATION_E(RGBA)
NV12ToXXXX_DECLARATION_E(RGB)
#if SPARKYUV_FULL_CHANNELS
NV12ToXXXX_DECLARATION_E(ARGB)
NV12ToXXXX_DECLARATION_E(ABGR)
NV12ToXXXX_DECLARATION_E(BGRA)
NV12ToXXXX_DECLARATION_E(BGR)
#endif

#undef NV12ToXXXX_DECLARATION

#define XXXToNVXX_DECLARATION_HWY(Pixel, NV) HWY_EXPORT(Pixel##To##NV##HWY);

XXXToNVXX_DECLARATION_HWY(RGBA, NV21)
XXXToNVXX_DECLARATION_HWY(RGB, NV21)
#if SPARKYUV_FULL_CHANNELS
XXXToNVXX_DECLARATION_HWY(ARGB, NV21)
XXXToNVXX_DECLARATION_HWY(ABGR, NV21)
XXXToNVXX_DECLARATION_HWY(BGRA, NV21)
XXXToNVXX_DECLARATION_HWY(BGR, NV21)
#endif

XXXToNVXX_DECLARATION_HWY(RGBA, NV12)
XXXToNVXX_DECLARATION_HWY(RGB, NV12)
#if SPARKYUV_FULL_CHANNELS
XXXToNVXX_DECLARATION_HWY(ARGB, NV12)
XXXToNVXX_DECLARATION_HWY(ABGR, NV12)
XXXToNVXX_DECLARATION_HWY(BGRA, NV12)
XXXToNVXX_DECLARATION_HWY(BGR, NV12)
#endif

#undef NV21ToXXXX_DECLARATION_HWY

#define XXXXToNVXX_DECLARATION_E(Pixel, NV) \
    HWY_DLLEXPORT void \
    Pixel##To##NV(const uint8_t *dst, uint32_t dstStride, uint32_t width, uint32_t height, uint8_t *ySrc, uint32_t yStride, \
                  uint8_t *uv, uint32_t uvStride, const float kr, const float kb, const SparkYuvColorRange colorRange) { \
      HWY_DYNAMIC_DISPATCH(Pixel##To##NV##HWY)(dst, dstStride, width, height, ySrc, yStride, uv, uvStride, kr, kb, colorRange); \
    }

XXXXToNVXX_DECLARATION_E(RGBA, NV21)
XXXXToNVXX_DECLARATION_E(RGB, NV21)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_E(ARGB, NV21)
XXXXToNVXX_DECLARATION_E(ABGR, NV21)
XXXXToNVXX_DECLARATION_E(BGRA, NV21)
XXXXToNVXX_DECLARATION_E(BGR, NV21)
#endif

XXXXToNVXX_DECLARATION_E(RGBA, NV12)
XXXXToNVXX_DECLARATION_E(RGB, NV12)
#if SPARKYUV_FULL_CHANNELS
XXXXToNVXX_DECLARATION_E(ARGB, NV12)
XXXXToNVXX_DECLARATION_E(ABGR, NV12)
XXXXToNVXX_DECLARATION_E(BGRA, NV12)
XXXXToNVXX_DECLARATION_E(BGR, NV12)
#endif

#undef XXXXToNVXX_DECLARATION_E

}
#endif