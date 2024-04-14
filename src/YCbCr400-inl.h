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

#if defined(SPARKYUV_YCbCr444_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YCbCr444_INL_H
#undef SPARKYUV_YCbCr444_INL_H
#else
#define SPARKYUV_YCbCr444_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include <algorithm>
#include <cmath>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void Pixel8ToYCbCr400HWY(const uint8_t *SPARKYUV_RESTRICT src,
                         const uint32_t srcStride,
                         const uint32_t width,
                         const uint32_t height,
                         uint8_t *SPARKYUV_RESTRICT yPlane,
                         const uint32_t yStride,
                         const float kr,
                         const float kb,
                         const SparkYuvColorRange colorRange) {
  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  uint16_t YR, YG, YB;
  uint16_t CbR, CbG, CbB;
  uint16_t CrR, CrG, CrB;

  const int precision = 8;

  ComputeTransformIntegers(kr, kb, static_cast<float>(biasY), static_cast<float>(biasUV),
                           static_cast<float>(rangeY), static_cast<float>(rangeUV),
                           255.f, precision, YR, YG, YB, CbR, CbG, CbB, CrR, CrG, CrB);

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);

  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const ScalableTag<uint16_t> du16;
  const ScalableTag<int16_t> di16;
  const Rebind<uint8_t, decltype(di16)> du8;
  const Half<decltype(di16)> dhi16;
  const Rebind<int32_t, decltype(dhi16)> d32;
  using VU8 = Vec<decltype(du8)>;
  using VU16 = Vec<decltype(di16)>;
  using V32 = Vec<decltype(d32)>;

  const auto scale = static_cast<float>( 1 << precision );
  const auto iBiasY = static_cast<uint16_t>((static_cast<float>(biasY) + 0.5f) * scale);

  const int lanes = Lanes(du8);

  const auto coeffTag = di16;

  const auto vYR = Set(coeffTag, YR);
  const auto vYG = Set(coeffTag, YG);
  const auto vYB = Set(coeffTag, YB);

  const auto vBiasY = Set(d32, iBiasY);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<uint8_t *>(yStore);

    auto mSrc = reinterpret_cast<const uint8_t *>(mSource);

    for (; x + lanes < width; x += lanes) {
      VU8 R8;
      VU8 G8;
      VU8 B8;
      VU8 A8;
      LoadRGBA<PixelType>(du8, mSrc, R8, G8, B8, A8);

      VU16 R16 = PromoteTo(di16, R8);
      VU16 G16 = PromoteTo(di16, G8);
      VU16 B16 = PromoteTo(di16, B8);

      V32 YRh = vBiasY;
      V32 YRl = WidenMulAccumulate(d32, R16, vYR, vBiasY, YRh);
      YRl = WidenMulAccumulate(d32, G16, vYG, YRl, YRh);
      YRl = WidenMulAccumulate(d32, B16, vYB, YRl, YRh);

      const auto Y = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, YRh), ShiftRightNarrow<8>(d32, YRl)));

      StoreU(DemoteTo(du8, Y), du8, yDst);

      yDst += lanes;

      mSrc += components * lanes;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;

      LoadRGB<uint8_t, uint16_t, PixelType>(mSrc, r, g, b);

      int Y = ((r * YR + g * YG + b * YB + iBiasY) >> precision);

      yDst[0] = Y;

      yDst += 1;

      mSrc += components;
    }

    yStore += yStride;
    mSource += srcStride;
  }
}

#define XXXXToYCbCr400HWY_DECLARATION_R(pixelType) \
        void pixelType##ToYCbCr400HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                      const uint32_t width, const uint32_t height,\
                                      uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                      const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel8ToYCbCr400HWY<sparkyuv::PIXEL_##pixelType>(src, srcStride, width, height,\
                                                           yPlane, yStride, kr, kb, colorRange);\
        }

XXXXToYCbCr400HWY_DECLARATION_R(RGBA)
XXXXToYCbCr400HWY_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr400HWY_DECLARATION_R(ARGB)
XXXXToYCbCr400HWY_DECLARATION_R(ABGR)
XXXXToYCbCr400HWY_DECLARATION_R(BGRA)
XXXXToYCbCr400HWY_DECLARATION_R(BGR)
#endif

#undef XXXXToYCbCr400HWY_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void YCbCr400ToXRGB(uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                    const uint32_t width, const uint32_t height,
                    const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                    const float kr, const float kb, const SparkYuvColorRange colorRange) {
  const ScalableTag<uint8_t> du8;
  const Half<decltype(du8)> du8h;
  const Rebind<int16_t, decltype(du8h)> di16;
  const Rebind<uint16_t, decltype(du8h)> du16;
  using VI16 = Vec<decltype(di16)>;

  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  const auto uvCorrIY = Set(du8, biasY);
  const auto A = Set(du8, 255);

  float fCrCoeff = 0.f;
  float fCbCoeff = 0.f;
  float fGCoeff1 = 0.f;
  float fGCoeff2 = 0.f;
  const float flumaCoeff = 255.f / static_cast<float>(rangeY);
  ComputeInverseTransform(kr, kb, 255.f, static_cast<float>(rangeUV), fCrCoeff, fCbCoeff, fGCoeff1, fGCoeff2);

  const int precision = 6;

  const int iLumaCoeff = static_cast<int>(::roundf(flumaCoeff * static_cast<float>( 1 << precision )));

  const auto ivLumaCoeff = Set(du8, iLumaCoeff);
  const auto ivLumaCoeffh = Set(du8h, iLumaCoeff);

  const int lanes = Lanes(du8);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  for (int y = 0; y < height; ++y) {
    auto ySrc = reinterpret_cast<const uint8_t *>(mYSrc);
    auto store = reinterpret_cast<uint8_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const auto Y = Sub(LoadU(du8, ySrc), uvCorrIY);

      const auto Yh = BitCast(di16, WidenMulHigh(du8, Y, ivLumaCoeff));
      const auto Yl = BitCast(di16, WidenMul(du8h, LowerHalf(Y), ivLumaCoeffh));
      const auto tone =
          Combine(du8, ShiftRightNarrow<6>(du16, BitCast(du16, Yh)), ShiftRightNarrow<6>(du16, BitCast(du16, Yl)));

      StoreRGBA<PixelType>(du8, store, tone, tone, tone, A);

      store += lanes * components;
      ySrc += lanes;
    }

    for (; x < width; ++x) {

      int Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
      int tone = Y >> 6;

      SaturatedStoreRGBA<uint8_t, int, PixelType>(store, tone, tone, tone, 255, 255);
      store += components;
      ySrc += 1;
    }
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define YCbCr400ToXXXX_DECLARATION_R(pixelType) \
    void YCbCr400To##pixelType##HWY(uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                    const uint32_t width, const uint32_t height,\
                                    const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                    const float kr, const float kb, const SparkYuvColorRange colorRange) {\
                         YCbCr400ToXRGB<sparkyuv::PIXEL_##pixelType>(src, srcStride, width, height,\
                                                                     yPlane, yStride, kr, kb, colorRange);\
                         }

YCbCr400ToXXXX_DECLARATION_R(RGBA)
YCbCr400ToXXXX_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr400ToXXXX_DECLARATION_R(ARGB)
YCbCr400ToXXXX_DECLARATION_R(ABGR)
YCbCr400ToXXXX_DECLARATION_R(BGRA)
YCbCr400ToXXXX_DECLARATION_R(BGR)
#endif

#undef YCbCr400ToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif