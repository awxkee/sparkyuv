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
#if defined(SPARKYUV__BALPHA_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__BALPHA_INL_H
#undef SPARKYUV__BALPHA_INL_H
#else
#define SPARKYUV__BALPHA_INL_H
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

  const ScalableTag<uint16_t> du16;
  const Rebind<uint8_t, decltype(du16)> du8;
  using V8 = Vec<decltype(du8)>;

  const int lanes = Lanes(du8);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

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

      const auto A16 = PromoteTo(du16, A);

      R = DemoteTo(du8, ShiftRight<8>(SaturatedAdd(Mul(PromoteTo(du16, R), A16), vExpand)));
      G = DemoteTo(du8, ShiftRight<8>(SaturatedAdd(Mul(PromoteTo(du16, G), A16), vExpand)));
      B = DemoteTo(du8, ShiftRight<8>(SaturatedAdd(Mul(PromoteTo(du16, B), A16), vExpand)));

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

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  for (int y = 0; y < height; ++y) {
    auto store = reinterpret_cast<uint8_t *>(mDestination);
    auto source = reinterpret_cast<const uint8_t *>(mSource);

    uint32_t x = 0;

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