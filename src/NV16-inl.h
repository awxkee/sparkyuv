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

#if defined(SPARKYUV_NV16_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_NV16_INL_H
#undef SPARKYUV_NV16_INL_H
#else
#define SPARKYUV_NV16_INL_H
#endif

#include "hwy/highway.h"
#include "src/yuv-inl.h"
#include "src/sparkyuv-internal.h"
#include "cassert"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, SparkYuvNVLoadOrder LoadOrder = sparkyuv::YUV_ORDER_UV>
void NV16ToPixel8(uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                  const uint32_t width, const uint32_t height,
                  const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                  const uint8_t *SPARKYUV_RESTRICT uvPlane, const uint32_t uvStride,
                  const float kr, const float kb,
                  const SparkYuvColorRange colorRange) {
  const ScalableTag<uint8_t> du8;
  const Half<decltype(du8)> du8h;
  const Rebind<int16_t, decltype(du8h)> di16;
  const RebindToUnsigned<decltype(di16)> du16;
  using VI16 = Vec<decltype(di16)>;
  const Half<decltype(di16)> dh16;

  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);
  auto mUVSrc = reinterpret_cast<const uint8_t *>(uvPlane);

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  const VI16 uvCorrection = Set(di16, biasUV);
  const auto a = Set(du8, 255);

  float fCrCoeff = 0.f;
  float fCbCoeff = 0.f;
  float fGCoeff1 = 0.f;
  float fGCoeff2 = 0.f;
  const float flumaCoeff = 255.f / static_cast<float>(rangeY);
  ComputeInverseTransform(kr, kb, 255.f, static_cast<float>(rangeUV), fCrCoeff, fCbCoeff, fGCoeff1, fGCoeff2);

  const int precision = 6;

  const int CrCoeff = static_cast<int>(std::roundf(fCrCoeff * static_cast<float>( 1 << precision )));
  const int CbCoeff = static_cast<int>(std::roundf(fCbCoeff * static_cast<float>( 1 << precision )));
  const int GCoeff1 = static_cast<int>(std::roundf(fGCoeff1 * static_cast<float>( 1 << precision )));
  const int GCoeff2 = static_cast<int>(std::roundf(fGCoeff2 * static_cast<float>( 1 << precision )));

  const int iLumaCoeff = static_cast<int>(std::roundf(flumaCoeff * static_cast<float>( 1 << precision )));

  const auto ivLumaCoeff = Set(du8, iLumaCoeff);
  const auto ivLumaCoeffh = Set(du8h, iLumaCoeff);
  const auto ivCrCoeff = Set(di16, CrCoeff);
  const auto ivCbCoeff = Set(di16, CbCoeff);
  const auto ivGCoeff1 = Set(di16, GCoeff1);
  const auto ivGCoeff2 = Set(di16, GCoeff2);

  const VI16 vZero = Zero(di16);

  const auto uvCorrIY = Set(du8, biasY);

  const int lanes = Lanes(du8);
  const int uvLanes = Lanes(du8h);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  for (int y = 0; y < height; ++y) {
    auto uvSource = reinterpret_cast<const uint8_t *>(mUVSrc);
    auto ySrc = reinterpret_cast<const uint8_t *>(mYSrc);
    auto store = reinterpret_cast<uint8_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      using VU8x4 = Vec<decltype(du8h)>;
      VU8x4 ulfh, vlfh;
      if (LoadOrder == YUV_ORDER_UV) {
        LoadInterleaved2(du8h, uvSource, ulfh, vlfh);
      } else {
        LoadInterleaved2(du8h, uvSource, vlfh, ulfh);
      }

      const auto luma8 = Sub(LoadU(du8, ySrc), uvCorrIY);

      auto ulf = Combine(du8, InterleaveUpper(du8h, ulfh, ulfh), InterleaveLower(ulfh, ulfh));
      auto vlf = Combine(du8, InterleaveUpper(du8h, vlfh, vlfh), InterleaveLower(vlfh, vlfh));

      const auto cbh = Sub(BitCast(di16, PromoteUpperTo(du16, ulf)), uvCorrection);
      const auto crh = Sub(BitCast(di16, PromoteUpperTo(du16, vlf)), uvCorrection);

      const auto cbl = Sub(BitCast(di16, PromoteLowerTo(du16, ulf)), uvCorrection);
      const auto crl = Sub(BitCast(di16, PromoteLowerTo(du16, vlf)), uvCorrection);

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

      StoreRGBA<PixelType>(du8, reinterpret_cast<uint8_t *>(store), r, g, b, a);

      store += lanes * components;
      ySrc += lanes;

      uvSource += uvLanes * 2;
    }

    for (; x < width; x += 2) {
      const uint8_t uValue = reinterpret_cast<const uint8_t *>(uvSource)[LoadOrder == YUV_ORDER_UV ? 0 : 1];
      const uint8_t vValue = reinterpret_cast<const uint8_t *>(uvSource)[LoadOrder == YUV_ORDER_UV ? 1 : 0];

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

      uvSource += 2;
    }

    mUVSrc += uvStride;
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define NVXXToXXXXHWY_DECLARATION_R(pixelType, NVType, NVOrder) \
        void NVType##To##pixelType##HWY(uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                           const uint32_t width, const uint32_t height,\
                           const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                           const uint8_t *SPARKYUV_RESTRICT uvPlane, const uint32_t uvStride, \
                           const float kr, const float kb, const SparkYuvColorRange colorRange) { \
        NV16ToPixel8<sparkyuv::PIXEL_##pixelType, NVOrder>(dst, dstStride, \
                                  width, height, yPlane, yStride, uvPlane, uvStride, kr, kb, colorRange); \
        }

NVXXToXXXXHWY_DECLARATION_R(RGBA, NV16, YUV_ORDER_UV)
NVXXToXXXXHWY_DECLARATION_R(RGB, NV16, YUV_ORDER_UV)
#if SPARKYUV_FULL_CHANNELS
NVXXToXXXXHWY_DECLARATION_R(ARGB, NV16, YUV_ORDER_UV)
NVXXToXXXXHWY_DECLARATION_R(ABGR, NV16, YUV_ORDER_UV)
NVXXToXXXXHWY_DECLARATION_R(BGRA, NV16, YUV_ORDER_UV)
NVXXToXXXXHWY_DECLARATION_R(BGR, NV16, YUV_ORDER_UV)
#endif

NVXXToXXXXHWY_DECLARATION_R(RGBA, NV61, YUV_ORDER_VU)
NVXXToXXXXHWY_DECLARATION_R(RGB, NV61, YUV_ORDER_VU)
#if SPARKYUV_FULL_CHANNELS
NVXXToXXXXHWY_DECLARATION_R(ARGB, NV61, YUV_ORDER_VU)
NVXXToXXXXHWY_DECLARATION_R(ABGR, NV61, YUV_ORDER_VU)
NVXXToXXXXHWY_DECLARATION_R(BGRA, NV61, YUV_ORDER_VU)
NVXXToXXXXHWY_DECLARATION_R(BGR, NV61, YUV_ORDER_VU)
#endif

#undef NVXXToXXXXHWY_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvNVLoadOrder LoadOrder = sparkyuv::YUV_ORDER_UV>
void Pixel8ToNV16HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                     const uint32_t width, const uint32_t height,
                     uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                     uint8_t *SPARKYUV_RESTRICT uvPlane, const uint32_t uvStride,
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
  auto uvSource = reinterpret_cast<uint8_t *>(uvPlane);

  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const ScalableTag<uint16_t> du16;
  const ScalableTag<int16_t> di16;
  const Rebind<uint8_t, decltype(di16)> du8;
  const Half<decltype(di16)> dhi16;
  const Rebind<int32_t, decltype(dhi16)> d32;
  const RebindToUnsigned<decltype(d32)> du32;
  using VU8 = Vec<decltype(du8)>;
  using V32 = Vec<decltype(d32)>;

  const Half<decltype(du8)> du8h;

  const int lanes = Lanes(du8);

  const auto vBiasY = Set(d32, iBiasY);
  const auto vBiasUV = Set(d32, iBiasUV);

#if SPARKYUV_ALLOW_WIDE_MULL_ACCUMULATE
  const auto coeffTag = di16;
#else
  const auto coeffTag = d32;
#endif

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

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<uint8_t *>(yStore);
    auto uvDestination = reinterpret_cast<uint8_t *>(uvSource);

    auto mSrc = reinterpret_cast<const uint8_t *>(mSource);

    for (; x + lanes < width; x += lanes) {
      VU8 R8;
      VU8 G8;
      VU8 B8;
      VU8 A8;
      LoadRGBA<PixelType>(du8, mSrc, R8, G8, B8, A8);

      auto R = BitCast(di16, PromoteTo(du16, R8));
      auto G = BitCast(di16, PromoteTo(du16, G8));
      auto B = BitCast(di16, PromoteTo(du16, B8));

#if SPARKYUV_ALLOW_WIDE_MULL_ACCUMULATE
      V32 YRh = vBiasY;
      V32 YRl = WidenMulAccumulate(di16, R, vYR, vBiasY, YRh);
      YRl = WidenMulAccumulate(di16, G, vYG, YRl, YRh);
      YRl = WidenMulAccumulate(di16, B, vYB, YRl, YRh);

      const auto
          Y = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, YRh), ShiftRightNarrow<8>(d32, YRl)));
      V32 Cbh = vBiasUV;
      V32 Cbl = ReorderWidenMulAccumulate(d32, R, vCbR, vBiasUV, Cbh);
      Cbl = WidenMulAccumulate(di16, G, vCbG, Cbl, Cbh);
      Cbl = WidenMulAccumulate(di16, B, vCbB, Cbl, Cbh);

      const auto
          Cbf = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Cbh), ShiftRightNarrow<8>(d32, Cbl)));

      V32 Crh = vBiasUV;
      V32 Crl = WidenMulAccumulate(di16, R, vCrR, vBiasUV, Crh);
      Crl = WidenMulAccumulate(di16, G, vCrG, Crl, Crh);
      Crl = WidenMulAccumulate(di16, B, vCrB, Crl, Crh);

      const auto
          Crf = BitCast(du16, Combine(di16, ShiftRightNarrow<8>(d32, Crh), ShiftRightNarrow<8>(d32, Crl)));

      const auto Cb = DemoteTo(du8h, ShiftRightNarrow<1>(du32, SumsOf2(Cbf)));
      const auto Cr = DemoteTo(du8h, ShiftRightNarrow<1>(du32, SumsOf2(Crf)));

      if (LoadOrder == YUV_ORDER_UV) {
        StoreInterleaved2(Cb, Cr, du8h, uvDestination);
      } else {
        StoreInterleaved2(Cr, Cb, du8h, uvDestination);
      }
#else
      V32 Rl = PromoteLowerTo(d32, R);
      V32 Rh = PromoteUpperTo(d32, R);
      V32 Bl = PromoteLowerTo(d32, B);
      V32 Bh = PromoteUpperTo(d32, B);
      V32 Gl = PromoteLowerTo(d32, G);
      V32 Gh = PromoteUpperTo(d32, G);
      const auto Yl = ShiftRightNarrow<8>(d32, MulAdd(Rl, vYR,
                                                      MulAdd(Gl, vYG,
                                                             MulAdd(Bl, vYB, vBiasY))));
      const auto Yh = ShiftRightNarrow<8>(d32, MulAdd(Rh, vYR,
                                                      MulAdd(Gh, vYG,
                                                             MulAdd(Bh, vYB, vBiasY))));

      const auto Y = BitCast(du16, Combine(di16, Yh, Yl));

      const auto Cbl = ShiftRightNarrow<8>(d32, MulAdd(Bl, vCbB,
                                                       MulAdd(Gl, vCbG,
                                                              MulAdd(Rl, vCbR, vBiasUV))));
      const auto Cbh = ShiftRightNarrow<8>(d32, MulAdd(Bh, vCbB,
                                                       MulAdd(Gh, vCbG,
                                                              MulAdd(Rh, vCbR, vBiasUV))));

      const auto Crh = ShiftRightNarrow<8>(d32, MulAdd(Rh, vCrR,
                                                       MulAdd(Gh, vCrG,
                                                              MulAdd(Bh, vCrB, vBiasUV))));

      const auto Crl = ShiftRightNarrow<8>(d32, MulAdd(Rl, vCrR,
                                                       MulAdd(Gl, vCrG,
                                                              MulAdd(Bl, vCrB, vBiasUV))));

      const auto Cbf = BitCast(du16, Combine(di16, Cbh, Cbl));
      const auto Crf = BitCast(du16, Combine(di16, Crh, Crl));

      const auto Cb = DemoteTo(du8h, ShiftRightNarrow<1>(du32, SumsOf2(Cbf)));
      const auto Cr = DemoteTo(du8h, ShiftRightNarrow<1>(du32, SumsOf2(Crf)));

      if (LoadOrder == YUV_ORDER_UV) {
        StoreInterleaved2(Cb, Cr, du8h, uvDestination);
      } else {
        StoreInterleaved2(Cr, Cb, du8h, uvDestination);
      }
#endif

      StoreU(DemoteTo(du8, Y), du8, yDst);

      yDst += lanes;
      uvDestination += lanes;

      mSrc += components * lanes;
    }

    for (; x < width; x += 2) {
      int r;
      int g;
      int b;

      LoadRGB<uint8_t, int, PixelType>(mSrc, r, g, b);

      int Y0 = ((r * YR + g * YG + b * YB + iBiasY) >> precision);

      yDst[0] = Y0;
      yDst += 1;

      int r2 = r;
      int g2 = g;
      int b2 = b;

      if (x + 1 < width) {
        mSrc += components;

        LoadRGB<uint8_t, int, PixelType>(mSrc, r2, g2, b2);

        int Y1 = ((r2 * YR + g2 * YG + b2 * YB + iBiasY) >> precision);

        yDst[0] = Y1;
        yDst += 1;

        mSrc += components;
      } else {
        mSrc += components;
      }

      r = (r + r2) >> 1;
      g = (g + g2) >> 1;
      b = (b + b2) >> 1;
      int Cb = ((-r * CbR - g * CbG + b * CbB + iBiasUV) >> precision);
      int Cr = ((r * CrR - g * CrG - b * CrB + iBiasUV) >> precision);
      if (LoadOrder == YUV_ORDER_UV) {
        uvDestination[0] = Cb;
        uvDestination[1] = Cr;
      } else {
        uvDestination[1] = Cb;
        uvDestination[0] = Cr;
      }
      uvDestination += 2;
    }

    yStore += yStride;
    uvSource += uvStride;
    mSource += srcStride;
  }
}

#define XXXXTONVXXHWY_DECLARATION_R(pixelType, NVType, NVOrder) \
        void pixelType##To##NVType##HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t dstStride,\
                           const uint32_t width, const uint32_t height,\
                           uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                           uint8_t *SPARKYUV_RESTRICT uvPlane, const uint32_t uvStride, \
                           const float kr, const float kb, const SparkYuvColorRange colorRange) { \
        Pixel8ToNV16HWY<sparkyuv::PIXEL_##pixelType, NVOrder>(src, dstStride, \
                                  width, height, yPlane, yStride, uvPlane, uvStride, kr, kb, colorRange); \
        }

XXXXTONVXXHWY_DECLARATION_R(RGBA, NV16, YUV_ORDER_UV)
XXXXTONVXXHWY_DECLARATION_R(RGB, NV16, YUV_ORDER_UV)
#if SPARKYUV_FULL_CHANNELS
XXXXTONVXXHWY_DECLARATION_R(ARGB, NV16, YUV_ORDER_UV)
XXXXTONVXXHWY_DECLARATION_R(ABGR, NV16, YUV_ORDER_UV)
XXXXTONVXXHWY_DECLARATION_R(BGRA, NV16, YUV_ORDER_UV)
XXXXTONVXXHWY_DECLARATION_R(BGR, NV16, YUV_ORDER_UV)
#endif

XXXXTONVXXHWY_DECLARATION_R(RGBA, NV61, YUV_ORDER_VU)
XXXXTONVXXHWY_DECLARATION_R(RGB, NV61, YUV_ORDER_VU)
#if SPARKYUV_FULL_CHANNELS
XXXXTONVXXHWY_DECLARATION_R(ARGB, NV61, YUV_ORDER_VU)
XXXXTONVXXHWY_DECLARATION_R(ABGR, NV61, YUV_ORDER_VU)
XXXXTONVXXHWY_DECLARATION_R(BGRA, NV61, YUV_ORDER_VU)
XXXXTONVXXHWY_DECLARATION_R(BGR, NV61, YUV_ORDER_VU)
#endif

#undef XXXXTONVXXHWY_DECLARATION_R
}
HWY_AFTER_NAMESPACE();

#endif