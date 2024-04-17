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

#if defined(SPARKYUV_YCbCr444P16_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YCbCr444P16_INL_H
#undef SPARKYUV_YCbCr444P16_INL_H
#else
#define SPARKYUV_YCbCr444P16_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include <algorithm>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, SparkYuvChromaSubsample chromaSubsample, int bitDepth>
void Pixel16ToYCbCr444HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                          const uint32_t width, const uint32_t height,
                          uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                          uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                          uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                          const float kr, const float kb, const SparkYuvColorRange colorRange) {
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
  const Half<decltype(du16)> dhu16;
  using V16 = Vec<decltype(d16)>;
  using V32 = Vec<decltype(d32)>;
  using VU16 = Vec<decltype(du16)>;

  const auto scale = static_cast<float>( 1 << precision );
  const auto iBiasY = static_cast<int>((static_cast<float>(biasY) + 0.5f) * scale);
  const auto iBiasUV = static_cast<int>((static_cast<float>(biasUV) + 0.5f) * scale);

  const int lanes = Lanes(d16);
  const int uvLanes = chromaSubsample == YUV_SAMPLE_444 ? Lanes(d16) : Lanes(dh16);

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
  const int cutOffUV = rangeUV + biasUV;

  const int components = getPixelTypeComponents(PixelType);

  const int lanesForward = getYuvChromaPixels(chromaSubsample);

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

      if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          auto nextRow = reinterpret_cast<const uint16_t *>(mSrc);
          if (y + 1 < height - 1) {
            nextRow = reinterpret_cast<const uint16_t *>(reinterpret_cast<const uint8_t *>(mSrc) + srcStride);
          }
          VU16 R1;
          VU16 G1;
          VU16 B1;
          VU16 A1;
          LoadRGBA<PixelType>(du16, reinterpret_cast<const uint16_t *>(nextRow), R1, G1, B1, A1);

          R = BitCast(d16, AddAndHalf(du16, BitCast(du16, R), R1));
          G = BitCast(d16, AddAndHalf(du16, BitCast(du16, G), G1));
          B = BitCast(d16, AddAndHalf(du16, BitCast(du16, B), B1));
        }
      }

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

      StoreU(Y, du16, yDst);

      if (chromaSubsample == YUV_SAMPLE_444) {
        StoreU(Cb, du16, uDst);
        StoreU(Cr, du16, vDst);
      } else if (chromaSubsample == YUV_SAMPLE_422) {
        const auto cbh = ShiftRightNarrow<1>(du32, SumsOf2(Cb));
        const auto crh = ShiftRightNarrow<1>(du32, SumsOf2(Cr));
        StoreU(cbh, dhu16, uDst);
        StoreU(crh, dhu16, vDst);
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          const auto cbh = ShiftRightNarrow<1>(du32, SumsOf2(Cb));
          const auto crh = ShiftRightNarrow<1>(du32, SumsOf2(Cr));
          StoreU(cbh, dhu16, uDst);
          StoreU(crh, dhu16, vDst);
        }
      }

      yDst += lanes;
      if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          uDst += uvLanes;
          vDst += uvLanes;
        }
      } else {
        uDst += uvLanes;
        vDst += uvLanes;
      }
      mSrc += components * lanes;
    }

    for (; x < width; x += lanesForward) {
      int r;
      int g;
      int b;

      LoadRGB<uint16_t, int, PixelType>(mSrc, r, g, b);

      int Y = ((r * static_cast<int>(YR) + g * static_cast<int>(YG) + b * static_cast<int>(YB) + iBiasY) >> precision);
      yDst[0] = std::clamp(Y, static_cast<int>(biasY), cutOffY);
      yDst += 1;
      mSrc += components;

      if (chromaSubsample == YUV_SAMPLE_422) {
        if (x + 1 < width) {
          int r1 = r, g1 = g, b1 = b;

          LoadRGB<uint16_t, int, PixelType>(mSrc, r1, g1, b1);

          r = (r + r1) >> 1;
          g = (g + g1) >> 1;
          b = (b + b1) >> 1;

          int Y1 = ((r1 * static_cast<int>(YR) + g1 * static_cast<int>(YG) + b1 * static_cast<int>(YB) + iBiasY)
              >> precision);
          yDst[0] = std::clamp(Y1, static_cast<int>(biasY), cutOffY);
          yDst += 1;
          mSrc += components;
        }
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          auto nextRow = reinterpret_cast<const uint16_t *>(mSrc);
          if (y + 1 < height - 1) {
            nextRow = reinterpret_cast<const uint16_t *>(reinterpret_cast<const uint8_t *>(mSrc) + srcStride);
          }

          int r1 = r, g1 = g, b1 = b;
          int r2 = r, g2 = g, b2 = b;
          int r3 = r, g3 = g, b3 = b;

          LoadRGB<uint16_t, int, PixelType>(nextRow, r2, g2, b2);

          nextRow += components;

          if (x + 1 < width) {
            LoadRGB<uint16_t, int, PixelType>(nextRow, r3, g3, b3);
            LoadRGB<uint16_t, int, PixelType>(mSrc, r1, g1, b1);

            int Y1 = ((r1 * static_cast<int>(YR) + g1 * static_cast<int>(YG) + b1 * static_cast<int>(YB) + iBiasY)
                >> precision);
            yDst[0] = std::clamp(Y1, static_cast<int>(biasY), cutOffY);
            yDst += 1;
            mSrc += components;
          }

          r = (r + r1 + r2 + r3) >> 2;
          g = (g + g1 + g2 + g3) >> 2;
          b = (b + b1 + b2 + b3) >> 2;
        }
      }

      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
        int Cb =
            ((-r * static_cast<int>(CbR) - g * static_cast<int>(CbG) + b * static_cast<int>(CbB) + iBiasUV)
                >> precision);
        int Cr =
            ((r * static_cast<int>(CrR) - g * static_cast<int>(CrG) - b * static_cast<int>(CrB) + iBiasUV)
                >> precision);

        uDst[0] = Cb;
        vDst[0] = Cr;
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          int Cb =
              ((-r * static_cast<int>(CbR) - g * static_cast<int>(CbG) + b * static_cast<int>(CbB) + iBiasUV)
                  >> precision);
          int Cr =
              ((r * static_cast<int>(CrR) - g * static_cast<int>(CrG) - b * static_cast<int>(CrB) + iBiasUV)
                  >> precision);

          uDst[0] = Cb;
          vDst[0] = Cr;
        }
      }

      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
        uDst += 1;
        vDst += 1;
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          uDst += 1;
          vDst += 1;
        }
      }
    }

    yStore += yStride;
    if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
      uStore += uStride;
      vStore += vStride;
    } else if (chromaSubsample == YUV_SAMPLE_420) {
      if (!(y & 1)) {
        uStore += uStride;
        vStore += vStride;
      }
    }

    mSource += srcStride;
  }
}

#define XXXXToYCbCr444PHWY_DECLARATION_R(pixelType, bit, yuvname, chroma) \
        void pixelType##To##yuvname##P##bit##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                            const uint32_t width, const uint32_t height,\
                                            uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                            uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                            uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel16ToYCbCr444HWY<sparkyuv::PIXEL_##pixelType, chroma, bit>(src, srcStride, width, height,\
                                                          yPlane, yStride, uPlane, uStride, vPlane, vStride, kr, kb, colorRange);\
        }

XXXXToYCbCr444PHWY_DECLARATION_R(RGBA, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(RGB, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444PHWY_DECLARATION_R(ARGB, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(ABGR, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(BGRA, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(BGR, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#endif

XXXXToYCbCr444PHWY_DECLARATION_R(RGBA, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(RGB, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444PHWY_DECLARATION_R(ARGB, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(ABGR, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(BGRA, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
XXXXToYCbCr444PHWY_DECLARATION_R(BGR, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#endif

XXXXToYCbCr444PHWY_DECLARATION_R(RGBA, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(RGB, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444PHWY_DECLARATION_R(ARGB, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(ABGR, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(BGRA, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(BGR, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#endif

XXXXToYCbCr444PHWY_DECLARATION_R(RGBA, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(RGB, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444PHWY_DECLARATION_R(ARGB, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(ABGR, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(BGRA, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
XXXXToYCbCr444PHWY_DECLARATION_R(BGR, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#endif

XXXXToYCbCr444PHWY_DECLARATION_R(RGBA, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(RGB, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444PHWY_DECLARATION_R(ARGB, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(ABGR, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(BGRA, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(BGR, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#endif

XXXXToYCbCr444PHWY_DECLARATION_R(RGBA, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(RGB, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr444PHWY_DECLARATION_R(ARGB, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(ABGR, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(BGRA, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
XXXXToYCbCr444PHWY_DECLARATION_R(BGR, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef XXXXToYCbCr444PHWY_DECLARATION_R

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, SparkYuvChromaSubsample chromaSubsample, int bitDepth>
void YCbCr444P16ToXRGB(uint16_t *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                       const uint32_t width, const uint32_t height,
                       const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                       const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                       const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                       const float kr, const float kb, const SparkYuvColorRange colorRange) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  const ScalableTag<uint16_t> d16;
  const RebindToSigned<decltype(d16)> di16;
  const Half<decltype(d16)> dh16;
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

  const auto uvCorrection = Set(di16, biasUV);
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
  const int lanesForward = getYuvChromaPixels(chromaSubsample);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh16);

  const int components = getPixelTypeComponents(PixelType);

  for (int y = 0; y < height; ++y) {
    auto CbSource = reinterpret_cast<const uint16_t *>(mUSrc);
    auto CrSource = reinterpret_cast<const uint16_t *>(mVSrc);
    auto ySrc = reinterpret_cast<const uint16_t *>(mYSrc);
    auto store = reinterpret_cast<uint16_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const auto Y = Sub(BitCast(di16, LoadU(d16, ySrc)), uvCorrIY);

      VI16 cb;
      VI16 cr;
      if (chromaSubsample == YUV_SAMPLE_444) {
        cb = Sub(BitCast(di16, LoadU(d16, CbSource)), uvCorrection);
        cr = Sub(BitCast(di16, LoadU(d16, CrSource)), uvCorrection);
      } else if (chromaSubsample == YUV_SAMPLE_420 || chromaSubsample == YUV_SAMPLE_422) {
        auto cbh = LoadU(dh16, CbSource);
        auto crh = LoadU(dh16, CrSource);
        const auto ulc = Sub(BitCast(di16, Combine(d16, cbh, cbh)), uvCorrection);
        cb = InterleaveLower(ulc, ulc);
        const auto vlc = Sub(BitCast(di16, Combine(d16, crh, crh)), uvCorrection);
        cr = InterleaveLower(vlc, vlc);
      } else {
        static_assert("Must not be reached");
      }

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

      if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        if (x + 1 < width) {
          int Y1 = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;
          int R1 = (Y1 + CrCoeff * Cr) >> precision;
          int B1 = (Y1 + CbCoeff * Cb) >> precision;
          int G1 = (Y1 - GCoeff1 * Cr - GCoeff2 * Cb) >> precision;

          SaturatedStoreRGBA<uint16_t, int, PixelType>(store, R1, G1, B1, maxColors, maxColors);
          store += components;
          ySrc += 1;
        }
      }

      CbSource += 1;
      CrSource += 1;
    }

    if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
      mUSrc += uStride;
      mVSrc += vStride;
    } else if (chromaSubsample == YUV_SAMPLE_420) {
      if (y & 1) {
        mUSrc += uStride;
        mVSrc += vStride;
      }
    }
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define YCbCr444PXToXXXX_DECLARATION_R(pixelType, bit, yuvname, chroma) \
    void yuvname##P##bit##To##pixelType##HWY(uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                             const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,\
                                             const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange) {\
                                             YCbCr444P16ToXRGB<sparkyuv::PIXEL_##pixelType, chroma, bit>(src, srcStride, width, height,\
                                                                                        yPlane, yStride, uPlane, uStride, vPlane, vStride, \
                                                                                        kr, kb, colorRange);\
                                           }

YCbCr444PXToXXXX_DECLARATION_R(RGBA, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(RGB, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_R(ARGB, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(ABGR, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(BGRA, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(BGR, 10, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#endif
YCbCr444PXToXXXX_DECLARATION_R(RGBA, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(RGB, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_R(ARGB, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(ABGR, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(BGRA, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(BGR, 10, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#endif
YCbCr444PXToXXXX_DECLARATION_R(RGBA, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(RGB, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_R(ARGB, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(ABGR, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(BGRA, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(BGR, 10, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#endif

YCbCr444PXToXXXX_DECLARATION_R(RGBA, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(RGB, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_R(ARGB, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(ABGR, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(BGRA, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
YCbCr444PXToXXXX_DECLARATION_R(BGR, 12, YCbCr444, sparkyuv::YUV_SAMPLE_444)
#endif

YCbCr444PXToXXXX_DECLARATION_R(RGBA, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(RGB, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_R(ARGB, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(ABGR, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(BGRA, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
YCbCr444PXToXXXX_DECLARATION_R(BGR, 12, YCbCr422, sparkyuv::YUV_SAMPLE_422)
#endif
YCbCr444PXToXXXX_DECLARATION_R(RGBA, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(RGB, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#if SPARKYUV_FULL_CHANNELS
YCbCr444PXToXXXX_DECLARATION_R(ARGB, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(ABGR, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(BGRA, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
YCbCr444PXToXXXX_DECLARATION_R(BGR, 12, YCbCr420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef YCbCr444PXToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif