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

#if defined(SPARKYUV__WIDE_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__WIDE_INL_H
#undef SPARKYUV__WIDE_INL_H
#else
#define SPARKYUV__WIDE_INL_H
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

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

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
      switch (PixelType) {
        case PIXEL_RGB:LoadInterleaved3(du8, source, R8, G8, B8);
          break;
        case PIXEL_BGR:LoadInterleaved3(du8, source, B8, G8, R8);
          break;
        case PIXEL_RGBA:LoadInterleaved4(du8, source, R8, G8, B8, A8);
          break;
        case PIXEL_BGRA:LoadInterleaved4(du8, source, B8, G8, R8, A8);
          break;
        case PIXEL_ARGB:LoadInterleaved4(du8, source, A8, R8, G8, B8);
          break;
        case PIXEL_ABGR:LoadInterleaved4(du8, source, A8, B8, G8, R8);
          break;
      }

      V16 R = PromoteTo(du16, R8);
      V16 G = PromoteTo(du16, G8);
      V16 B = PromoteTo(du16, B8);
      V16 A;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        A = PromoteTo(du16, A8);
      }

      if (diff == 2) {
        R = ShiftLeft<2>(R);
        G = ShiftLeft<2>(G);
        B = ShiftLeft<2>(B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeft<2>(A);
        }
      } else if (diff == 4) {
        R = ShiftLeft<4>(R);
        G = ShiftLeft<4>(G);
        B = ShiftLeft<4>(B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeft<4>(A);
        }
      } else if (diff == 8) {
        R = ShiftLeft<8>(R);
        G = ShiftLeft<8>(G);
        B = ShiftLeft<8>(B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeft<8>(A);
        }
      } else {
        R = ShiftLeftSame(R, diff);
        G = ShiftLeftSame(G, diff);
        B = ShiftLeftSame(B, diff);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftLeftSame(A, diff);
        }
      }

      switch (PixelType) {
        case PIXEL_RGBA:StoreInterleaved4(R, G, B, A, du16, store);
          break;
        case PIXEL_ABGR:StoreInterleaved4(A, B, G, R, du16, store);
          break;
        case PIXEL_BGR:StoreInterleaved3(B, G, R, du16, store);
          break;
        case PIXEL_RGB:StoreInterleaved3(R, G, B, du16, store);
          break;
        case PIXEL_BGRA:StoreInterleaved4(B, G, R, A, du16, store);
          break;
        case PIXEL_ARGB:StoreInterleaved4(A, R, G, B, du16, store);
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
        case PIXEL_RGB:r = static_cast<uint16_t>(source[0]);
          g = static_cast<uint16_t>(source[1]);
          b = static_cast<uint16_t>(source[2]);
          break;
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
        case PIXEL_BGR:b = static_cast<uint16_t>(source[0]);
          g = static_cast<uint16_t>(source[1]);
          r = static_cast<uint16_t>(source[2]);
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

      r = r << diff;
      g = g << diff;
      b = b << diff;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        a = a << diff;
      }

      switch (PixelType) {
        case PIXEL_RGBA:store[3] = a;
        case PIXEL_RGB:store[0] = r;
          store[1] = g;
          store[2] = b;
          break;
        case PIXEL_BGRA:store[3] = a;
        case PIXEL_BGR:store[2] = r;
          store[1] = g;
          store[0] = b;
          break;
        case PIXEL_ABGR:store[0] = a;
          store[3] = r;
          store[2] = g;
          store[1] = b;
          break;
        case PIXEL_ARGB:store[0] = a;
          store[1] = r;
          store[2] = g;
          store[3] = b;
          break;
      }

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
WideXXXXFrom8ToN_DECLARATION_R(ARGB, 10)
WideXXXXFrom8ToN_DECLARATION_R(ABGR, 10)
WideXXXXFrom8ToN_DECLARATION_R(BGRA, 10)
WideXXXXFrom8ToN_DECLARATION_R(RGB, 10)
WideXXXXFrom8ToN_DECLARATION_R(BGR, 10)

WideXXXXFrom8ToN_DECLARATION_R(RGBA, 12)
WideXXXXFrom8ToN_DECLARATION_R(ARGB, 12)
WideXXXXFrom8ToN_DECLARATION_R(ABGR, 12)
WideXXXXFrom8ToN_DECLARATION_R(BGRA, 12)
WideXXXXFrom8ToN_DECLARATION_R(RGB, 12)
WideXXXXFrom8ToN_DECLARATION_R(BGR, 12)

WideXXXXFrom8ToN_DECLARATION_R(RGBA, 16)
WideXXXXFrom8ToN_DECLARATION_R(ARGB, 16)
WideXXXXFrom8ToN_DECLARATION_R(ABGR, 16)
WideXXXXFrom8ToN_DECLARATION_R(BGRA, 16)
WideXXXXFrom8ToN_DECLARATION_R(RGB, 16)
WideXXXXFrom8ToN_DECLARATION_R(BGR, 16)

#undef WideXXXXFrom8ToN_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif