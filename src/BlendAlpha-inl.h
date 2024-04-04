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
      switch (PixelType) {
        case PIXEL_RGBA:LoadInterleaved4(du8, source, R, G, B, A);
          break;
        case PIXEL_BGRA:LoadInterleaved4(du8, source, B, G, B, A);
          break;
        case PIXEL_ARGB:LoadInterleaved4(du8, source, A, R, G, B);
          break;
        case PIXEL_ABGR:LoadInterleaved4(du8, source, A, B, G, R);
          break;
        default:break;
      }

      const auto A16 = PromoteTo(du16, A);

      R = DemoteTo(du8, ShiftRight<8>(SaturatedAdd(Mul(PromoteTo(du16, R), A16), vExpand)));
      G = DemoteTo(du8, ShiftRight<8>(SaturatedAdd(Mul(PromoteTo(du16, G), A16), vExpand)));
      B = DemoteTo(du8, ShiftRight<8>(SaturatedAdd(Mul(PromoteTo(du16, B), A16), vExpand)));

      switch (PixelType) {
        case PIXEL_RGBA:StoreInterleaved4(R, G, B, A, du8, store);
          break;
        case PIXEL_ABGR:StoreInterleaved4(A, B, G, R, du8, store);
          break;
        case PIXEL_BGRA:StoreInterleaved4(B, G, R, A, du8, store);
          break;
        case PIXEL_ARGB:StoreInterleaved4(A, R, G, B, du8, store);
          break;
      }

      store += lanes * components;
      source += lanes * components;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;
      uint16_t a;

      switch (PixelType) {
        case PIXEL_RGBA:r = static_cast<uint16_t>(source[0]);
          g = static_cast<uint16_t>(source[1]);
          b = static_cast<uint16_t>(source[2]);
          a = static_cast<uint16_t>(source[3]);
          break;
        case PIXEL_BGRA:b = static_cast<uint16_t>(source[0]);
          g = static_cast<uint16_t>(source[1]);
          r = static_cast<uint16_t>(source[2]);
          a = static_cast<uint16_t>(source[3]);
          break;
        case PIXEL_ARGB:a = static_cast<uint16_t>(source[0]);
          r = static_cast<uint16_t>(source[1]);
          g = static_cast<uint16_t>(source[2]);
          b = static_cast<uint16_t>(source[3]);
          break;
        case PIXEL_ABGR:a = static_cast<uint16_t>(source[0]);
          b = static_cast<uint16_t>(source[1]);
          g = static_cast<uint16_t>(source[2]);
          r = static_cast<uint16_t>(source[3]);
          break;
      }

      r = (r * a + 255) >> 8;
      g = (g * a + 255) >> 8;
      b = (b * a + 255) >> 8;

      switch (PixelType) {
        case PIXEL_RGBA: {
          store[3] = a;
          store[0] = r;
          store[1] = g;
          store[2] = b;
        }
          break;
        case PIXEL_BGRA: {
          store[3] = a;
          store[2] = r;
          store[1] = g;
          store[0] = b;
        }
          break;
        case PIXEL_ABGR: {
          store[0] = a;
          store[3] = r;
          store[2] = g;
          store[1] = b;
        }
          break;
        case PIXEL_ARGB: {
          store[0] = a;
          store[1] = r;
          store[2] = g;
          store[3] = b;
        }
          break;
      }

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

}

HWY_AFTER_NAMESPACE();

#endif