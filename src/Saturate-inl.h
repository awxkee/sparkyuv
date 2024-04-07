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

#if defined(SPARKYUV_SATURATE_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_SATURATE_INL_H
#undef SPARKYUV_SATURATE_INL_H
#else
#define SPARKYUV_SATURATE_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

/**
 * @brief Compiler will generate for each combination best option for the given bit depth and pixel type,
 * when dynamic bit depth is provided it affects performance a little
 */
template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, int sourceBitDepth>
void SaturateSurfaceFromNBitTo8(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                                uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                                const uint32_t width, const uint32_t height) {
  const ScalableTag<uint16_t> du16;
  const Rebind<uint8_t, decltype(du16)> du8;
  using V8 = Vec<decltype(du8)>;
  using V16 = Vec<decltype(du16)>;

  const int lanes = Lanes(du8);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const int diff = sourceBitDepth - 8;

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  for (int y = 0; y < height; ++y) {
    auto store = reinterpret_cast<uint8_t *>(mDestination);
    auto source = reinterpret_cast<const uint16_t *>(mSource);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      V16 R;
      V16 G;
      V16 B;
      V16 A;
      LoadRGBA<PixelType>(du16, source, R, G, B, A);

      V8 R8, G8, B8, A8;

      if (diff == 2) {
        R8 = ShiftRightDemote<2>(du16, R);
        G8 = ShiftRightDemote<2>(du16, G);
        B8 = ShiftRightDemote<2>(du16, B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A8 = ShiftRightDemote<2>(du16, A);
        }
      } else if (diff == 4) {
        R8 = ShiftRightDemote<4>(du16, R);
        G8 = ShiftRightDemote<4>(du16, G);
        B8 = ShiftRightDemote<4>(du16, B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A8 = ShiftRightDemote<4>(du16, A);
        }
      } else if (diff == 8) {
        R8 = ShiftRightDemote<8>(du16, R);
        G8 = ShiftRightDemote<8>(du16, G);
        B8 = ShiftRightDemote<8>(du16, B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A8 = ShiftRightDemote<8>(du16, A);
        }
      } else {
        R8 = DemoteTo(du8, ShiftRightSame(R, diff));
        G8 = DemoteTo(du8, ShiftRightSame(G, diff));
        B8 = DemoteTo(du8, ShiftRightSame(B, diff));
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A8 = DemoteTo(du8, ShiftRightSame(A, diff));
        }
      }

      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        A8 = DemoteTo(du8, A);
      }

      StoreRGBA<PixelType>(du8, store, R8, G8, B8, A8);

      store += lanes * components;
      source += lanes * components;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;
      uint16_t a;

      LoadRGBA<uint16_t, uint16_t, PixelType>(source, r, g, b, a);

      r = r >> diff;
      g = g >> diff;
      b = b >> diff;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        a = a >> diff;
      }

      StoreRGBA<uint8_t, uint16_t, PixelType>(store, r, g, b, a);

      store += components;
      source += components;
    }

    mDestination += dstStride;
    mSource += srcStride;
  }
}

/**
* @brief Dynamic version slower than fixed
*/
template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void SaturateSurface16To8(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                          uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                          const uint32_t width, const uint32_t height, const int sourceBitDepth) {
  const ScalableTag<uint16_t> du16;
  const Rebind<uint8_t, decltype(du16)> du8;
  using V8 = Vec<decltype(du8)>;
  using V16 = Vec<decltype(du16)>;

  const int lanes = Lanes(du8);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const int diff = sourceBitDepth - 8;

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  for (int y = 0; y < height; ++y) {
    auto store = reinterpret_cast<uint8_t *>(mDestination);
    auto source = reinterpret_cast<const uint16_t *>(mSource);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      V16 R;
      V16 G;
      V16 B;
      V16 A;
      LoadRGBA<PixelType>(du16, source, R, G, B, A);

      R = ShiftRightSame(R, diff);
      G = ShiftRightSame(G, diff);
      B = ShiftRightSame(B, diff);
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        A = ShiftRightSame(A, diff);
      }

      const V8 R8 = DemoteTo(du8, R);
      const V8 G8 = DemoteTo(du8, G);
      const V8 B8 = DemoteTo(du8, B);
      V8 A8;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        A8 = DemoteTo(du8, A);
      }

      StoreRGBA<PixelType>(du8, store, R8, G8, B8, A8);

      store += lanes * components;
      source += lanes * components;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;
      uint16_t a;

      LoadRGBA<uint16_t, uint16_t, PixelType>(source, r, g, b, a);

      r = r >> diff;
      g = g >> diff;
      b = b >> diff;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        a = a >> diff;
      }

      StoreRGBA<uint8_t, uint16_t, PixelType>(store, r, g, b, a);

      store += components;
      source += components;
    }

    mDestination += dstStride;
    mSource += srcStride;
  }
}

#define SaturateXXXXFromNTo8_DECLARATION_R(pixelType, sourceBitDepth) \
    void Saturate##pixelType##sourceBitDepth##To8HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                     uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                     const uint32_t width, const uint32_t height) {\
        SaturateSurfaceFromNBitTo8<sparkyuv::PIXEL_##pixelType, sourceBitDepth>(src, srcStride, dst, dstStride,\
                                                                                width, height); \
    }

SaturateXXXXFromNTo8_DECLARATION_R(RGBA, 10)
SaturateXXXXFromNTo8_DECLARATION_R(RGB, 10)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFromNTo8_DECLARATION_R(ARGB, 10)
SaturateXXXXFromNTo8_DECLARATION_R(ABGR, 10)
SaturateXXXXFromNTo8_DECLARATION_R(BGRA, 10)
SaturateXXXXFromNTo8_DECLARATION_R(BGR, 10)
#endif

SaturateXXXXFromNTo8_DECLARATION_R(RGBA, 12)
SaturateXXXXFromNTo8_DECLARATION_R(RGB, 12)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFromNTo8_DECLARATION_R(ARGB, 12)
SaturateXXXXFromNTo8_DECLARATION_R(ABGR, 12)
SaturateXXXXFromNTo8_DECLARATION_R(BGRA, 12)
SaturateXXXXFromNTo8_DECLARATION_R(BGR, 12)
#endif

SaturateXXXXFromNTo8_DECLARATION_R(RGBA, 16)
SaturateXXXXFromNTo8_DECLARATION_R(RGB, 16)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXXFromNTo8_DECLARATION_R(ARGB, 16)
SaturateXXXXFromNTo8_DECLARATION_R(ABGR, 16)
SaturateXXXXFromNTo8_DECLARATION_R(BGRA, 16)
SaturateXXXXFromNTo8_DECLARATION_R(BGR, 16)
#endif

#undef SaturateXXXXFromNTo8_DECLARATION_R

#define SaturateXXXX16_T08_DECLARATION_R(pixelType) \
    void Saturate##pixelType##16##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                     uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                     const uint32_t width, const uint32_t height, const int sourceBitDepth) {\
        SaturateSurface16To8<sparkyuv::PIXEL_##pixelType>(src, srcStride, dst, dstStride,\
                                                           width, height, sourceBitDepth); \
    }

SaturateXXXX16_T08_DECLARATION_R(RGBA)
SaturateXXXX16_T08_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
SaturateXXXX16_T08_DECLARATION_R(ARGB)
SaturateXXXX16_T08_DECLARATION_R(ABGR)
SaturateXXXX16_T08_DECLARATION_R(BGRA)
SaturateXXXX16_T08_DECLARATION_R(BGR)
#endif

#undef SaturateXXXX16_T08_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif