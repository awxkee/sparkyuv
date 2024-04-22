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

#if defined(SPARKYUV_BALPHA_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_BALPHA_INL_H
#undef SPARKYUV_BALPHA_INL_H
#else
#define SPARKYUV_BALPHA_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include "math/fast_math-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

using namespace hwy;

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void
PremultiplyAlphaPixel8(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                       uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                       const uint32_t width, const uint32_t height) {
  static_assert(
      PixelType == PIXEL_RGBA || PixelType == PIXEL_ARGB || PixelType == PIXEL_ABGR || PixelType == PIXEL_BGRA,
      "Cannot be used on image without alpha");

  const ScalableTag<uint8_t> du8;
  const Half<decltype(du8)> dhu8;
  const Rebind<uint16_t, decltype(dhu8)> du16;
  using V8 = Vec<decltype(du8)>;

  const int lanes = Lanes(du8);

  const int components = getPixelTypeComponents(PixelType);

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  const auto vExpand = Set(du16, 255);

  for (int y = 0; y < height; ++y) {
    auto store = reinterpret_cast<uint8_t *>(mDestination);
    auto source = reinterpret_cast<const uint8_t *>(mSource);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      V8 R;
      V8 G;
      V8 B;
      V8 A;
      LoadRGBA<PixelType>(du8, source, R, G, B, A);

      const auto Rh = ShiftRightNarrow<8>(du16, SaturatedAdd(WidenMulHigh(du8, R, A), vExpand));
      const auto Gh = ShiftRightNarrow<8>(du16, SaturatedAdd(WidenMulHigh(du8, G, A), vExpand));
      const auto Bh = ShiftRightNarrow<8>(du16, SaturatedAdd(WidenMulHigh(du8, B, A), vExpand));

      auto Alow = LowerHalf(A);

      const auto Rl = ShiftRightNarrow<8>(du16, SaturatedAdd(WidenMul(dhu8, LowerHalf(R), Alow), vExpand));
      const auto Gl = ShiftRightNarrow<8>(du16, SaturatedAdd(WidenMul(dhu8, LowerHalf(G), Alow), vExpand));
      const auto Bl = ShiftRightNarrow<8>(du16, SaturatedAdd(WidenMul(dhu8, LowerHalf(B), Alow), vExpand));

      R = Combine(du8, Rh, Rl);
      G = Combine(du8, Gh, Gl);
      B = Combine(du8, Bh, Bl);

      StoreRGBA<PixelType>(du8, store, R, G, B, A);

      store += lanes * components;
      source += lanes * components;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;
      uint16_t a;

      LoadRGBA<uint8_t, uint16_t, PixelType>(source, r, g, b, a);

      r = (r * a + 255) >> 8;
      g = (g * a + 255) >> 8;
      b = (b * a + 255) >> 8;

      StoreRGBA<uint8_t, uint16_t, PixelType>(store, r, g, b, a);

      store += components;
      source += components;
    }

    mDestination += dstStride;
    mSource += srcStride;
  }
}

#define PREMULTIPLY_ALPHA_DECLARATION_R(pixelType) \
    void pixelType##PremultiplyAlphaHWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                        uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height) {\
        PremultiplyAlphaPixel8<sparkyuv::PIXEL_##pixelType>(src, srcStride, dst, dstStride, width, height); \
    }

PREMULTIPLY_ALPHA_DECLARATION_R(RGBA)
PREMULTIPLY_ALPHA_DECLARATION_R(ARGB)
PREMULTIPLY_ALPHA_DECLARATION_R(ABGR)
PREMULTIPLY_ALPHA_DECLARATION_R(BGRA)

#undef PREMULTIPLY_ALPHA_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void
UnpremultiplyAlpha8HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                       uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                       const uint32_t width, const uint32_t height) {
  static_assert(
      PixelType == PIXEL_RGBA || PixelType == PIXEL_ARGB || PixelType == PIXEL_ABGR || PixelType == PIXEL_BGRA,
      "Cannot be used on image without alpha");

  const int components = getPixelTypeComponents(PixelType);

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  const ScalableTag<uint8_t> du8;
  const Half<decltype(du8)> dhu8;
  const Rebind<uint16_t, decltype(dhu8)> du16;
  const int lanes = Lanes(du8);
  using V8 = Vec<decltype(du8)>;
  const auto scale = Set(du8, 255);
  const auto lScale = Set(dhu8, 255);
  const auto zeros = Set(du16, 0);
  const auto mScale = Set(du16, 255);

  for (int y = 0; y < height; ++y) {
    auto store = reinterpret_cast<uint8_t *>(mDestination);
    auto source = reinterpret_cast<const uint8_t *>(mSource);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      V8 R, G, B, A;
      LoadRGBA<PixelType>(du8, source, R, G, B, A);

      auto Ah = PromoteUpperTo(du16, A);
      auto Al = PromoteLowerTo(du16, A);

#if HWY_TARGET != HWY_SVE && HWY_TARGET != HWY_SVE2 && HWY_TARGET != HWY_SVE_256 && HWY_TARGET != HWY_SVE2_128
      const auto ahMask = Ah == zeros;
      const auto alMask = Ah == zeros;
      Ah = IfThenElse(ahMask, mScale, Ah);
      Al = IfThenElse(alMask, mScale, Al);
#endif

      const auto Rh = Div(WidenMulHigh(du8, R, scale), Ah);
      const auto Rl = Div(WidenMul(dhu8, LowerHalf(R), lScale), Al);

      const auto Bh = Div(WidenMulHigh(du8, B, scale), Ah);
      const auto Bl = Div(WidenMul(dhu8, LowerHalf(B), lScale), Al);

      const auto Gh = Div(WidenMulHigh(du8, G, scale), Ah);
      const auto Gl = Div(WidenMul(dhu8, LowerHalf(G), lScale), Al);

      const auto RR = Combine(du8, DemoteTo(dhu8, Rh), DemoteTo(dhu8, Rl));
      const auto GG = Combine(du8, DemoteTo(dhu8, Gh), DemoteTo(dhu8, Gl));
      const auto BB = Combine(du8, DemoteTo(dhu8, Bh), DemoteTo(dhu8, Bl));

      StoreRGBA<PixelType>(du8, store, RR, GG, BB, A);

      store += components * lanes;
      source += components * lanes;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;
      uint16_t a;

      LoadRGBA<uint8_t, uint16_t, PixelType>(source, r, g, b, a);

      if (a != 0) {
        r = (r * 255) / a;
        g = (g * 255) / a;
        b = (b * 255) / a;
      }

      StoreRGBA<uint8_t, uint16_t, PixelType>(store, r, g, b, a);

      store += components;
      source += components;
    }

    mDestination += dstStride;
    mSource += srcStride;
  }
}

#define UNPREMULTIPLY_ALPHA_DECLARATION_R(pixelType) \
    void pixelType##UnpremultiplyAlphaHWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                        uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height) {\
        UnpremultiplyAlpha8HWY<sparkyuv::PIXEL_##pixelType>(src, srcStride, dst, dstStride, width, height); \
    }

UNPREMULTIPLY_ALPHA_DECLARATION_R(RGBA)
UNPREMULTIPLY_ALPHA_DECLARATION_R(ARGB)
UNPREMULTIPLY_ALPHA_DECLARATION_R(ABGR)
UNPREMULTIPLY_ALPHA_DECLARATION_R(BGRA)

#undef UNPREMULTIPLY_ALPHA_DECLARATION_R

}

HWY_AFTER_NAMESPACE();

#endif