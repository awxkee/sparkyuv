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
void Pixel8ToYCbCr444HWY(const uint8_t *SPARKYUV_RESTRICT src,
                         const uint32_t srcStride,
                         const uint32_t width,
                         const uint32_t height,
                         uint8_t *SPARKYUV_RESTRICT yPlane,
                         const uint32_t yStride,
                         uint8_t *SPARKYUV_RESTRICT uPlane,
                         const uint32_t uStride,
                         uint8_t *SPARKYUV_RESTRICT vPlane,
                         const uint32_t vStride,
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
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);

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
  const auto iBiasUV = static_cast<uint16_t>((static_cast<float>(biasUV) + 0.5f) * scale);

  const int lanes = Lanes(du8);

  const auto coeffTag = di16;

  const auto vYR = Set(coeffTag, YR);
  const auto vYG = Set(coeffTag, YG);
  const auto vYB = Set(coeffTag, YB);

  const auto vCbR = Set(coeffTag, -CbR);
  const auto vCbG = Set(coeffTag, -CbG);
  const auto vCbB = Set(coeffTag, CbB);

  const auto vCrR = Set(coeffTag, CrR);
  const auto vCrG = Set(coeffTag, -CrG);
  const auto vCrB = Set(coeffTag, -CrB);

  const auto vBiasY = Set(d32, iBiasY);
  const auto vBiasUV = Set(d32, iBiasUV);

  const int components = getPixelTypeComponents(PixelType);

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<uint8_t *>(yStore);
    auto uDst = reinterpret_cast<uint8_t *>(uStore);
    auto vDst = reinterpret_cast<uint8_t *>(vStore);

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

      V32 Cbh = vBiasUV;
      V32 Cbl = WidenMulAccumulate(d32, R16, vCbR, vBiasUV, Cbh);
      Cbl = WidenMulAccumulate(d32, G16, vCbG, Cbl, Cbh);
      Cbl = WidenMulAccumulate(d32, B16, vCbB, Cbl, Cbh);

      const auto Cb = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Cbh), ShiftRightNarrow<8>(d32, Cbl)));

      V32 Crh = vBiasUV;
      V32 Crl = WidenMulAccumulate(d32, R16, vCrR, vBiasUV, Crh);
      Crl = WidenMulAccumulate(d32, G16, vCrG, Crl, Crh);
      Crl = WidenMulAccumulate(d32, B16, vCrB, Crl, Crh);

      const auto Cr = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Crh), ShiftRightNarrow<8>(d32, Crl)));

      StoreU(DemoteTo(du8, Y), du8, yDst);
      StoreU(DemoteTo(du8, Cb), du8, uDst);
      StoreU(DemoteTo(du8, Cr), du8, vDst);

      yDst += lanes;
      uDst += lanes;
      vDst += lanes;

      mSrc += components * lanes;
    }

    for (; x < width; ++x) {
      uint16_t r;
      uint16_t g;
      uint16_t b;

      LoadRGB<uint8_t, uint16_t, PixelType>(mSrc, r, g, b);

      int Y = ((r * YR + g * YG + b * YB + iBiasY) >> precision);
      int Cb =
          ((-r * CbR - g * CbG + b * CbB + iBiasUV) >> precision);
      int Cr =
          ((r * CrR - g * CrG - b * CrB + iBiasUV) >> precision);

      yDst[0] = Y;
      uDst[0] = Cb;
      vDst[0] = Cr;

      yDst += 1;
      uDst += 1;
      vDst += 1;

      mSrc += components;
    }

    yStore += yStride;
    uStore += uStride;
    vStore += vStride;

    mSource += srcStride;
  }
}

#define XXXXToYCbCr444HWY_DECLARATION_R(pixelType) \
        void pixelType##ToYCbCr444HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                    const uint32_t width, const uint32_t height,\
                                    uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                    uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                    uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                    const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel8ToYCbCr444HWY<sparkyuv::PIXEL_##pixelType>(src, srcStride, width, height,\
                                                         yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
        }

XXXXToYCbCr444HWY_DECLARATION_R(RGBA)
XXXXToYCbCr444HWY_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444HWY_DECLARATION_R(ARGB)
XXXXToYCbCr444HWY_DECLARATION_R(ABGR)
XXXXToYCbCr444HWY_DECLARATION_R(BGRA)
XXXXToYCbCr444HWY_DECLARATION_R(BGR)
#endif

#undef XXXXToYCbCr444HWY_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void YCbCr444ToXRGB(uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                    const uint32_t width, const uint32_t height,
                    const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                    const uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                    const uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                    const float kr, const float kb, const SparkYuvColorRange colorRange) {
  const ScalableTag<uint8_t> du8;
  const Half<decltype(du8)> du8h;
  const Rebind<int16_t, decltype(du8h)> di16;
  const Rebind<uint16_t, decltype(du8h)> du16;
  using VI16 = Vec<decltype(di16)>;

  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);
  auto mUSrc = reinterpret_cast<const uint8_t *>(uPlane);
  auto mVSrc = reinterpret_cast<const uint8_t *>(vPlane);

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  const auto uvCorrection = Set(di16, biasUV);

  const auto uvCorrIY = Set(du8, biasY);
  const auto A = Set(du8, 255);

  float fCrCoeff = 0.f;
  float fCbCoeff = 0.f;
  float fGCoeff1 = 0.f;
  float fGCoeff2 = 0.f;
  const float flumaCoeff = 255.f / static_cast<float>(rangeY);
  ComputeInverseTransform(kr, kb, 255.f, static_cast<float>(rangeUV), fCrCoeff, fCbCoeff, fGCoeff1, fGCoeff2);

  const int precision = 6;

  const int CrCoeff = static_cast<int>(::roundf(fCrCoeff * static_cast<float>( 1 << precision )));
  const int CbCoeff = static_cast<int>(::roundf(fCbCoeff * static_cast<float>( 1 << precision )));
  const int GCoeff1 = static_cast<int>(::roundf(fGCoeff1 * static_cast<float>( 1 << precision )));
  const int GCoeff2 = static_cast<int>(::roundf(fGCoeff2 * static_cast<float>( 1 << precision )));

  const int iLumaCoeff = static_cast<int>(::roundf(flumaCoeff * static_cast<float>( 1 << precision )));

  const auto ivLumaCoeff = Set(du8, iLumaCoeff);
  const auto ivLumaCoeffh = Set(du8h, iLumaCoeff);
  const VI16 ivCrCoeff = Set(di16, CrCoeff);
  const VI16 ivCbCoeff = Set(di16, CbCoeff);
  const VI16 ivGCoeff1 = Set(di16, GCoeff1);
  const VI16 ivGCoeff2 = Set(di16, GCoeff2);

  const int lanes = Lanes(du8);

  const int components = getPixelTypeComponents(PixelType);

  const auto vZero = Zero(di16);

  for (int y = 0; y < height; ++y) {
    auto uSource = reinterpret_cast<const uint8_t *>(mUSrc);
    auto vSource = reinterpret_cast<const uint8_t *>(mVSrc);
    auto ySrc = reinterpret_cast<const uint8_t *>(mYSrc);
    auto store = reinterpret_cast<uint8_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const auto Y = Sub(LoadU(du8, ySrc), uvCorrIY);
      const auto U = LoadU(du8, uSource);
      const auto V = LoadU(du8, vSource);

      const auto cbl = Sub(BitCast(di16, PromoteLowerTo(du16, U)), uvCorrection);
      const auto crl = Sub(BitCast(di16, PromoteLowerTo(du16, V)), uvCorrection);
      const auto cbh = Sub(BitCast(di16, PromoteUpperTo(du16, U)), uvCorrection);
      const auto crh = Sub(BitCast(di16, PromoteUpperTo(du16, V)), uvCorrection);

      const auto Yh = BitCast(di16, WidenMulHigh(du8, Y, ivLumaCoeff));
      const auto rh = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedAdd(Mul(ivCrCoeff, crh), Yh), vZero)));
      const auto bh = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedAdd(Mul(ivCbCoeff, cbh), Yh), vZero)));
      const auto
          gh = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedSub(Yh,
                                                                        SaturatedAdd(Mul(ivGCoeff1, crh),
                                                                                     Mul(ivGCoeff2, cbh))), vZero)));

      const auto Yl = BitCast(di16, WidenMul(du8h, LowerHalf(Y), ivLumaCoeffh));
      const auto rl = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedAdd(Mul(ivCrCoeff, crl), Yl), vZero)));
      const auto bl = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedAdd(Mul(ivCbCoeff, cbl), Yl), vZero)));
      const auto
          gl = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedSub(Yl,
                                                                        SaturatedAdd(Mul(ivGCoeff1, crl),
                                                                                     Mul(ivGCoeff2, cbl))), vZero)));

      const auto r = Combine(du8, rh, rl);
      const auto g = Combine(du8, gh, gl);
      const auto b = Combine(du8, bh, bl);

      StoreRGBA<PixelType>(du8, store, r, g, b, A);

      store += lanes * components;
      ySrc += lanes;
      uSource += lanes;
      vSource += lanes;
    }

    for (; x < width; ++x) {
      const uint8_t uValue = reinterpret_cast<const uint8_t *>(uSource)[0];
      const uint8_t vValue = reinterpret_cast<const uint8_t *>(vSource)[0];

      int Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
      const int Cr = (static_cast<int>(vValue) - biasUV);
      const int Cb = (static_cast<int>(uValue) - biasUV);

      int R = (Y + CrCoeff * Cr) >> 6;
      int B = (Y + CbCoeff * Cb) >> 6;
      int G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> 6;

      SaturatedStoreRGBA<uint8_t, int, PixelType>(store, R, G, B, 255, 255);
      store += components;
      ySrc += 1;
      vSource += 1;
      uSource += 1;
    }

    mUSrc += uStride;
    mVSrc += vStride;
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define YCbCr444ToXXXX_DECLARATION_R(pixelType) \
    void YCbCr444To##pixelType##HWY(uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                         const uint32_t width, const uint32_t height,\
                         const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                         const uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                         const uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                         const float kr, const float kb, const SparkYuvColorRange colorRange) {\
                         YCbCr444ToXRGB<sparkyuv::PIXEL_##pixelType>(src, srcStride, width, height,\
                                                                     yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
                         }

YCbCr444ToXXXX_DECLARATION_R(RGBA)
YCbCr444ToXXXX_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr444ToXXXX_DECLARATION_R(ARGB)
YCbCr444ToXXXX_DECLARATION_R(ABGR)
YCbCr444ToXXXX_DECLARATION_R(BGRA)
YCbCr444ToXXXX_DECLARATION_R(BGR)
#endif

#undef YCbCr444ToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif