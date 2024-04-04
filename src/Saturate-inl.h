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

#if defined(SPARKYUV__SATURATE_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__SATURATE_INL_H
#undef SPARKYUV__SATURATE_INL_H
#else
#define SPARKYUV__SATURATE_INL_H
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
      switch (PixelType) {
        case PIXEL_RGB:LoadInterleaved3(du16, source, R, G, B);
          break;
        case PIXEL_BGR:LoadInterleaved3(du16, source, B, G, R);
          break;
        case PIXEL_RGBA:LoadInterleaved4(du16, source, R, G, B, A);
          break;
        case PIXEL_BGRA:LoadInterleaved4(du16, source, B, G, B, A);
          break;
        case PIXEL_ARGB:LoadInterleaved4(du16, source, A, R, G, B);
          break;
        case PIXEL_ABGR:LoadInterleaved4(du16, source, A, B, G, R);
          break;
      }

      if (diff == 2) {
        R = ShiftRight<2>(R);
        G = ShiftRight<2>(G);
        B = ShiftRight<2>(B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftRight<2>(A);
        }
      } else if (diff == 4) {
        R = ShiftRight<4>(R);
        G = ShiftRight<4>(G);
        B = ShiftRight<4>(B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftRight<4>(A);
        }
      } else if (diff == 8) {
        R = ShiftRight<8>(R);
        G = ShiftRight<8>(G);
        B = ShiftRight<8>(B);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftRight<8>(A);
        }
      } else {
        R = ShiftRightSame(R, diff);
        G = ShiftRightSame(G, diff);
        B = ShiftRightSame(B, diff);
        if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
          A = ShiftRightSame(A, diff);
        }
      }

      const V8 R8 = DemoteTo(du8, R);
      const V8 G8 = DemoteTo(du8, G);
      const V8 B8 = DemoteTo(du8, B);
      V8 A8;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        A8 = DemoteTo(du8, A);
      }

      switch (PixelType) {
        case PIXEL_RGBA:StoreInterleaved4(R8, G8, B8, A8, du8, store);
          break;
        case PIXEL_ABGR:StoreInterleaved4(A8, B8, G8, R8, du8, store);
          break;
        case PIXEL_BGR:StoreInterleaved3(B8, G8, R8, du8, store);
          break;
        case PIXEL_RGB:StoreInterleaved3(R8, G8, B8, du8, store);
          break;
        case PIXEL_BGRA:StoreInterleaved4(B8, G8, R8, A8, du8, store);
          break;
        case PIXEL_ARGB:StoreInterleaved4(A8, R8, G8, B8, du8, store);
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

      r = r >> diff;
      g = g >> diff;
      b = b >> diff;
      if (PixelType != PIXEL_BGR && PixelType != PIXEL_RGB) {
        a = a >> diff;
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

#define SaturateXXXXFromNTo8_DECLARATION_R(pixelType, sourceBitDepth) \
    void Saturate##pixelType##sourceBitDepth##To8HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride, \
                                                     uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                     const uint32_t width, const uint32_t height) {\
        SaturateSurfaceFromNBitTo8<sparkyuv::PIXEL_##pixelType, sourceBitDepth>(src, srcStride, dst, dstStride,\
                                                                                width, height); \
    }

SaturateXXXXFromNTo8_DECLARATION_R(RGBA, 10)
SaturateXXXXFromNTo8_DECLARATION_R(ARGB, 10)
SaturateXXXXFromNTo8_DECLARATION_R(ABGR, 10)
SaturateXXXXFromNTo8_DECLARATION_R(BGRA, 10)
SaturateXXXXFromNTo8_DECLARATION_R(RGB, 10)
SaturateXXXXFromNTo8_DECLARATION_R(BGR, 10)

SaturateXXXXFromNTo8_DECLARATION_R(RGBA, 12)
SaturateXXXXFromNTo8_DECLARATION_R(ARGB, 12)
SaturateXXXXFromNTo8_DECLARATION_R(ABGR, 12)
SaturateXXXXFromNTo8_DECLARATION_R(BGRA, 12)
SaturateXXXXFromNTo8_DECLARATION_R(RGB, 12)
SaturateXXXXFromNTo8_DECLARATION_R(BGR, 12)

SaturateXXXXFromNTo8_DECLARATION_R(RGBA, 16)
SaturateXXXXFromNTo8_DECLARATION_R(ARGB, 16)
SaturateXXXXFromNTo8_DECLARATION_R(ABGR, 16)
SaturateXXXXFromNTo8_DECLARATION_R(BGRA, 16)
SaturateXXXXFromNTo8_DECLARATION_R(RGB, 16)
SaturateXXXXFromNTo8_DECLARATION_R(BGR, 16)

#undef SaturateXXXXFromNTo8_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif