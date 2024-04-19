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

#if defined(SPARKYUV_RGB565_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_RGB565_INL_H
#undef SPARKYUV_RGB565_INL_H
#else
#define SPARKYUV_RGB565_INL_H
#endif

#include "hwy/highway.h"
#include "src/yuv-inl.h"
#include "src/sparkyuv-internal.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
template<class T, SparkYuvReformatPixelType PixelType = sparkyuv::REFORMAT_RGBA>
void
ReformatSurfaceToRGB565Impl(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                            uint16_t *SPARKYUV_RESTRICT destination, const uint32_t dstStride,
                            const uint32_t width, const uint32_t height, const int bitDepth) {
  const int rbShiftDiff = bitDepth - 5;
  const int gShiftDiff = bitDepth - 6;

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(destination);

  const int components = (PixelType == REFORMAT_BGR || PixelType == REFORMAT_RGB) ? 3 : 4;

  const ScalableTag<uint16_t> d16;

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto srcPixels = reinterpret_cast<const T *>(mSource);
    auto dstPixels = reinterpret_cast<uint16_t *>(mDestination);

    if (std::is_same<T, uint8_t>::value) {
      const ScalableTag<uint8_t> d8;
      const int lanes = Lanes(d8);
      const int writeLanes = Lanes(d16);
      using V = Vec<decltype(d8)>;
      for (; x + lanes < width; x += lanes) {
        V R;
        V G;
        V B;
        V A;
        LoadReformatRGBA<PixelType>(d8, reinterpret_cast<const uint8_t *>(srcPixels), R, G, B, A);

        auto R16l = PromoteLowerTo(d16, R);
        auto G16l = PromoteLowerTo(d16, G);
        auto B16l = PromoteLowerTo(d16, B);

        R16l = ShiftLeft<11>(ShiftRightSame(R16l, rbShiftDiff));
        G16l = ShiftLeft<5>(ShiftRightSame(G16l, gShiftDiff));
        B16l = ShiftRightSame(B16l, rbShiftDiff);
        const auto lo = Or(Or(R16l, G16l), B16l);

        StoreU(lo, d16, reinterpret_cast<uint16_t *>(dstPixels));

        R16l = PromoteUpperTo(d16, R);
        G16l = PromoteUpperTo(d16, G);
        B16l = PromoteUpperTo(d16, B);

        R16l = ShiftLeft<11>(ShiftRightSame(R16l, rbShiftDiff));
        G16l = ShiftLeft<5>(ShiftRightSame(G16l, gShiftDiff));
        B16l = ShiftRightSame(B16l, rbShiftDiff);
        const auto hi = Or(Or(R16l, G16l), B16l);

        StoreU(hi, d16, reinterpret_cast<uint16_t *>(dstPixels) + writeLanes);

        srcPixels += components * lanes;
        dstPixels += writeLanes * 2;
      }
    } else if (std::is_same<T, uint16_t>::value) {
      const int lanes = Lanes(d16);
      const int writeLanes = Lanes(d16);
      using V = Vec<decltype(d16)>;
      V R;
      V G;
      V B;
      V A;
      LoadReformatRGBA<PixelType>(d16, reinterpret_cast<const uint16_t *>(srcPixels), R, G, B, A);

      const auto R32h = ShiftLeft<11>(ShiftRightSame(R, rbShiftDiff));
      const auto G32h = ShiftLeft<5>(ShiftRightSame(G, gShiftDiff));
      const auto B32h = ShiftRightSame(B, rbShiftDiff);
      const auto hi = Or(Or(R32h, G32h), B32h);

      StoreU(hi, d16, reinterpret_cast<uint16_t *>(dstPixels));

      srcPixels += components * lanes;
      dstPixels += writeLanes;
    }

    for (; x < width; ++x) {
      T r;
      T g;
      T b;

      switch (PixelType) {
        case REFORMAT_RGBA1010102: {
          uint32_t rgba1010102 = reinterpret_cast<const uint32_t *>(srcPixels)[0];
          const uint32_t scalarMask = (1u << 10u) - 1u;
          uint32_t b32, g32, r32;
          b32 = static_cast<T>((rgba1010102 & scalarMask));
          g32 = static_cast<T>(((rgba1010102 >> 10) & scalarMask));
          r32 = static_cast<T>(((rgba1010102 >> 20) & scalarMask));
          r = static_cast<T>(r32);
          g = static_cast<T>(g32);
          b = static_cast<T>(b32);
        }
          break;
        case REFORMAT_RGBA:
        case REFORMAT_RGB: {
          r = static_cast<T>(srcPixels[0]);
          g = static_cast<T>(srcPixels[1]);
          b = static_cast<T>(srcPixels[2]);
        }
          break;
        case REFORMAT_BGRA:
        case REFORMAT_BGR: {
          b = static_cast<T>(srcPixels[0]);
          g = static_cast<T>(srcPixels[1]);
          r = static_cast<T>(srcPixels[2]);
        }
          break;
        case REFORMAT_ARGB: {
          r = static_cast<T>(srcPixels[1]);
          g = static_cast<T>(srcPixels[2]);
          b = static_cast<T>(srcPixels[3]);
        }
          break;
        case REFORMAT_ABGR: {
          b = static_cast<T>(srcPixels[1]);
          g = static_cast<T>(srcPixels[2]);
          r = static_cast<T>(srcPixels[3]);
        }
          break;
      }

      uint16_t red565 = (r >> rbShiftDiff) << 11;
      uint16_t green565 = (g >> gShiftDiff) << 5;
      uint16_t blue565 = b >> rbShiftDiff;

      auto result = static_cast<uint16_t>(red565 | green565 | blue565);
      dstPixels[0] = result;

      srcPixels += components;
      dstPixels += 1;
    }

    mSource += srcStride;
    mDestination += dstStride;
  }
}

template<SparkYuvReformatPixelType PixelType = sparkyuv::REFORMAT_RGBA>
void
RGB565ToF16Impl(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                uint16_t *SPARKYUV_RESTRICT destination, const uint32_t dstStride,
                const uint32_t width, const uint32_t height) {
  static_assert(PixelType != REFORMAT_RGBA1010102, "RGBA1010102 has no sense in F16");

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(destination);

  const int components = (PixelType == REFORMAT_BGR || PixelType == REFORMAT_RGB) ? 3 : 4;

  const auto rbMaxColors = 1.f / static_cast<float >(::powf(2.f, 5.f) - 1.f);
  const auto gMaxColors = 1.f / static_cast<float>(::powf(2.f, 6.f) - 1.f);
  const uint16_t a = hwy::F16FromF32(1.0f).bits;

#if SPARKYUV_ALLOW_FLOAT16
  const ScalableTag<hwy::float16_t> df;
  const auto vRBScale = Set(df, hwy::F16FromF32(rbMaxColors));
  const auto vGScale = Set(df, hwy::F16FromF32(gMaxColors));
  const int lanes = Lanes(df);
  using V = Vec<decltype(df)>;
  const Rebind<uint16_t, decltype(df)> d16;
  const auto vEraseRB = Set(d16, 0x1F);
  const auto vEraseG = Set(d16, 0x3F);
  const auto A = Set(df, hwy::F16FromF32(1.0f));
#endif

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto srcPixels = reinterpret_cast<const uint16_t *>(mSource);
    auto dstPixels = reinterpret_cast<uint16_t *>(mDestination);

#if SPARKYUV_ALLOW_FLOAT16
    for (; x + lanes < width; x += lanes) {
      const auto rgb565 = LoadU(d16, srcPixels);
      auto R = ConvertTo(df, And(ShiftRight<11>(rgb565), vEraseRB));
      auto G = ConvertTo(df, And(ShiftRight<5>(rgb565), vEraseG));
      auto B = ConvertTo(df, And(rgb565, vEraseRB));

      R = Mul(R, vRBScale);
      G = Mul(G, vGScale);
      B = Mul(B, vRBScale);

      StoreReformatRGBA<PixelType>(df, reinterpret_cast<hwy::float16_t *>(dstPixels), R, G, B, A);

      srcPixels += lanes;
      dstPixels += components * lanes;
    }
#endif

    for (; x < width; ++x) {
      uint16_t color565 = srcPixels[0];
      auto r = hwy::F16FromF32(static_cast<float>(((color565 >> 11) & 0x1F)) * rbMaxColors).bits;
      auto g = hwy::F16FromF32(static_cast<float>(((color565 >> 5) & 0x3F)) * gMaxColors).bits;
      auto b = hwy::F16FromF32(static_cast<float>((color565 & 0x1F)) * rbMaxColors).bits;

      switch (PixelType) {
        case REFORMAT_RGBA:dstPixels[3] = a;
        case REFORMAT_RGB: {
          dstPixels[0] = r;
          dstPixels[1] = g;
          dstPixels[2] = b;
        }
          break;
        case REFORMAT_BGRA:dstPixels[3] = a;
        case REFORMAT_BGR: {
          dstPixels[2] = r;
          dstPixels[1] = g;
          dstPixels[0] = b;
        }
          break;
        case REFORMAT_ABGR: {
          dstPixels[0] = a;
          dstPixels[3] = r;
          dstPixels[2] = g;
          dstPixels[1] = b;
        }
          break;
        case REFORMAT_ARGB: {
          dstPixels[0] = a;
          dstPixels[1] = r;
          dstPixels[2] = g;
          dstPixels[3] = b;
        }
          break;
      }

      srcPixels += 1;
      dstPixels += components;
    }

    mSource += srcStride;
    mDestination += dstStride;
  }
}

#define F16_TO_RGB565(PixelName, Pixel) \
    void RGB565To##PixelName##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                 uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                  const uint32_t width, const uint32_t height) {\
        RGB565ToF16Impl<sparkyuv::REFORMAT_##Pixel>(src, srcStride, dst, dstStride,\
                                                   width, height);\
    }

F16_TO_RGB565(RGBF16, RGB)
F16_TO_RGB565(BGRF16, BGR)
F16_TO_RGB565(RGBAF16, RGBA)
F16_TO_RGB565(BGRAF16, BGRA)
F16_TO_RGB565(ABGRF16, ABGR)
F16_TO_RGB565(ARGBF16, ARGB)

#undef F16_TO_RGB565

template<SparkYuvReformatPixelType PixelType = sparkyuv::REFORMAT_RGBA>
void
ReformatF16ToRGB565Impl(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                        uint16_t *SPARKYUV_RESTRICT destination, const uint32_t dstStride,
                        const uint32_t width, const uint32_t height) {
  static_assert(PixelType != REFORMAT_RGBA1010102, "RGBA1010102 has no sense in F16");
  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(destination);

  const int components = (PixelType == REFORMAT_BGR || PixelType == REFORMAT_RGB) ? 3 : 4;

  const auto rbMaxColors = static_cast<float >(::powf(2.f, 5.f) - 1.f);
  const auto gMaxColors = static_cast<float>(::powf(2.f, 6.f) - 1.f);
#if SPARKYUV_ALLOW_FLOAT16
  const ScalableTag<hwy::float16_t> df;
  const auto vRBScale = Set(df, hwy::F16FromF32(rbMaxColors));
  const auto vGScale = Set(df, hwy::F16FromF32(gMaxColors));
  const int lanes = Lanes(df);
  using V = Vec<decltype(df)>;
  const Rebind<uint16_t, decltype(df)> d16;
#endif

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto srcPixels = reinterpret_cast<const uint16_t *>(mSource);
    auto dstPixels = reinterpret_cast<uint16_t *>(mDestination);

#if SPARKYUV_ALLOW_FLOAT16
    for (; x + lanes < width; x += lanes) {
      V R;
      V G;
      V B;
      V A;
      LoadReformatRGBA<PixelType>(df, reinterpret_cast<const hwy::float16_t *>(srcPixels), R, G, B, A);

      R = Mul(R, vRBScale);
      G = Mul(G, vGScale);
      B = Mul(B, vRBScale);

      const auto R32h = ShiftLeft<11>(ConvertTo(d16, R));
      const auto G32h = ShiftLeft<5>(ConvertTo(d16, G));
      const auto B32h = ConvertTo(d16, B);
      const auto px = Or(Or(R32h, G32h), B32h);

      StoreU(px, d16, reinterpret_cast<uint16_t *>(dstPixels));

      srcPixels += components * lanes;
      dstPixels += lanes;
    }
#endif

    for (; x < width; ++x) {
      float r;
      float g;
      float b;

      switch (PixelType) {
        case REFORMAT_RGBA:
        case REFORMAT_RGB: {
          r = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[0]));
          g = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[1]));
          b = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[2]));
        }
          break;
        case REFORMAT_BGRA:
        case REFORMAT_BGR: {
          b = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[0]));
          g = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[1]));
          r = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[2]));
        }
          break;
        case REFORMAT_ARGB: {
          r = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[1]));
          g = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[2]));
          b = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[3]));
        }
          break;
        case REFORMAT_ABGR: {
          b = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[1]));
          g = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[2]));
          r = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[3]));
        }
          break;
      }

      uint16_t r565 = static_cast<uint16_t>(::roundf(r * rbMaxColors)) & 0x1F;
      uint16_t g565 = static_cast<uint16_t>(::roundf(g * gMaxColors)) & 0x3F;
      uint16_t b565 = static_cast<uint16_t>(::roundf(b * rbMaxColors)) & 0x1F;

      uint16_t red565 = r565 << 11;
      uint16_t green565 = g565 << 5;
      uint16_t blue565 = b565;

      auto result = static_cast<uint16_t>(red565 | green565 | blue565);
      dstPixels[0] = result;

      srcPixels += components;
      dstPixels += 1;
    }

    mSource += srcStride;
    mDestination += dstStride;
  }
}

#define REFORMATF16_TO_RGB565(PixelName, Pixel) \
    void PixelName##ToRGB565HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height) {\
        ReformatF16ToRGB565Impl<sparkyuv::REFORMAT_##Pixel>(src, srcStride, dst, dstStride,\
                                                           width, height);\
    }

REFORMATF16_TO_RGB565(RGBF16, RGB)
REFORMATF16_TO_RGB565(BGRF16, BGR)
REFORMATF16_TO_RGB565(RGBAF16, RGBA)
REFORMATF16_TO_RGB565(BGRAF16, BGRA)
REFORMATF16_TO_RGB565(ABGRF16, ABGR)
REFORMATF16_TO_RGB565(ARGBF16, ARGB)

#undef REFORMATF16_TO_RGB565

#define REFORMAT_TO_RGB565_BIT_DEFINED(PixelName, Pixel, T, depth) \
    void PixelName##ToRGB565HWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height) {\
        ReformatSurfaceToRGB565Impl<T, sparkyuv::REFORMAT_##Pixel>(src, srcStride, dst, dstStride,\
                                                                  width, height, depth);\
    }

REFORMAT_TO_RGB565_BIT_DEFINED(RGB, RGB, uint8_t, 8)
REFORMAT_TO_RGB565_BIT_DEFINED(BGR, BGR, uint8_t, 8)
REFORMAT_TO_RGB565_BIT_DEFINED(RGBA, RGBA, uint8_t, 8)
REFORMAT_TO_RGB565_BIT_DEFINED(BGRA, BGRA, uint8_t, 8)
REFORMAT_TO_RGB565_BIT_DEFINED(ABGR, ABGR, uint8_t, 8)
REFORMAT_TO_RGB565_BIT_DEFINED(ARGB, ARGB, uint8_t, 8)
REFORMAT_TO_RGB565_BIT_DEFINED(RGBA1010102, RGBA1010102, uint8_t, 10)

#undef REFORMAT_TO_RGB565_BIT_DEFINED

#define REFORMAT_TO_RGB565(PixelName, Pixel, T) \
    void PixelName##ToRGB565HWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                const uint32_t width, const uint32_t height, const int bitDepth) {\
        ReformatSurfaceToRGB565Impl<T, sparkyuv::REFORMAT_##Pixel>(src, srcStride, dst, dstStride,\
                                                                  width, height, bitDepth);\
    }

REFORMAT_TO_RGB565(RGB16, RGB, uint16_t)
REFORMAT_TO_RGB565(BGR16, BGR, uint16_t)
REFORMAT_TO_RGB565(RGBA16, RGBA, uint16_t)
REFORMAT_TO_RGB565(BGRA16, BGRA, uint16_t)
REFORMAT_TO_RGB565(ABGR16, ABGR, uint16_t)
REFORMAT_TO_RGB565(ARGB16, ARGB, uint16_t)

#undef REFORMAT_TO_RGB565

