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

#if defined(SPARKYUV_YCbCr422_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YCbCr422_INL_H
#undef SPARKYUV_YCbCr422_INL_H
#else
#define SPARKYUV_YCbCr422_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include <algorithm>
#include <cmath>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void Pixel8ToYCbCr422(const uint8_t *SPARKYUV_RESTRICT src,
                      const uint32_t srcStride,
                      const uint32_t width, const uint32_t height,
                      uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                      uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                      uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                      const float kr, const float kb, const SparkYuvColorRange colorRange) {
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

  const auto scale = static_cast<float>( 1 << precision );
  const auto iBiasY = static_cast<uint16_t>((static_cast<float>(biasY) + 0.5f) * scale);
  const auto iBiasUV = static_cast<uint16_t>((static_cast<float>(biasUV) + 0.5f) * scale);

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);

  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const ScalableTag<uint16_t> du16;
  const ScalableTag<int16_t> di16;
  const ScalableTag<uint8_t> du8;
  const Half<decltype(di16)> dhi16;
  const Rebind<int32_t, decltype(dhi16)> d32;
  using VU8 = Vec<decltype(du8)>;
  using VU16 = Vec<decltype(di16)>;
  using V32 = Vec<decltype(d32)>;

  const Half<decltype(du8)> du8h;

  const int lanes = Lanes(du8);
  const int uvLanes = Lanes(du8h);

  const auto vBiasY = Set(d32, iBiasY);
  const auto vBiasUV = Set(d32, iBiasUV);

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

      const auto Rh = BitCast(di16, PromoteUpperTo(du16, R8));
      const auto Gh = BitCast(di16, PromoteUpperTo(du16, G8));
      const auto Bh = BitCast(di16, PromoteUpperTo(du16, B8));

      const auto Rl = BitCast(di16, PromoteLowerTo(du16, R8));
      const auto Gl = BitCast(di16, PromoteLowerTo(du16, G8));
      const auto Bl = BitCast(di16, PromoteLowerTo(du16, B8));

      V32 YhRh = vBiasY;
      V32 YhRl = WidenMulAccumulate(d32, Rh, vYR, vBiasY, YhRh);
      YhRl = WidenMulAccumulate(d32, Gh, vYG, YhRl, YhRh);
      YhRl = WidenMulAccumulate(d32, Bh, vYB, YhRl, YhRh);

      const auto
          Yh = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, YhRh), ShiftRightNarrow<8>(d32, YhRl)));

      V32 Chbh = vBiasUV;
      V32 Chbl = WidenMulAccumulate(d32, Rh, vCbR, vBiasUV, Chbh);
      Chbl = WidenMulAccumulate(d32, Gh, vCbG, Chbl, Chbh);
      Chbl = WidenMulAccumulate(d32, Bh, vCbB, Chbl, Chbh);

      const auto
          Chbf = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Chbh), ShiftRightNarrow<8>(d32, Chbl)));

      V32 Chrh = vBiasUV;
      V32 Chrl = WidenMulAccumulate(d32, Rh, vCrR, vBiasUV, Chrh);
      Chrl = WidenMulAccumulate(d32, Gh, vCrG, Chrl, Chrh);
      Chrl = WidenMulAccumulate(d32, Bh, vCrB, Chrl, Chrh);

      const auto
          Chrf = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Chrh), ShiftRightNarrow<8>(d32, Chrl)));

      V32 YlRh = vBiasY;
      V32 YlRl = WidenMulAccumulate(d32, Rl, vYR, vBiasY, YlRh);
      YlRl = WidenMulAccumulate(d32, Gl, vYG, YlRl, YlRh);
      YlRl = WidenMulAccumulate(d32, Bl, vYB, YlRl, YlRh);

      const auto
          Yl = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, YlRh), ShiftRightNarrow<8>(d32, YlRl)));

      V32 Clbh = vBiasUV;
      V32 Clbl = WidenMulAccumulate(d32, Rl, vCbR, vBiasUV, Clbh);
      Clbl = WidenMulAccumulate(d32, Gl, vCbG, Clbl, Clbh);
      Clbl = WidenMulAccumulate(d32, Bl, vCbB, Clbl, Clbh);

      const auto
          Clbf = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Clbh), ShiftRightNarrow<8>(d32, Clbl)));

      V32 Clrh = vBiasUV;
      V32 Clrl = WidenMulAccumulate(d32, Rl, vCrR, vBiasUV, Clrh);
      Clrl = WidenMulAccumulate(d32, Gl, vCrG, Clrl, Clrh);
      Clrl = WidenMulAccumulate(d32, Bl, vCrB, Clrl, Clrh);

      const auto
          Clrf = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Clrh), ShiftRightNarrow<8>(d32, Clrl)));

      const auto Cb = ShiftRightNarrow<1>(du16, SumsOf2(Combine(du8, DemoteTo(du8h, Chbf), DemoteTo(du8h, Clbf))));
      const auto Cr = ShiftRightNarrow<1>(du16, SumsOf2(Combine(du8, DemoteTo(du8h, Chrf), DemoteTo(du8h, Clrf))));

      StoreU(Combine(du8, DemoteTo(du8h, Yh), DemoteTo(du8h, Yl)), du8, yDst);
      StoreU(Cb, du8h, uDst);
      StoreU(Cr, du8h, vDst);

      yDst += lanes;
      uDst += uvLanes;
      vDst += uvLanes;

      mSrc += components * lanes;
    }

    for (; x < width; x += 2) {
      int r;
      int g;
      int b;

      LoadRGB<uint8_t, int, PixelType>(mSrc, r, g, b);

      int Y = ((r * YR + g * YG + b * YB + iBiasY) >> precision);

      yDst[0] = Y;
      yDst += 1;

      mSrc += components;

      if (x + 1 < width) {
        int r1;
        int g1;
        int b1;

        LoadRGB<uint8_t, int, PixelType>(mSrc, r1, g1, b1);

        int Y1 = ((r1 * YR + g1 * YG + b1 * YB + iBiasY) >> precision);

        r = (r + r1) >> 1;
        g = (g + g1) >> 1;
        b = (b + b1) >> 1;

        yDst[0] = Y1;
        yDst += 1;

        mSrc += components;
      }

      int Cb = ((-r * CbR - g * CbG + b * CbB + iBiasUV) >> precision);
      int Cr = ((r * CrR - g * CrG - b * CrB + iBiasUV) >> precision);

      uDst[0] = Cb;
      vDst[0] = Cr;

      uDst += 1;
      vDst += 1;
    }

    yStore += yStride;
    uStore += uStride;
    vStore += vStride;

    mSource += srcStride;
  }
}

#define XXXXToYCbCr422HWY_DECLARATION_R(pixelType) \
        void pixelType##ToYCbCr422HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                   const uint32_t width, const uint32_t height,\
                                   uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                   uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                   uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                   const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel8ToYCbCr422<sparkyuv::PIXEL_##pixelType>(src, srcStride, width, height,\
                                                      yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
        }

XXXXToYCbCr422HWY_DECLARATION_R(RGBA)
XXXXToYCbCr422HWY_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr422HWY_DECLARATION_R(ARGB)
XXXXToYCbCr422HWY_DECLARATION_R(ABGR)
XXXXToYCbCr422HWY_DECLARATION_R(BGRA)
XXXXToYCbCr422HWY_DECLARATION_R(BGR)
#endif

#undef YCbCr444ToXXXX_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void YCbCr422ToPixel8(uint8_t *SPARKYUV_RESTRICT dst,
                      const uint32_t dstStride,
                      const uint32_t width,
                      const uint32_t height,
                      const uint8_t *SPARKYUV_RESTRICT yPlane,
                      const uint32_t yStride,
                      const uint8_t *SPARKYUV_RESTRICT uPlane,
                      const uint32_t uStride,
                      const uint8_t *SPARKYUV_RESTRICT vPlane,
                      const uint32_t vStride,
                      const float kr,
                      const float kb,
                      const SparkYuvColorRange colorRange) {
  const ScalableTag<uint8_t> du8;
  const Half<decltype(du8)> du8h;
  const Rebind<int16_t, decltype(du8h)> di16;
  const RebindToUnsigned<decltype(di16)> du16;
  using VU16 = Vec<decltype(di16)>;

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  const VU16 uvCorrection = Set(di16, biasUV);
  const auto uvCorrIY = Set(du8, biasY);

  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);
  auto mUSrc = reinterpret_cast<const uint8_t *>(uPlane);
  auto mVSrc = reinterpret_cast<const uint8_t *>(vPlane);

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
  const VU16 ivCrCoeff = Set(di16, CrCoeff);
  const VU16 ivCbCoeff = Set(di16, CbCoeff);
  const VU16 ivGCoeff1 = Set(di16, GCoeff1);
  const VU16 ivGCoeff2 = Set(di16, GCoeff2);

  const VU16 vZero = Zero(di16);

  const int lanes = Lanes(du8);
  const int uvLanes = Lanes(du8h);

  const int components = getPixelTypeComponents(PixelType);

  for (int y = 0; y < height; ++y) {
    auto uSource = reinterpret_cast<const uint8_t *>(mUSrc);
    auto vSource = reinterpret_cast<const uint8_t *>(mVSrc);
    auto ySrc = reinterpret_cast<const uint8_t *>(mYSrc);
    auto store = reinterpret_cast<uint8_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const auto luma8 = Sub(LoadU(du8, ySrc), uvCorrIY);
      const VU16 ulFull = Sub(BitCast(di16, PromoteTo(du16, LoadU(du8h, uSource))), uvCorrection);
      const VU16 vlFull = Sub(BitCast(di16, PromoteTo(du16, LoadU(du8h, vSource))), uvCorrection);

      const auto cbl = InterleaveLower(ulFull, ulFull);
      const auto cbh = InterleaveUpper(di16, ulFull, ulFull);
      const auto crl = InterleaveLower(vlFull, vlFull);
      const auto crh = InterleaveUpper(di16, vlFull, vlFull);

      const auto Yh = BitCast(di16, WidenMulHigh(du8, luma8, ivLumaCoeff));
      const auto rh = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedAdd(Mul(ivCrCoeff, crh), Yh), vZero)));
      const auto bh = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedAdd(Mul(ivCbCoeff, cbh), Yh), vZero)));
      const auto
          gh = ShiftRightNarrow<6>(du16, BitCast(du16, Max(SaturatedSub(Yh,
                                                                        SaturatedAdd(Mul(ivGCoeff1, crh),
                                                                                     Mul(ivGCoeff2, cbh))), vZero)));

      const auto Yl = BitCast(di16, WidenMul(du8h, LowerHalf(luma8), ivLumaCoeffh));
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

      uSource += uvLanes;
      vSource += uvLanes;
    }

    for (; x < width; x += 2) {
      const uint8_t uValue = reinterpret_cast<const uint8_t *>(uSource)[0];
      const uint8_t vValue = reinterpret_cast<const uint8_t *>(vSource)[0];

      int Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
      const int Cr = (static_cast<int>(vValue) - biasUV);
      const int Cb = (static_cast<int>(uValue) - biasUV);

      int R = (Y + CrCoeff * Cr) >> precision;
      int B = (Y + CbCoeff * Cb) >> precision;
      int G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

      SaturatedStoreRGBA<uint8_t, int, PixelType>(store, R, G, B, 255, 255);

      store += components;
      ySrc += 1;

      if (x + 1 < width) {
        Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
        R = (Y + CrCoeff * Cr) >> precision;
        B = (Y + CbCoeff * Cb) >> precision;
        G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

        SaturatedStoreRGBA<uint8_t, int, PixelType>(store, R, G, B, 255, 255);
        store += components;
        ySrc += 1;
      }

      vSource += 1;
      uSource += 1;
    }

    mUSrc += uStride;
    mVSrc += vStride;
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define YCbCr422ToXXXXHWY_DECLARATION_R(pixelType) \
        void YCbCr422To##pixelType##HWY(uint8_t *SPARKYUV_RESTRICT dst,const uint32_t dstStride,\
                                      const uint32_t width,const uint32_t height,\
                                      const uint8_t *SPARKYUV_RESTRICT yPlane,const uint32_t yStride,\
                                      const uint8_t *SPARKYUV_RESTRICT uPlane,const uint32_t uStride,\
                                      const uint8_t *SPARKYUV_RESTRICT vPlane,const uint32_t vStride,\
                                      const float kr,const float kb, const SparkYuvColorRange colorRange) {\
          YCbCr422ToPixel8<PIXEL_##pixelType>(dst, dstStride, width, height,\
                                     yPlane, yStride, uPlane, uStride, vPlane, vStride,\
                                     kr, kb, colorRange);\
        }

YCbCr422ToXXXXHWY_DECLARATION_R(RGBA)
YCbCr422ToXXXXHWY_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
YCbCr422ToXXXXHWY_DECLARATION_R(ARGB)
YCbCr422ToXXXXHWY_DECLARATION_R(ABGR)
YCbCr422ToXXXXHWY_DECLARATION_R(BGRA)
YCbCr422ToXXXXHWY_DECLARATION_R(BGR)
#endif

#undef YCbCr422ToXXXXHWY_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif