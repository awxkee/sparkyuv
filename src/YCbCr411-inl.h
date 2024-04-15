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

#if defined(SPARKYUV_YCbCr411_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YCbCr411_INL_H
#undef SPARKYUV_YCbCr411_INL_H
#else
#define SPARKYUV_YCbCr411_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include <algorithm>
#include <cmath>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, SparkYuvChromaSubsample chromaSubsample>
void Pixel8ToYCbCr411(const uint8_t *SPARKYUV_RESTRICT src,
                      const uint32_t srcStride,
                      const uint32_t width, const uint32_t height,
                      uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                      uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                      uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                      const float kr, const float kb, const SparkYuvColorRange colorRange) {
  static_assert(chromaSubsample == YUV_SAMPLE_411 || chromaSubsample == YUV_SAMPLE_410, "Not supported chroma subsample");
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
  const RebindToUnsigned<decltype(d32)> du32;
  using VU8 = Vec<decltype(du8)>;
  using VU16 = Vec<decltype(di16)>;
  using V32 = Vec<decltype(d32)>;
  const Rebind<uint8_t, decltype(du32)> du8CbCr;

  const Half<decltype(du8)> du8h;

  const int lanes = Lanes(du8);
  const int uvLanes = lanes / 4;

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

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;
  const uint32_t lastY = height - 1 - 4;

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

      V32 YlRh = vBiasY;
      V32 YlRl = WidenMulAccumulate(d32, Rl, vYR, vBiasY, YlRh);
      YlRl = WidenMulAccumulate(d32, Gl, vYG, YlRl, YlRh);
      YlRl = WidenMulAccumulate(d32, Bl, vYB, YlRl, YlRh);

      const auto
          Yl = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, YlRh), ShiftRightNarrow<8>(d32, YlRl)));

      StoreU(Combine(du8, DemoteTo(du8h, Yh), DemoteTo(du8h, Yl)), du8, yDst);

      if (chromaSubsample == YUV_SAMPLE_411 || ((y % 4 == 0 || y > lastY) && chromaSubsample == YUV_SAMPLE_410)) {
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

        const auto
            Cb = ShiftRightNarrow<2>(du32,
                                     SumsOf2(SumsOf2(Combine(du8, DemoteTo(du8h, Chbf), DemoteTo(du8h, Clbf)))));
        const auto
            Cr = ShiftRightNarrow<2>(du32,
                                     SumsOf2(SumsOf2(Combine(du8, DemoteTo(du8h, Chrf), DemoteTo(du8h, Clrf)))));

        StoreU(DemoteTo(du8CbCr, Cb), du8CbCr, uDst);
        StoreU(DemoteTo(du8CbCr, Cr), du8CbCr, vDst);
      }

      yDst += lanes;
      uDst += uvLanes;
      vDst += uvLanes;

      mSrc += components * lanes;
    }

    for (; x < width; x += 4) {
      int r;
      int g;
      int b;

      LoadRGB<uint8_t, int, PixelType>(mSrc, r, g, b);

      int r1 = r, g1 = g, b1 = b;
      int r2 = r, g2 = g, b2 = b;
      int r3 = r, g3 = g, b3 = b;

      int Y = ((r * YR + g * YG + b * YB + iBiasY) >> precision);

      yDst[0] = Y;
      yDst += 1;

      mSrc += components;

      if (x + 1 < width) {
        LoadRGB<uint8_t, int, PixelType>(mSrc, r1, g1, b1);

        int Y1 = ((r1 * YR + g1 * YG + b1 * YB + iBiasY) >> precision);

        yDst[0] = Y1;
        yDst += 1;

        mSrc += components;
      }

      if (x + 2 < width) {
        LoadRGB<uint8_t, int, PixelType>(mSrc, r2, g2, b2);

        int Y1 = ((r2 * YR + g2 * YG + b2 * YB + iBiasY) >> precision);

        yDst[0] = Y1;
        yDst += 1;

        mSrc += components;
      }

      if (x + 3 < width) {
        LoadRGB<uint8_t, int, PixelType>(mSrc, r3, g3, b3);

        int Y1 = ((r3 * YR + g3 * YG + b3 * YB + iBiasY) >> precision);

        yDst[0] = Y1;
        yDst += 1;

        mSrc += components;
      }

      if (chromaSubsample == YUV_SAMPLE_411 || ((y % 4 == 0 || y > lastY) && chromaSubsample == YUV_SAMPLE_410)) {
        r = (r + r1 + r2 + r3) >> 2;
        g = (g + g1 + g2 + g3) >> 2;
        b = (b + b1 + b2 + b3) >> 2;

        int Cb = ((-r * CbR - g * CbG + b * CbB + iBiasUV) >> precision);
        int Cr = ((r * CrR - g * CrG - b * CrB + iBiasUV) >> precision);

        uDst[0] = Cb;
        vDst[0] = Cr;
      }

      uDst += 1;
      vDst += 1;
    }

    yStore += yStride;
    if (chromaSubsample == YUV_SAMPLE_411) {
      uStore += uStride;
      vStore += vStride;
    } else if (chromaSubsample == YUV_SAMPLE_410) {
      if (y % 4 == 0) {
        uStore += uStride;
        vStore += vStride;
      }
    }

    mSource += srcStride;
  }
}

#define XXXXToYCbCr411HWY_DECLARATION_R(pixelType, chroma) \
        void pixelType##ToYCbCr411HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                   const uint32_t width, const uint32_t height,\
                                   uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                   uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                   uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                   const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel8ToYCbCr411<sparkyuv::PIXEL_##pixelType, chroma>(src, srcStride, width, height,\
                                                      yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
        }

XXXXToYCbCr411HWY_DECLARATION_R(RGBA, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411HWY_DECLARATION_R(RGB, sparkyuv::YUV_SAMPLE_411)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr411HWY_DECLARATION_R(ARGB, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411HWY_DECLARATION_R(ABGR, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411HWY_DECLARATION_R(BGRA, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411HWY_DECLARATION_R(BGR, sparkyuv::YUV_SAMPLE_411)
#endif

#undef XXXXToYCbCr411HWY_DECLARATION_R

#define XXXXToYCbCr410HWY_DECLARATION_R(pixelType, chroma) \
        void pixelType##ToYCbCr410HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                   const uint32_t width, const uint32_t height,\
                                   uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                   uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                   uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                   const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel8ToYCbCr411<sparkyuv::PIXEL_##pixelType, chroma>(src, srcStride, width, height,\
                                                      yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
        }

XXXXToYCbCr410HWY_DECLARATION_R(RGBA, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410HWY_DECLARATION_R(RGB, sparkyuv::YUV_SAMPLE_410)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr410HWY_DECLARATION_R(ARGB, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410HWY_DECLARATION_R(ABGR, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410HWY_DECLARATION_R(BGRA, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410HWY_DECLARATION_R(BGR, sparkyuv::YUV_SAMPLE_410)
#endif

#undef XXXXToYCbCr411HWY_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, SparkYuvChromaSubsample chromaSubsample>
void YCbCr411ToPixel8(uint8_t *SPARKYUV_RESTRICT dst,
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
  static_assert(chromaSubsample == YUV_SAMPLE_411 || chromaSubsample == YUV_SAMPLE_410, "Not supported chroma subsample");
  const ScalableTag<uint8_t> du8;
  const Half<decltype(du8)> du8h;
  const Half<decltype(du8h)> du8hh;
  const Rebind<int16_t, decltype(du8h)> di16;
  const RebindToUnsigned<decltype(di16)> du16;
  const Rebind<uint16_t, decltype(du8hh)> du16hh;
  const RebindToSigned<decltype(du16hh)> di16hh;
  using VU16 = Vec<decltype(di16)>;

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  const auto uvCorrection = Set(di16hh, biasUV);
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
  const int uvLanes = lanes / 4;

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  for (int y = 0; y < height; ++y) {
    auto uSource = reinterpret_cast<const uint8_t *>(mUSrc);
    auto vSource = reinterpret_cast<const uint8_t *>(mVSrc);
    auto ySrc = reinterpret_cast<const uint8_t *>(mYSrc);
    auto store = reinterpret_cast<uint8_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const RepartitionToWide<decltype(di16hh)> di32hh;
      const Twice<decltype(di32hh)> di32h;
      const auto luma8 = Sub(LoadU(du8, ySrc), uvCorrIY);
      const auto ulFull = Sub(BitCast(di16hh, PromoteTo(du16hh, LoadU(du8hh, uSource))), uvCorrection);
      const auto vlFull = Sub(BitCast(di16hh, PromoteTo(du16hh, LoadU(du8hh, vSource))), uvCorrection);

      /**
       * Since we're need to duplicate one pixel to 4 times,
       * to broadcast vector from x1 x2 x3 x3 -> x1 x1 x1 x1 x2 x2 x2 x2 x3 x3 x3 x3 x4 x4 x4 x4
       * we need to zip at first to receive x1 x1 x2 x2 x3 x3 x4 x4
       * then reinterpret cast to int32 to pack into -> (x1 x1) (x2 x2) (x3 x3) (x4 x4)
       * interleave int32 to receive (x1 x1) (x1 x1) (x2 x2) (x2 x2) (x3 x3) (x3 x3) (x4 x4) (x4 x4)
       * and reinterpret cast back into int16
       */

      const auto cbll32 = BitCast(di32hh, InterleaveLower(ulFull, ulFull));
      auto cbll = Combine(di32h, cbll32, cbll32);
      cbll = InterleaveLower(cbll, cbll);
      const auto cbl = BitCast(di16, cbll);
      const auto cbhh32 = BitCast(di32hh, InterleaveUpper(di16hh, ulFull, ulFull));
      auto cbhh = Combine(di32h, cbhh32, cbhh32);
      cbhh = InterleaveLower(cbhh, cbhh);
      const auto cbh = BitCast(di16, cbhh);
      const auto crll32 = BitCast(di32hh, InterleaveLower(vlFull, vlFull));
      auto crll = Combine(di32h, crll32, crll32);
      crll = InterleaveLower(crll, crll);
      const auto crl = BitCast(di16, crll);
      const auto crhh32 = BitCast(di32hh, InterleaveUpper(di16hh, vlFull, vlFull));
      auto crhh = Combine(di32h, crhh32, crhh32);
      crhh = InterleaveLower(crhh, crhh);
      const auto crh = BitCast(di16, crhh);

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

    for (; x < width; x += 4) {
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

      if (x + 2 < width) {
        Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
        R = (Y + CrCoeff * Cr) >> precision;
        B = (Y + CbCoeff * Cb) >> precision;
        G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

        SaturatedStoreRGBA<uint8_t, int, PixelType>(store, R, G, B, 255, 255);
        store += components;
        ySrc += 1;
      }

      if (x + 3 < width) {
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

    if (chromaSubsample == YUV_SAMPLE_411) {
      mUSrc += uStride;
      mVSrc += vStride;
    } else if (chromaSubsample == YUV_SAMPLE_410) {
      if (y % 4 == 0) {
        mUSrc += uStride;
        mVSrc += vStride;
      }
    }
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define YCbCr411ToXXXXHWY_DECLARATION_R(pixelType, chroma) \
        void YCbCr411To##pixelType##HWY(uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                      const uint32_t width,const uint32_t height,\
                                      const uint8_t *SPARKYUV_RESTRICT yPlane,const uint32_t yStride,\
                                      const uint8_t *SPARKYUV_RESTRICT uPlane,const uint32_t uStride,\
                                      const uint8_t *SPARKYUV_RESTRICT vPlane,const uint32_t vStride,\
                                      const float kr,const float kb, const SparkYuvColorRange colorRange) {\
          YCbCr411ToPixel8<PIXEL_##pixelType, chroma>(dst, dstStride, width, height,\
                                     yPlane, yStride, uPlane, uStride, vPlane, vStride,\
                                     kr, kb, colorRange);\
        }

YCbCr411ToXXXXHWY_DECLARATION_R(RGBA, sparkyuv::YUV_SAMPLE_411)
YCbCr411ToXXXXHWY_DECLARATION_R(RGB, sparkyuv::YUV_SAMPLE_411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411ToXXXXHWY_DECLARATION_R(ARGB, sparkyuv::YUV_SAMPLE_411)
YCbCr411ToXXXXHWY_DECLARATION_R(ABGR, sparkyuv::YUV_SAMPLE_411)
YCbCr411ToXXXXHWY_DECLARATION_R(BGRA, sparkyuv::YUV_SAMPLE_411)
YCbCr411ToXXXXHWY_DECLARATION_R(BGR, sparkyuv::YUV_SAMPLE_411)
#endif

#undef YCbCr411ToXXXXHWY_DECLARATION_R

#define YCbCr410ToXXXXHWY_DECLARATION_R(pixelType, chroma) \
        void YCbCr410To##pixelType##HWY(uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                      const uint32_t width,const uint32_t height,\
                                      const uint8_t *SPARKYUV_RESTRICT yPlane,const uint32_t yStride,\
                                      const uint8_t *SPARKYUV_RESTRICT uPlane,const uint32_t uStride,\
                                      const uint8_t *SPARKYUV_RESTRICT vPlane,const uint32_t vStride,\
                                      const float kr,const float kb, const SparkYuvColorRange colorRange) {\
          YCbCr411ToPixel8<PIXEL_##pixelType, chroma>(dst, dstStride, width, height,\
                                     yPlane, yStride, uPlane, uStride, vPlane, vStride,\
                                     kr, kb, colorRange);\
        }

YCbCr410ToXXXXHWY_DECLARATION_R(RGBA, sparkyuv::YUV_SAMPLE_410)
YCbCr410ToXXXXHWY_DECLARATION_R(RGB, sparkyuv::YUV_SAMPLE_410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410ToXXXXHWY_DECLARATION_R(ARGB, sparkyuv::YUV_SAMPLE_410)
YCbCr410ToXXXXHWY_DECLARATION_R(ABGR, sparkyuv::YUV_SAMPLE_410)
YCbCr410ToXXXXHWY_DECLARATION_R(BGRA, sparkyuv::YUV_SAMPLE_410)
YCbCr410ToXXXXHWY_DECLARATION_R(BGR, sparkyuv::YUV_SAMPLE_410)
#endif

#undef YCbCr410ToXXXXHWY_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif
