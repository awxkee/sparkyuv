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

#if defined(SPARKYUV__NV16_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__NV16_INL_H
#undef SPARKYUV__NV16_INL_H
#else
#define SPARKYUV__NV16_INL_H
#endif

#include "hwy/highway.h"
#include "src/yuv-inl.h"
#include "src/sparkyuv-internal.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, SparkYuvNVLoadOrder LoadOrder = sparkyuv::YUV_ORDER_UV>
void NV16ToPixel8(uint8_t *SPARKYUV_RESTRICT dst,
                  const uint32_t dstStride,
                  const uint32_t width,
                  const uint32_t height,
                  const uint8_t *SPARKYUV_RESTRICT yPlane,
                  const uint32_t yStride,
                  const uint8_t *SPARKYUV_RESTRICT uvPlane,
                  const uint32_t uvStride,
                  const float kr, const float kb,
                  const SparkYuvColorRange colorRange) {
  const ScalableTag<int16_t> du16x8;
  const RebindToSigned<decltype(du16x8)> di16;
  const Rebind<uint8_t, decltype(du16x8)> du8x8;
  const Half<decltype(du8x8)> du8h;
  using VU16 = Vec<decltype(du16x8)>;
  const Half<decltype(du16x8)> dh16;

  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);
  auto mUVSrc = reinterpret_cast<const uint8_t *>(uvPlane);

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  const VU16 uvCorrection = Set(du16x8, biasUV);
  const auto A = Set(du16x8, 255);

  float fCrCoeff = 0.f;
  float fCbCoeff = 0.f;
  float fGCoeff1 = 0.f;
  float fGCoeff2 = 0.f;
  const float flumaCoeff = 255.f / static_cast<float>(rangeY);
  ComputeInverseTransform(kr, kb, 255.f, static_cast<float>(rangeUV), fCrCoeff, fCbCoeff, fGCoeff1, fGCoeff2);

  int precision = 6;

  const int CrCoeff = static_cast<int>(std::roundf(fCrCoeff * static_cast<float>( 1 << precision )));
  const int CbCoeff = static_cast<int>(std::roundf(fCbCoeff * static_cast<float>( 1 << precision )));
  const int GCoeff1 = static_cast<int>(std::roundf(fGCoeff1 * static_cast<float>( 1 << precision )));
  const int GCoeff2 = static_cast<int>(std::roundf(fGCoeff2 * static_cast<float>( 1 << precision )));

  const int iLumaCoeff = static_cast<int>(std::roundf(flumaCoeff * static_cast<float>( 1 << precision )));

  const VU16 ivLumaCoeff = Set(du16x8, iLumaCoeff);
  const VU16 ivCrCoeff = Set(du16x8, CrCoeff);
  const VU16 ivCbCoeff = Set(du16x8, CbCoeff);
  const VU16 ivGCoeff1 = Set(du16x8, GCoeff1);
  const VU16 ivGCoeff2 = Set(du16x8, GCoeff2);

  const VU16 vZero = Zero(du16x8);

  const auto uvCorrIY = Set(du16x8, biasY);

  const int lanes = Lanes(du8x8);
  const int uvLanes = Lanes(du8h);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  for (int y = 0; y < height; ++y) {
    auto uvSource = reinterpret_cast<const uint8_t *>(mUVSrc);
    auto ySrc = reinterpret_cast<const uint8_t *>(mYSrc);
    auto store = reinterpret_cast<uint8_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const auto luma = Sub(BitCast(di16, PromoteTo(du16x8, LoadU(du8x8, ySrc))), uvCorrIY);

      using VU8x4 = Vec<decltype(du8h)>;
      VU8x4 ulfh, vlfh;
      if (LoadOrder == YUV_ORDER_UV) {
        LoadInterleaved2(du8h, uvSource, ulfh, vlfh);
      } else {
        LoadInterleaved2(du8h, uvSource, vlfh, ulfh);
      }
      auto ulf = Combine(du8x8, InterleaveUpper(du8h, ulfh, ulfh), InterleaveLower(ulfh, ulfh));
      auto vlf = Combine(du8x8, InterleaveUpper(du8h, vlfh, vlfh), InterleaveLower(vlfh, vlfh));

      const VU16 cb = Sub(PromoteTo(du16x8, ulf), uvCorrection);
      const VU16 cr = Sub(PromoteTo(du16x8, vlf), uvCorrection);

      const auto light = Mul(luma, ivLumaCoeff);
      const VU16 r = ShiftRight<6>(Max(SaturatedAdd(Mul(ivCrCoeff, cr), light), vZero));
      const VU16 b = ShiftRight<6>(Max(SaturatedAdd(Mul(ivCbCoeff, cb), light), vZero));
      const VU16
          g = ShiftRight<6>(Max(SaturatedSub(light, SaturatedAdd(Mul(ivGCoeff1, cr), Mul(ivGCoeff2, cb))), vZero));

      StoreRGBA<PixelType>(du16x8, reinterpret_cast<uint8_t *>(store), r, g, b, A);

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
      store += 4;
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

#if SPARKYUV_ALLOW_WIDE_MULL_ACCUMULATE
  const auto vCbR = Set(coeffTag, -CbR);
#else
  const auto vCbR = Set(coeffTag, CbR);
#endif
  const auto vCbG = Set(coeffTag, -CbG);
  const auto vCbB = Set(coeffTag, CbB);

  const auto vCrR = Set(coeffTag, CrR);
  const auto vCrG = Set(coeffTag, -CrG);
#if SPARKYUV_ALLOW_WIDE_MULL_ACCUMULATE
  const auto vCrB = Set(coeffTag, -CrB);
#else
  const auto vCrB = Set(coeffTag, CrB);
#endif

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
      V32 YRl = ReorderWidenMulAccumulate(d32, R, vYR, vBiasY, YRh);
      YRl = ReorderWidenMulAccumulate(d32, G, vYG, YRl, YRh);
      YRl = ReorderWidenMulAccumulate(d32, B, vYB, YRl, YRh);

      const auto
          Y = BitCast(du16, Combine(di16, DemoteTo(dhi16, ShiftRight<8>(YRh)), DemoteTo(dhi16, ShiftRight<8>(YRl))));
      V32 Cbh = vBiasUV;
      V32 Cbl = ReorderWidenMulAccumulate(d32, R, vCbR, vBiasUV, Cbh);
      Cbl = ReorderWidenMulAccumulate(d32, G, vCbG, Cbl, Cbh);
      Cbl = ReorderWidenMulAccumulate(d32, B, vCbB, Cbl, Cbh);

      const auto
          Cbf = BitCast(du16, Combine(di16, DemoteTo(dhi16, ShiftRight<8>(Cbh)), DemoteTo(dhi16, ShiftRight<8>(Cbl))));

      V32 Crh = vBiasUV;
      V32 Crl = ReorderWidenMulAccumulate(d32, R, vCrR, vBiasUV, Crh);
      Crl = ReorderWidenMulAccumulate(d32, G, vCrG, Crl, Crh);
      Crl = ReorderWidenMulAccumulate(d32, B, vCrB, Crl, Crh);

      const auto
          Crf = BitCast(du16, Combine(di16, DemoteTo(dhi16, ShiftRight<8>(Crh)), DemoteTo(dhi16, ShiftRight<8>(Crl))));

      const auto Cb = DemoteTo(du8h, ShiftRight<1>(SumsOf2(Cbf)));
      const auto Cr = DemoteTo(du8h, ShiftRight<1>(SumsOf2(Crf)));

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
      const auto Yl = ShiftRight<8>(MulAdd(Rl, vYR,
                                           MulAdd(Gl, vYG,
                                                  MulAdd(Bl, vYB, vBiasY))));
      const auto Yh = ShiftRight<8>(MulAdd(Rh, vYR,
                                           MulAdd(Gh, vYG,
                                                  MulAdd(Bh, vYB, vBiasY))));

      const auto Y = BitCast(du16, Combine(di16, DemoteTo(dhi16, Yh), DemoteTo(dhi16, Yl)));

     const auto Cbl = ShiftRight<8>(Add(MulAdd(Bl, vCbB,
                                               MulSub(Gl, vCbG,
                                                      Mul(Rl, vCbR))), vBiasUV));
     const auto Cbh = ShiftRight<8>(Add(MulAdd(Bh, vCbB,
                                               MulSub(Gh, vCbG,
                                                      Mul(Rh, vCbR))), vBiasUV));

     const auto Crh = ShiftRight<8>(Add(MulAdd(Rh, vCrR,
                                               MulSub(Gh, vCrG,
                                                      Mul(Bh, vCrB))), vBiasUV));

     const auto Crl = ShiftRight<8>(Add(MulAdd(Rl, vCrR,
                                               MulSub(Gl, vCrG,
                                                      Mul(Bl, vCrB))), vBiasUV));

     const auto Cbf = BitCast(du16, Combine(di16, DemoteTo(dhi16, Cbh), DemoteTo(dhi16, Cbl)));
     const auto Crf = BitCast(du16, Combine(di16, DemoteTo(dhi16, Crh), DemoteTo(dhi16, Crl)));

     const auto Cb = DemoteTo(du8h, ShiftRight<1>(SumsOf2(Cbf)));
     const auto Cr = DemoteTo(du8h, ShiftRight<1>(SumsOf2(Crf)));

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