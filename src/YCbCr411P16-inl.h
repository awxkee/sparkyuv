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

#if defined(SPARKYUV_YCbCr411P16_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YCbCr411P16_INL_H
#undef SPARKYUV_YCbCr411P16_INL_H
#else
#define SPARKYUV_YCbCr411P16_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include <algorithm>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, int bitDepth, SparkYuvChromaSubsample chromaSubsample>
void Pixel16ToYCbCr411HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                          const uint32_t width, const uint32_t height,
                          uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                          uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                          uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                          const float kr, const float kb, const SparkYuvColorRange colorRange) {
  static_assert(chromaSubsample == YUV_SAMPLE_411 || chromaSubsample == YUV_SAMPLE_410,
                "Not supported chroma subsample");
  static_assert(bitDepth >= 8, "Invalid bit depth");
  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, biasY, biasUV, rangeY, rangeUV);

  uint16_t YR, YG, YB;
  uint16_t CbR, CbG, CbB;
  uint16_t CrR, CrG, CrB;

  const int precision = 8;

  const int maxColors = static_cast<int>(::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

  float zYR, zYG, zYB;
  float zCbR, zCbG, zCbB;
  float zCrR, zCrG, zCrB;

  ComputeTransform(kr, kb, static_cast<float>(biasY), static_cast<float>(biasUV),
                   static_cast<float>(rangeY), static_cast<float>(rangeUV),
                   static_cast<float>(maxColors), zYR, zYG, zYB, zCbR, zCbG, zCbB, zCrR, zCrG, zCrB);

  ComputeTransformIntegers(kr, kb, static_cast<float>(biasY), static_cast<float>(biasUV),
                           static_cast<float>(rangeY), static_cast<float>(rangeUV),
                           static_cast<float>(maxColors), precision, YR, YG, YB, CbR, CbG, CbB, CrR, CrG, CrB);

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);

  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const ScalableTag<int16_t> d16;
  const RebindToUnsigned<decltype(d16)> du16;
  const Half<decltype(d16)> dh16;
  const Rebind<int32_t, decltype(dh16)> d32;
  const RebindToUnsigned<decltype(d32)> du32;
  using V16 = Vec<decltype(d16)>;
  using V32 = Vec<decltype(d32)>;
  using VU16 = Vec<decltype(du16)>;

  const auto scale = static_cast<float>( 1 << precision );
  const auto iBiasY = static_cast<int>((static_cast<float>(biasY) + 0.5f) * scale);
  const auto iBiasUV = static_cast<int>((static_cast<float>(biasUV) + 0.5f) * scale);

  const int lanes = Lanes(d16);
  const int uvLanes = lanes / 4;

  const auto coeffTag = d16;

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

  const int cutOffY = rangeY + biasY;

  const int components = getPixelTypeComponents(PixelType);
  const uint32_t lastY = height - 1 - 4;

  const int lanesForward = 4;

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<uint16_t *>(yStore);
    auto uDst = reinterpret_cast<uint16_t *>(uStore);
    auto vDst = reinterpret_cast<uint16_t *>(vStore);

    auto mSrc = reinterpret_cast<const uint16_t *>(mSource);

    for (; x + lanes < width; x += lanes) {
      V16 R;
      V16 G;
      V16 B;
      V16 A;

      LoadRGBA<PixelType>(d16, reinterpret_cast<const int16_t *>(mSrc), R, G, B, A);

      V32 YRh = vBiasY;
      V32 YRl = WidenMulAccumulate(d32, R, vYR, vBiasY, YRh);
      YRl = WidenMulAccumulate(d32, G, vYG, YRl, YRh);
      YRl = WidenMulAccumulate(d32, B, vYB, YRl, YRh);

      const auto
          Y = BitCast(du16, Combine(d16, ShiftRightNarrow<8>(d32, YRh), ShiftRightNarrow<8>(d32, YRl)));
      StoreU(Y, du16, yDst);

      if (chromaSubsample == YUV_SAMPLE_411 || ((y % 4 == 0 || y > lastY) && chromaSubsample == YUV_SAMPLE_410)) {
        V32 Cbh = vBiasUV;
        V32 Cbl = WidenMulAccumulate(d32, R, vCbR, vBiasUV, Cbh);
        Cbl = WidenMulAccumulate(d32, G, vCbG, Cbl, Cbh);
        Cbl = WidenMulAccumulate(d32, B, vCbB, Cbl, Cbh);

        const auto
            Cb = BitCast(du16, Combine(d16, ShiftRightNarrow<8>(d32, Cbh), ShiftRightNarrow<8>(d32, Cbl)));

        V32 Crh = vBiasUV;
        V32 Crl = WidenMulAccumulate(d32, R, vCrR, vBiasUV, Crh);
        Crl = WidenMulAccumulate(d32, G, vCrG, Crl, Crh);
        Crl = WidenMulAccumulate(d32, B, vCrB, Crl, Crh);

        const auto
            Cr = BitCast(du16, Combine(d16, ShiftRightNarrow<8>(d32, Crh), ShiftRightNarrow<8>(d32, Crl)));

        const RepartitionToWide<decltype(du32)> du64;
        const Rebind<uint16_t, decltype(du64)> du16cb;

        const auto cbh = ShiftRightNarrow<2>(du64, SumsOf2(SumsOf2(Cb)));
        const auto crh = ShiftRightNarrow<2>(du64, SumsOf2(SumsOf2(Cr)));
        StoreU(DemoteTo(du16cb, cbh), du16cb, uDst);
        StoreU(DemoteTo(du16cb, crh), du16cb, vDst);
      }

      yDst += lanes;
      uDst += uvLanes;
      vDst += uvLanes;
      mSrc += components * lanes;
    }

    for (; x < width; x += lanesForward) {
      int r;
      int g;
      int b;

      LoadRGB<uint16_t, int, PixelType>(mSrc, r, g, b);

      int r1 = r, g1 = g, b1 = b;
      int r2 = r, g2 = g, b2 = b;
      int r3 = r, g3 = g, b3 = b;

      int Y = ((r * static_cast<int>(YR) + g * static_cast<int>(YG) + b * static_cast<int>(YB) + iBiasY) >> precision);
      yDst[0] = std::clamp(Y, static_cast<int>(biasY), cutOffY);
      yDst += 1;
      mSrc += components;

      if (x + 1 < width) {
        LoadRGB<uint16_t, int, PixelType>(mSrc, r1, g1, b1);

        int Y1 = ((r1 * YR + g1 * YG + b1 * YB + iBiasY) >> precision);

        yDst[0] = Y1;
        yDst += 1;

        mSrc += components;
      }

      if (x + 2 < width) {
        LoadRGB<uint16_t, int, PixelType>(mSrc, r2, g2, b2);

        int Y1 = ((r2 * YR + g2 * YG + b2 * YB + iBiasY) >> precision);

        yDst[0] = Y1;
        yDst += 1;

        mSrc += components;
      }

      if (x + 3 < width) {
        LoadRGB<uint16_t, int, PixelType>(mSrc, r3, g3, b3);

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

    if (chromaSubsample == YUV_SAMPLE_411 || (y % 4 == 0 && chromaSubsample == YUV_SAMPLE_410)) {
      vStore += vStride;
      uStore += uStride;
    }
    yStore += yStride;

    mSource += srcStride;
  }
}

#define XXXXToYCbCr411PHWY_DECLARATION_R(pixelType, bit, yuvname, chroma) \
        void pixelType##To##yuvname##P##bit##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                            const uint32_t width, const uint32_t height,\
                                            uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                            uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                            uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel16ToYCbCr411HWY<sparkyuv::PIXEL_##pixelType, bit, chroma>(src, srcStride, width, height,\
                                                          yPlane, yStride, uPlane, uStride, vPlane, vStride,     \
                                                          kr, kb, colorRange);\
        }

XXXXToYCbCr411PHWY_DECLARATION_R(RGBA, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(RGB, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr411PHWY_DECLARATION_R(ARGB, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(ABGR, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(BGRA, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(BGR, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#endif

XXXXToYCbCr411PHWY_DECLARATION_R(RGBA, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(RGB, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr411PHWY_DECLARATION_R(ARGB, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(ABGR, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(BGRA, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
XXXXToYCbCr411PHWY_DECLARATION_R(BGR, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#endif

#undef XXXXToYCbCr411PHWY_DECLARATION_R

#define XXXXToYCbCr410PHWY_DECLARATION_R(pixelType, bit, yuvname, chroma) \
        void pixelType##To##yuvname##P##bit##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                            const uint32_t width, const uint32_t height,\
                                            uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                            uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                            uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel16ToYCbCr411HWY<sparkyuv::PIXEL_##pixelType, bit, chroma>(src, srcStride, width, height,\
                                                          yPlane, yStride, uPlane, uStride, vPlane, vStride,     \
                                                          kr, kb, colorRange);\
        }

XXXXToYCbCr410PHWY_DECLARATION_R(RGBA, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(RGB, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr410PHWY_DECLARATION_R(ARGB, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(ABGR, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(BGRA, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(BGR, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#endif

XXXXToYCbCr410PHWY_DECLARATION_R(RGBA, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(RGB, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr410PHWY_DECLARATION_R(ARGB, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(ABGR, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(BGRA, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
XXXXToYCbCr410PHWY_DECLARATION_R(BGR, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#endif

#undef XXXXToYCbCr410PHWY_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, int bitDepth, SparkYuvChromaSubsample chromaSubsample>
void YCbCr411P16ToXRGB(uint16_t *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                       const uint32_t width, const uint32_t height,
                       const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                       const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                       const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                       const float kr, const float kb, const SparkYuvColorRange colorRange) {
  static_assert(chromaSubsample == YUV_SAMPLE_411 || chromaSubsample == YUV_SAMPLE_410,
                "Not supported chroma subsample");
  static_assert(bitDepth >= 8, "Invalid bit depth");
  const ScalableTag<uint16_t> d16;
  const RebindToSigned<decltype(d16)> di16;
  const Half<decltype(d16)> dh16;
  const Half<decltype(dh16)> dhh16;
  const RebindToSigned<decltype(dhh16)> dhhi16;
  const Rebind<int32_t, decltype(dh16)> d32;
  using V32 = Vec<decltype(d32)>;
  using V16 = Vec<decltype(d16)>;
  using VI16 = Vec<decltype(di16)>;

  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);
  auto mUSrc = reinterpret_cast<const uint8_t *>(uPlane);
  auto mVSrc = reinterpret_cast<const uint8_t *>(vPlane);
  auto dst = reinterpret_cast<uint8_t *>(rgbaData);

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, biasY, biasUV, rangeY, rangeUV);

  const int maxColors = static_cast<int>(::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

  const auto uvCorrection = Set(dhhi16, biasUV);
  const V16 vAlpha = Set(d16, maxColors);
  const auto uvCorrIY = Set(di16, biasY);

  float fCrCoeff = 0.f;
  float fCbCoeff = 0.f;
  float fGCoeff1 = 0.f;
  float fGCoeff2 = 0.f;
  const float flumaCoeff = static_cast<float>(maxColors) / static_cast<float>(rangeY);
  ComputeInverseTransform(kr, kb,
                          static_cast<float>(maxColors),
                          static_cast<float>(rangeUV),
                          fCrCoeff, fCbCoeff, fGCoeff1, fGCoeff2);

  const int precision = 6;

  const int CrCoeff = static_cast<int>(::roundf(fCrCoeff * static_cast<float>( 1 << precision )));
  const int CbCoeff = static_cast<int>(::roundf(fCbCoeff * static_cast<float>( 1 << precision )));
  const int GCoeff1 = static_cast<int>(::roundf(fGCoeff1 * static_cast<float>( 1 << precision )));
  const int GCoeff2 = static_cast<int>(::roundf(fGCoeff2 * static_cast<float>( 1 << precision )));

  const int iLumaCoeff = static_cast<int>(::roundf(flumaCoeff * static_cast<float>( 1 << precision )));

  const auto ivGCoeff1 = Set(di16, -GCoeff1);
  const auto ivLumaCoeff = Set(di16, iLumaCoeff);
  const auto ivCrCoeff = Set(di16, CrCoeff);
  const auto ivCbCoeff = Set(di16, CbCoeff);
  const auto ivGCoeff2 = Set(di16, -GCoeff2);
  const auto sum0 = Zero(d32);

  const int lanes = Lanes(d16);
  const int lanesForward = 4;
  const int uvLanes = lanes / 4;

  const int components = getPixelTypeComponents(PixelType);

  for (int y = 0; y < height; ++y) {
    auto CbSource = reinterpret_cast<const uint16_t *>(mUSrc);
    auto CrSource = reinterpret_cast<const uint16_t *>(mVSrc);
    auto ySrc = reinterpret_cast<const uint16_t *>(mYSrc);
    auto store = reinterpret_cast<uint16_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const auto Y = Sub(BitCast(di16, LoadU(d16, ySrc)), uvCorrIY);

      auto cbh16 = LoadU(dhh16, CbSource);
      auto crh16 = LoadU(dhh16, CrSource);
      const auto ulShort = Sub(BitCast(dhhi16, cbh16), uvCorrection);
      const auto vlShort = Sub(BitCast(dhhi16, crh16), uvCorrection);
      const RebindToSigned<decltype(dh16)> dhi16;

      const auto ulIntLower = InterleaveLower(ulShort, ulShort);
      const auto ulLShort = Combine(dhi16, ulIntLower, ulIntLower);
      const auto ulIntUpper = InterleaveUpper(dhhi16, ulShort, ulShort);
      const auto ulHShort = Combine(dhi16, ulIntUpper, ulIntUpper);
      const auto cb = Combine(di16, ulHShort, ulLShort);

      const auto vlIntLower = InterleaveLower(vlShort, vlShort);
      const auto vlLShort = Combine(dhi16, vlIntLower, vlIntLower);
      const auto vlIntUpper = InterleaveUpper(dhhi16, vlShort, vlShort);
      const auto vlHShort = Combine(dhi16, vlIntUpper, vlIntUpper);
      const auto cr = Combine(di16, vlHShort, vlLShort);

      V32 Yh = Zero(d32);
      const V32 Yl = WidenMulAccumulate(d32, Y, ivLumaCoeff, sum0, Yh);

      V32 Crh = Yh;
      const V32 Crl = WidenMulAccumulate(d32, cr, ivCrCoeff, Yl, Crh);

      V32 Cbh = Yh;
      const V32 Cbl = WidenMulAccumulate(d32, cb, ivCbCoeff, Yl, Cbh);

      V32 G1h = Yh;
      const V32 G1l = WidenMulAccumulate(d32, cr, ivGCoeff1, Yl, G1h);

      V32 G2h = G1h;
      const V32 G2l = WidenMulAccumulate(d32, cb, ivGCoeff2, G1l, G2h);

      const V32 rh = Crh;
      const V32 rl = Crl;
      const V32 bh = Cbh;
      const V32 bl = Cbl;

      const auto gl = G2l;
      const auto gh = G2h;

      // In 12 bit overflow is highly likely so there is a need to handle it slightly in another way
      V16 r, g, b;
      if (bitDepth == 12) {
        r = Combine(d16, BitCast(dh16, ShiftRightNarrow<6>(d32, rh)), BitCast(dh16, ShiftRightNarrow<6>(d32, rl)));
        g = Combine(d16, BitCast(dh16, ShiftRightNarrow<6>(d32, gh)), BitCast(dh16, ShiftRightNarrow<6>(d32, gl)));
        b = Combine(d16, BitCast(dh16, ShiftRightNarrow<6>(d32, bh)), BitCast(dh16, ShiftRightNarrow<6>(d32, bl)));
      } else {
        r = ShiftRight<6>(Combine(d16, DemoteTo(dh16, rh), DemoteTo(dh16, rl)));
        g = ShiftRight<6>(Combine(d16, DemoteTo(dh16, gh), DemoteTo(dh16, gl)));
        b = ShiftRight<6>(Combine(d16, DemoteTo(dh16, bh), DemoteTo(dh16, bl)));
      }

      StoreRGBA<PixelType>(d16, store, r, g, b, vAlpha);

      store += lanes * components;
      ySrc += lanes;

      CbSource += uvLanes;
      CrSource += uvLanes;
    }

    for (; x < width; x += lanesForward) {
      const uint16_t uValue = reinterpret_cast<const uint16_t *>(CbSource)[0];
      const uint16_t vValue = reinterpret_cast<const uint16_t *>(CrSource)[0];

      int Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
      const int Cr = (static_cast<int>(vValue) - biasUV);
      const int Cb = (static_cast<int>(uValue) - biasUV);

      int R = (Y + CrCoeff * Cr) >> precision;
      int B = (Y + CbCoeff * Cb) >> precision;
      int G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

      SaturatedStoreRGBA<uint16_t, int, PixelType>(store, R, G, B, maxColors, maxColors);

      store += components;
      ySrc += 1;

      if (x + 1 < width) {
        Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
        R = (Y + CrCoeff * Cr) >> precision;
        B = (Y + CbCoeff * Cb) >> precision;
        G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

        SaturatedStoreRGBA<uint16_t, int, PixelType>(store, R, G, B, maxColors, maxColors);
        store += components;
        ySrc += 1;
      }

      if (x + 2 < width) {
        Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
        R = (Y + CrCoeff * Cr) >> precision;
        B = (Y + CbCoeff * Cb) >> precision;
        G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

        SaturatedStoreRGBA<uint16_t, int, PixelType>(store, R, G, B, maxColors, maxColors);
        store += components;
        ySrc += 1;
      }

      if (x + 3 < width) {
        Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
        R = (Y + CrCoeff * Cr) >> precision;
        B = (Y + CbCoeff * Cb) >> precision;
        G = (Y - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

        SaturatedStoreRGBA<uint16_t, int, PixelType>(store, R, G, B, maxColors, maxColors);
        store += components;
        ySrc += 1;
      }

      CbSource += 1;
      CrSource += 1;
    }

    if (chromaSubsample == YUV_SAMPLE_411 || (chromaSubsample == YUV_SAMPLE_410 && y % 4 == 0)) {
      mUSrc += uStride;
      mVSrc += vStride;
    }
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define YCbCr411PXToXXXX_DECLARATION_R(pixelType, bit, yuvname, chroma) \
    void yuvname##P##bit##To##pixelType##HWY(uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                             const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                             const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange) {\
      YCbCr411P16ToXRGB<sparkyuv::PIXEL_##pixelType, bit, chroma>(src, srcStride, width, height,\
                                                 yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                 kr, kb, colorRange);\
    }

YCbCr411PXToXXXX_DECLARATION_R(RGBA, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(RGB, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_R(ARGB, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(ABGR, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(BGRA, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(BGR, 10, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#endif

YCbCr411PXToXXXX_DECLARATION_R(RGBA, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(RGB, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#if SPARKYUV_FULL_CHANNELS
YCbCr411PXToXXXX_DECLARATION_R(ARGB, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(ABGR, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(BGRA, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
YCbCr411PXToXXXX_DECLARATION_R(BGR, 12, YCbCr411, sparkyuv::YUV_SAMPLE_411)
#endif

#undef YCbCr411PXToXXXX_DECLARATION_R

#define YCbCr410PXToXXXX_DECLARATION_R(pixelType, bit, yuvname, chroma) \
    void yuvname##P##bit##To##pixelType##HWY(uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                             const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                             const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange) {\
      YCbCr411P16ToXRGB<sparkyuv::PIXEL_##pixelType, bit, chroma>(src, srcStride, width, height,\
                                                 yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                 kr, kb, colorRange);\
    }

YCbCr410PXToXXXX_DECLARATION_R(RGBA, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(RGB, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410PXToXXXX_DECLARATION_R(ARGB, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(ABGR, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(BGRA, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(BGR, 10, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#endif

YCbCr410PXToXXXX_DECLARATION_R(RGBA, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(RGB, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#if SPARKYUV_FULL_CHANNELS
YCbCr410PXToXXXX_DECLARATION_R(ARGB, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(ABGR, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(BGRA, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
YCbCr410PXToXXXX_DECLARATION_R(BGR, 12, YCbCr410, sparkyuv::YUV_SAMPLE_410)
#endif

#undef YCbCr411PXToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif