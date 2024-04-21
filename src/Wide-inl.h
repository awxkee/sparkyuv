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

#if defined(SPARKYUV_WIDE_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_WIDE_INL_H
#undef SPARKYUV_WIDE_INL_H
#else
#define SPARKYUV_WIDE_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, int targetBitDepth>
void WideSurfaceFrom8BitToN(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                            uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                            const uint32_t width, const uint32_t height) {
  const ScalableTag<uint16_t> du16;
  const Rebind<uint8_t, decltype(du16)> du8;
  using V8 = Vec<decltype(du8)>;
  using V16 = Vec<decltype(du16)>;

  const int lanes = Lanes(du8);

  const int components = getPixelTypeComponents(PixelType);

  const int diff = targetBitDepth - 8;

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  for (int y = 0; y < height; ++y) {
    auto store = reinterpret_cast<uint16_t *>(mDestination);
    auto source = reinterpret_cast<const uint8_t *>(mSource);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      V8 R8;
      V8 G8;
      V8 B8;
      V8 A8;
      LoadRGBA<PixelType>(du8, source, R8, G8, B8, A8);

      V16 R, G, B, A;

      if (diff == 2) {
        R = ShiftLeftWide<2>(du8, R8);
        G = ShiftLeftWide<2>(du8, G8);
        B = ShiftLeftWide<2>(du8, B8);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeftWide<2>(du8, A8);
        }
      } else if (diff == 4) {
        R = ShiftLeftWide<4>(du8, R8);
        G = ShiftLeftWide<4>(du8, G8);
        B = ShiftLeftWide<4>(du8, B8);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeftWide<4>(du8, A8);
        }
      } else if (diff == 8) {
        R = ShiftLeftWide<8>(du8, R8);
        G = ShiftLeftWide<8>(du8, G8);
        B = ShiftLeftWide<8>(du8, B8);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeftWide<8>(du8, A8);
        }
      } else {
        R = ShiftLeftSame(PromoteTo(du16, R8), diff);
        G = ShiftLeftSame(PromoteTo(du16, G8), diff);
        B = ShiftLeftSame(PromoteTo(du16, B8), diff);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeftSame(PromoteTo(du16, A8), diff);
        }
      }

      StoreRGBA<PixelType>(du16, store, R, G, B, A);

      store += lanes * components;
      source += lanes * components;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;
      uint16_t a;

      LoadRGBA<uint8_t, uint16_t, PixelType>(source, r, g, b, a);

      r = r << diff;
      g = g << diff;
      b = b << diff;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        a = a << diff;
      }

      StoreRGBA<uint16_t, uint16_t, PixelType>(store, r, g, b, a);

      store += components;
      source += components;
    }

    mDestination += dstStride;
    mSource += srcStride;
  }
}

#define WideXXXXFrom8ToN_DECLARATION_R(pixelType, targetBitDepth) \
    void Wide##pixelType##8To##targetBitDepth##HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                   uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                   const uint32_t width, const uint32_t height) {\
        WideSurfaceFrom8BitToN<sparkyuv::PIXEL_##pixelType, targetBitDepth>(src, srcStride, dst, dstStride,\
                                                                            width, height); \
    }

WideXXXXFrom8ToN_DECLARATION_R(RGBA, 10)
WideXXXXFrom8ToN_DECLARATION_R(RGB, 10)

WideXXXXFrom8ToN_DECLARATION_R(RGBA, 12)
WideXXXXFrom8ToN_DECLARATION_R(RGB, 12)

WideXXXXFrom8ToN_DECLARATION_R(RGBA, 16)
WideXXXXFrom8ToN_DECLARATION_R(RGB, 16)

#undef WideXXXXFrom8ToN_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void WideSurfaceFrom8BitToGEN(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                            uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                            const uint32_t width, const uint32_t height, const int targetBitDepth) {
  const ScalableTag<uint16_t> du16;
  const Rebind<uint8_t, decltype(du16)> du8;
  using V8 = Vec<decltype(du8)>;
  using V16 = Vec<decltype(du16)>;

  const int lanes = Lanes(du8);

  const int components = getPixelTypeComponents(PixelType);

  const int diff = targetBitDepth - 8;

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  for (int y = 0; y < height; ++y) {
    auto store = reinterpret_cast<uint16_t *>(mDestination);
    auto source = reinterpret_cast<const uint8_t *>(mSource);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      V8 R8;
      V8 G8;
      V8 B8;
      V8 A8;
      LoadRGBA<PixelType>(du8, source, R8, G8, B8, A8);

      V16 R = PromoteTo(du16, R8);
      V16 G = PromoteTo(du16, G8);
      V16 B = PromoteTo(du16, B8);
      V16 A;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        A = PromoteTo(du16, A8);
      }

      R = ShiftLeftSame(R, diff);
      G = ShiftLeftSame(G, diff);
      B = ShiftLeftSame(B, diff);
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        A = ShiftLeftSame(A, diff);
      }

      StoreRGBA<PixelType>(du16, store, R, G, B, A);

      store += lanes * components;
      source += lanes * components;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;
      uint16_t a;

      LoadRGBA<uint8_t, uint16_t, PixelType>(source, r, g, b, a);

      r = r << diff;
      g = g << diff;
      b = b << diff;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        a = a << diff;
      }

      StoreRGBA<uint16_t, uint16_t, PixelType>(store, r, g, b, a);

      store += components;
      source += components;
    }

    mDestination += dstStride;
    mSource += srcStride;
  }
}

#define WideXXXXFrom8ToN_DECLARATION_GEN_R(pixelType) \
    void Wide##pixelType##8##HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                   uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                   const uint32_t width, const uint32_t height, const int targetBitDepth) {\
        WideSurfaceFrom8BitToGEN<sparkyuv::PIXEL_##pixelType>(src, srcStride, dst, dstStride,\
                                                                            width, height, targetBitDepth); \
    }

WideXXXXFrom8ToN_DECLARATION_GEN_R(RGBA)
WideXXXXFrom8ToN_DECLARATION_GEN_R(RGB)

#undef WideXXXXFrom8ToN_DECLARATION_GEN_R

}
HWY_AFTER_NAMESPACE();

#endif