template<class T, SparkYuvReformatPixelType PixelType = sparkyuv::REFORMAT_RGBA>
void
RGB565ReformatToSurfaceImpl(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                            T *SPARKYUV_RESTRICT destination, const uint32_t dstStride,
                            const uint32_t width, const uint32_t height, const int bitDepth) {
  const int rbShiftDiff = bitDepth - 5;
  const int gShiftDiff = bitDepth - 6;

  auto mSource = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(destination);

  const T maxColors = static_cast<T>(::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

  const int components = (PixelType == REFORMAT_BGR || PixelType == REFORMAT_RGB) ? 3 : 4;

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;
    auto srcPixels = reinterpret_cast<const uint16_t *>(mSource);
    auto dstPixels = reinterpret_cast<T *>(mDestination);
    for (; x < width; ++x) {
      uint16_t color565 = srcPixels[0];
      T r = static_cast<T>(((color565 >> 11) & 0x1F) << rbShiftDiff);
      T g = static_cast<T>(((color565 >> 5) & 0x3F) << gShiftDiff);
      T b = static_cast<T>((color565 & 0x1F) << rbShiftDiff);
      T a = maxColors;

      switch (PixelType) {
        case REFORMAT_RGBA1010102: {
          const int a1010102Diff = bitDepth - 2;
          uint32_t r32 = r, g32 = g, b32 = b, a32 = a;
          a32 = a32 >> a1010102Diff;
          r32 = r32;
          g32 = g32;
          b32 = b32;
          uint32_t px = (a32 << 30) | (r32 << 20) | (g32 << 10) | b32;
          reinterpret_cast<uint32_t *>(dstPixels)[0] = px;
        }
          break;
        case REFORMAT_RGBA:dstPixels[3] = a;
        case REFORMAT_RGB: {
          dstPixels[0] = r;
          dstPixels[1] = g;
          dstPixels[2] = b;
        }
          break;
        case REFORMAT_BGRA:dstPixels[3] = a;
        case REFORMAT_BGR: {
          dstPixels[2] = r;
          dstPixels[1] = g;
          dstPixels[0] = b;
        }
          break;
        case REFORMAT_ABGR: {
          dstPixels[0] = a;
          dstPixels[3] = r;
          dstPixels[2] = g;
          dstPixels[1] = b;
        }
          break;
        case REFORMAT_ARGB: {
          dstPixels[0] = a;
          dstPixels[1] = r;
          dstPixels[2] = g;
          dstPixels[3] = b;
        }
          break;
      }

      srcPixels += 1;
      dstPixels += components;
    }

    mSource += srcStride;
    mDestination += dstStride;
  }
}

#define REFORMAT_RGB565_TO_NEW_BIT(PixelName, Pixel, T, depth) \
    void RGB565To##PixelName##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                  T *SPARKYUV_RESTRICT destination, const uint32_t dstStride,\
                                  const uint32_t width, const uint32_t height) {\
        RGB565ReformatToSurfaceImpl<T, sparkyuv::REFORMAT_##Pixel>(src, srcStride, destination, dstStride,\
                                                                   width, height, depth);\
    }

REFORMAT_RGB565_TO_NEW_BIT(RGB, RGB, uint8_t, 8)
REFORMAT_RGB565_TO_NEW_BIT(BGR, BGR, uint8_t, 8)
REFORMAT_RGB565_TO_NEW_BIT(RGBA, RGBA, uint8_t, 8)
REFORMAT_RGB565_TO_NEW_BIT(BGRA, BGRA, uint8_t, 8)
REFORMAT_RGB565_TO_NEW_BIT(ABGR, ABGR, uint8_t, 8)
REFORMAT_RGB565_TO_NEW_BIT(ARGB, ARGB, uint8_t, 8)
REFORMAT_RGB565_TO_NEW_BIT(RGBA1010102, RGBA1010102, uint8_t, 10)

#undef REFORMAT_RGB565_TO_NEW_BIT

#define REFORMAT_RGB565_TO_NEW_SURFACE(PixelName, Pixel, T) \
    void RGB565To##PixelName##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                  T *SPARKYUV_RESTRICT destination, const uint32_t dstStride,\
                                  const uint32_t width, const uint32_t height, const int bitDepth) {\
        RGB565ReformatToSurfaceImpl<T, sparkyuv::REFORMAT_##Pixel>(src, srcStride, destination, dstStride,\
                                                                   width, height, bitDepth);\
    }

REFORMAT_RGB565_TO_NEW_SURFACE(RGB16, RGB, uint16_t)
REFORMAT_RGB565_TO_NEW_SURFACE(BGR16, BGR, uint16_t)
REFORMAT_RGB565_TO_NEW_SURFACE(RGBA16, RGBA, uint16_t)
REFORMAT_RGB565_TO_NEW_SURFACE(BGRA16, BGRA, uint16_t)
REFORMAT_RGB565_TO_NEW_SURFACE(ABGR16, ABGR, uint16_t)
REFORMAT_RGB565_TO_NEW_SURFACE(ARGB16, ARGB, uint16_t)

#undef REFORMAT_RGB565_TO_NEW_SURFACE

}
HWY_AFTER_NAMESPACE();

#endif