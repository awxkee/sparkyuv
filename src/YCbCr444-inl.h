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

#if defined(SPARKYUV__YUV444_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__YUV444_INL_H
#undef SPARKYUV__YUV444_INL_H
#else
#define SPARKYUV__YUV444_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void Pixel8ToYUV444HWY(const uint8_t *SPARKYUV_RESTRICT src,
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

  const auto vBiasY = Set(d32, iBiasY);
  const auto vBiasUV = Set(d32, iBiasUV);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

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

#if SPARKYUV_ALLOW_WIDE_MULL_ACCUMULATE
      V32 YRh = vBiasY;
      V32 YRl = ReorderWidenMulAccumulate(d32, R16, vYR, vBiasY, YRh);
      YRl = ReorderWidenMulAccumulate(d32, G16, vYG, YRl, YRh);
      YRl = ReorderWidenMulAccumulate(d32, B16, vYB, YRl, YRh);

      const auto Y = BitCast(du16, Combine(di16, DemoteTo(dhi16, ShiftRight<8>(YRh)), DemoteTo(dhi16, ShiftRight<8>(YRl))));

      V32 Cbh = vBiasUV;
      V32 Cbl = ReorderWidenMulAccumulate(d32, R16, vCbR, vBiasUV, Cbh);
      Cbl = ReorderWidenMulAccumulate(d32, G16, vCbG, Cbl, Cbh);
      Cbl = ReorderWidenMulAccumulate(d32, B16, vCbB, Cbl, Cbh);

      const auto Cb = BitCast(du16, Combine(di16, DemoteTo(dhi16, ShiftRight<8>(Cbh)), DemoteTo(dhi16, ShiftRight<8>(Cbl))));

      V32 Crh = vBiasUV;
      V32 Crl = ReorderWidenMulAccumulate(d32, R16, vCrR, vBiasUV, Crh);
      Crl = ReorderWidenMulAccumulate(d32, G16, vCrG, Crl, Crh);
      Crl = ReorderWidenMulAccumulate(d32, B16, vCrB, Crl, Crh);

      const auto Cr = BitCast(du16, Combine(di16, DemoteTo(dhi16, ShiftRight<8>(Crh)), DemoteTo(dhi16, ShiftRight<8>(Crl))));
#else
      V32 Rl = PromoteLowerTo(d32, R16);
      V32 Rh = PromoteUpperTo(d32, R16);
      V32 Bl = PromoteLowerTo(d32, B16);
      V32 Bh = PromoteUpperTo(d32, B16);
      V32 Gl = PromoteLowerTo(d32, G16);
      V32 Gh = PromoteUpperTo(d32, G16);

      const auto Yl = ShiftRight<8>(MulAdd(Rl, vYR,
                                           MulAdd(Gl, vYG,
                                                  MulAdd(Bl, vYB, vBiasY))));
      const auto Yh = ShiftRight<8>(MulAdd(Rh, vYR,
                                           MulAdd(Gh, vYG,
                                                  MulAdd(Bh, vYB, vBiasY))));

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

      const auto Y = BitCast(du16, Combine(di16, DemoteTo(dhi16, Yh), DemoteTo(dhi16, Yl)));
      const auto Cb = BitCast(du16, Combine(di16, DemoteTo(dhi16, Cbh), DemoteTo(dhi16, Cbl)));
      const auto Cr = BitCast(du16, Combine(di16, DemoteTo(dhi16, Crh), DemoteTo(dhi16, Crl)));
#endif

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

#define XXXXToYUV444HWY_DECLARATION_R(pixelType) \
        void pixelType##ToYUV444HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                    const uint32_t width, const uint32_t height,\
                                    uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                    uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                    uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                    const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel8ToYUV444HWY<sparkyuv::PIXEL_##pixelType>(src, srcStride, width, height,\
                                                         yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
        }

XXXXToYUV444HWY_DECLARATION_R(RGBA)
XXXXToYUV444HWY_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
XXXXToYUV444HWY_DECLARATION_R(ARGB)
XXXXToYUV444HWY_DECLARATION_R(ABGR)
XXXXToYUV444HWY_DECLARATION_R(BGRA)
XXXXToYUV444HWY_DECLARATION_R(BGR)
#endif

#undef XXXXToYUV444HWY_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA>
void YUV444ToXRGB(uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,
                  const uint32_t width, const uint32_t height,
                  const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                  const uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                  const uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                  const float kr, const float kb, const SparkYuvColorRange colorRange) {
  const ScalableTag<int16_t> d16;
  const Rebind<uint8_t, decltype(d16)> du8x8;
  using VU16 = Vec<decltype(d16)>;
  const Half<decltype(d16)> dh16;

  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);
  auto mUSrc = reinterpret_cast<const uint8_t *>(uPlane);
  auto mVSrc = reinterpret_cast<const uint8_t *>(vPlane);

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, 8, biasY, biasUV, rangeY, rangeUV);

  const VU16 uvCorrection = Set(d16, biasUV);

  const auto uvCorrIY = Set(d16, biasY);
  const auto A = Set(d16, 255);

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

  const VU16 ivLumaCoeff = Set(d16, iLumaCoeff);
  const VU16 ivCrCoeff = Set(d16, CrCoeff);
  const VU16 ivCbCoeff = Set(d16, CbCoeff);
  const VU16 ivGCoeff1 = Set(d16, GCoeff1);
  const VU16 ivGCoeff2 = Set(d16, GCoeff2);

  const int lanes = Lanes(du8x8);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const auto vZero = Zero(d16);

  for (int y = 0; y < height; ++y) {
    auto uSource = reinterpret_cast<const uint8_t *>(mUSrc);
    auto vSource = reinterpret_cast<const uint8_t *>(mVSrc);
    auto ySrc = reinterpret_cast<const uint8_t *>(mYSrc);
    auto store = reinterpret_cast<uint8_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const VU16 Y = Sub(PromoteTo(d16, LoadU(du8x8, ySrc)), uvCorrIY);

      const VU16 cb = Sub(PromoteTo(d16, LoadU(du8x8, uSource)), uvCorrection);
      const VU16 cr = Sub(PromoteTo(d16, LoadU(du8x8, vSource)), uvCorrection);

      const auto light = Mul(Y, ivLumaCoeff);
      const VU16 r = Max(ShiftRight<6>(SaturatedAdd(Mul(ivCrCoeff, cr), light)), vZero);
      const VU16 b = Max(ShiftRight<6>(SaturatedAdd(Mul(ivCbCoeff, cb), light)), vZero);
      const VU16 g = Max(ShiftRight<6>(SaturatedSub(light, SaturatedAdd(Mul(ivGCoeff1, cr), Mul(ivGCoeff2, cb)))), vZero);

      StoreRGBA<PixelType>(d16, store, r, g, b, A);

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

#define YUV444ToXXXX_DECLARATION_R(pixelType) \
    void YUV444To##pixelType##HWY(uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                         const uint32_t width, const uint32_t height,\
                         const uint8_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                         const uint8_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                         const uint8_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                         const float kr, const float kb, const SparkYuvColorRange colorRange) {\
                         YUV444ToXRGB<sparkyuv::PIXEL_##pixelType>(src, srcStride, width, height,\
                                                                     yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
                         }

YUV444ToXXXX_DECLARATION_R(RGBA)
YUV444ToXXXX_DECLARATION_R(RGB)
#if SPARKYUV_FULL_CHANNELS
YUV444ToXXXX_DECLARATION_R(ARGB)
YUV444ToXXXX_DECLARATION_R(ABGR)
YUV444ToXXXX_DECLARATION_R(BGRA)
YUV444ToXXXX_DECLARATION_R(BGR)
#endif

#undef YUV444ToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif