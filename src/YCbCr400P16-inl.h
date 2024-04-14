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

#if defined(SPARKYUV_YCbCr400P16_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YCbCr400P16_INL_H
#undef SPARKYUV_YCbCr400P16_INL_H
#else
#define SPARKYUV_YCbCr400P16_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include <algorithm>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, int bitDepth>
void Pixel16ToYCbCr400HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                          const uint32_t width, const uint32_t height,
                          uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
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

  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const ScalableTag<int16_t> d16;
  const RebindToUnsigned<decltype(d16)> du16;
  const Half<decltype(d16)> dh16;
  const Rebind<int32_t, decltype(dh16)> d32;
  using V16 = Vec<decltype(d16)>;
  using V32 = Vec<decltype(d32)>;
  using VU16 = Vec<decltype(du16)>;

  const auto scale = static_cast<float>( 1 << precision );
  const auto iBiasY = static_cast<int>((static_cast<float>(biasY) + 0.5f) * scale);

  const int lanes = Lanes(d16);

  const auto coeffTag = d16;

  const auto vYR = Set(coeffTag, YR);
  const auto vYG = Set(coeffTag, YG);
  const auto vYB = Set(coeffTag, YB);

  const auto vBiasY = Set(d32, iBiasY);

  const int cutOffY = rangeY + biasY;

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const int lanesForward = 1;

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<uint16_t *>(yStore);

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

      yDst += lanes;
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
    }

    yStore += yStride;
    mSource += srcStride;
  }
}

#define XXXXToYCbCr400PHWY_DECLARATION_R(pixelType, bit, yuvname) \
        void pixelType##To##yuvname##P##bit##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                            const uint32_t width, const uint32_t height,\
                                            uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                            const float kr, const float kb, const SparkYuvColorRange colorRange) {\
          Pixel16ToYCbCr400HWY<sparkyuv::PIXEL_##pixelType, bit>(src, srcStride, width, height,\
                                                          yPlane, yStride, kr, kb, colorRange);\
        }

XXXXToYCbCr400PHWY_DECLARATION_R(RGBA, 10, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr400PHWY_DECLARATION_R(ARGB, 10, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(ABGR, 10, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(BGRA, 10, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(BGR, 10, YCbCr400)
#endif

XXXXToYCbCr400PHWY_DECLARATION_R(RGBA, 12, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
XXXXToYCbCr400PHWY_DECLARATION_R(ARGB, 12, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(ABGR, 12, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(BGRA, 12, YCbCr400)
XXXXToYCbCr400PHWY_DECLARATION_R(BGR, 12, YCbCr400)
#endif

template<SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, int bitDepth>
void YCbCr400P16ToXRGB(uint16_t *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                       const uint32_t width, const uint32_t height,
                       const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
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
  auto dst = reinterpret_cast<uint8_t *>(rgbaData);

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, biasY, biasUV, rangeY, rangeUV);

  const int maxColors = static_cast<int>(::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

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

  const int iLumaCoeff = static_cast<int>(::roundf(flumaCoeff * static_cast<float>( 1 << precision )));

  const auto ivLumaCoeff = Set(di16, iLumaCoeff);
  const auto sum0 = Zero(d32);

  const int lanes = Lanes(d16);
  const int lanesForward = 1;

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  for (int y = 0; y < height; ++y) {
    auto ySrc = reinterpret_cast<const uint16_t *>(mYSrc);
    auto store = reinterpret_cast<uint16_t *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      const auto Y = Sub(BitCast(di16, LoadU(d16, ySrc)), uvCorrIY);

      V32 Yh = Zero(d32);
      const V32 Yl = WidenMulAccumulate(d32, Y, ivLumaCoeff, sum0, Yh);

      const auto
          tone = Combine(d16, BitCast(dh16, ShiftRightNarrow<6>(d32, Yh)), BitCast(dh16, ShiftRightNarrow<6>(d32, Yl)));

      StoreRGBA<PixelType>(d16, store, tone, tone, tone, vAlpha);

      store += lanes * components;
      ySrc += lanes;
    }

    for (; x < width; x += lanesForward) {
      int Y = (static_cast<int>(ySrc[0]) - biasY) * iLumaCoeff;

      int tone = Y >> precision;

      SaturatedStoreRGBA<uint16_t, int, PixelType>(store, tone, tone, tone, maxColors, maxColors);

      store += components;
      ySrc += 1;
    }
    mYSrc += yStride;
    dst += dstStride;
  }
}

#define YCbCr400PXToXXXX_DECLARATION_R(pixelType, bit, yuvname) \
    void yuvname##P##bit##To##pixelType##HWY(uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                             const uint32_t width, const uint32_t height,\
                                             const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,\
                                             const float kr, const float kb, const SparkYuvColorRange colorRange) {\
                                             YCbCr400P16ToXRGB<sparkyuv::PIXEL_##pixelType, bit>(src, srcStride, width, height,\
                                                                                        yPlane, yStride, \
                                                                                        kr, kb, colorRange);\
                                           }

YCbCr400PXToXXXX_DECLARATION_R(RGBA, 10, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(RGB, 10, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400PXToXXXX_DECLARATION_R(ARGB, 10, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(ABGR, 10, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(BGRA, 10, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(BGR, 10, YCbCr400)
#endif

YCbCr400PXToXXXX_DECLARATION_R(RGBA, 12, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(RGB, 12, YCbCr400)
#if SPARKYUV_FULL_CHANNELS
YCbCr400PXToXXXX_DECLARATION_R(ARGB, 12, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(ABGR, 12, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(BGRA, 12, YCbCr400)
YCbCr400PXToXXXX_DECLARATION_R(BGR, 12, YCbCr400)
#endif

#undef YCbCr400PXToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif