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


#if defined(SPARKYUV_TRANSFORM444_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_TRANSFORM444_INL_H
#undef SPARKYUV_TRANSFORM444_INL_H
#else
#define SPARKYUV_TRANSFORM444_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<typename T,
    SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth, int precision>
void TransformPixelToSample(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                            const uint32_t width, const uint32_t height,
                            T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                            T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                            T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                            const SparkYuvColorRange colorRange,
                            const SparkYuvTransformMatrix matrix) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  static_assert(
      chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_420 || chromaSubsample == YUV_SAMPLE_422,
      "Unsupported chroma type");
  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, biasY, biasUV, rangeY, rangeUV);

  const auto scale = static_cast<float>(1 << precision);
  const int maxColors = static_cast<int>(::powf(2.f, static_cast<float>(bitDepth)) - 1.f);
  const float rangeReformatY = static_cast<float>(rangeY) / static_cast<float>(maxColors);
  const float rangeReformatUV = static_cast<float>(rangeUV) / static_cast<float>(maxColors);

  const auto YR = static_cast<int16_t>(::roundf(matrix.Y1 * scale * rangeReformatY)),
      YG = static_cast<int16_t>(::roundf(matrix.Y2 * scale * rangeReformatY)),
      YB = static_cast<int16_t>(::roundf(matrix.Y3 * scale * rangeReformatY));
  const auto CbR = static_cast<int16_t>(::roundf(matrix.U1 * scale * rangeReformatUV)),
      CbG = static_cast<int16_t>(::roundf(matrix.U2 * scale * rangeReformatUV)),
      CbB = static_cast<int16_t>(::roundf(matrix.U3 * scale * rangeReformatUV));
  const auto CrR = static_cast<int16_t>(::roundf(matrix.V1 * scale * rangeReformatUV)),
      CrG = static_cast<int16_t>(::roundf(matrix.V2 * scale * rangeReformatUV)),
      CrB = static_cast<int16_t>(::roundf(matrix.V3 * scale * rangeReformatUV));

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);

  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const ScalableTag<uint16_t> d16;
  const Half<decltype(d16)> dh16;
  const Rebind<int32_t, decltype(dh16)> d32;
  const RebindToSigned<decltype(d16)> di16;
  const Half<decltype(d16)> dhu16;
  using V16 = Vec<decltype(d16)>;
  using V32 = Vec<decltype(d32)>;
  const RebindToUnsigned<decltype(d32)> du32;

  const auto iBiasY = static_cast<int>((static_cast<float>(biasY) + 0.5f) * scale);
  const auto iBiasUV = static_cast<int>((static_cast<float>(biasUV) + 0.5f) * scale);

  const int lanes = Lanes(d16);
  const int uvLanes = chromaSubsample == YUV_SAMPLE_444 ? Lanes(d16) : Lanes(dh16);

  const auto vYR = Set(di16, YR);
  const auto vYG = Set(di16, YG);
  const auto vYB = Set(di16, YB);

  const auto vCbR = Set(di16, CbR);
  const auto vCbG = Set(di16, CbG);
  const auto vCbB = Set(di16, CbB);

  const auto vCrR = Set(di16, CrR);
  const auto vCrG = Set(di16, CrG);
  const auto vCrB = Set(di16, CrB);

  const auto vBiasY = Set(d32, iBiasY);
  const auto vBiasUV = Set(d32, iBiasUV);

  const int cutOffY = rangeY + biasY;
  const int cutOffUV = rangeUV + biasY;

  const auto vCutOffY = Set(di16, cutOffY);
  const auto vCutOffUV = Set(di16, cutOffUV);
  const auto vLowCutOffY = Set(di16, colorRange == sparkyuv::YUV_RANGE_TV ? biasY : 0);
  const auto viZeros = Zero(di16);

  const int components = getPixelTypeComponents(PixelType);

  const int lanesForward = getYuvChromaPixels(chromaSubsample);

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<T *>(yStore);
    auto uDst = reinterpret_cast<T *>(uStore);
    auto vDst = reinterpret_cast<T *>(vStore);

    auto mSrc = reinterpret_cast<const T *>(mSource);

    for (; x + lanes < width; x += lanes) {
      V16 R;
      V16 G;
      V16 B;
      V16 A;

      if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(d16)> d8;
        LoadRGBTo16<PixelType>(d8, reinterpret_cast<const uint8_t *>(mSrc), R, G, B);
      } else if (std::is_same<T, uint16_t>::value) {
        LoadRGBA<PixelType>(d16, reinterpret_cast<const uint16_t *>(mSrc), R, G, B, A);
      }

      V32 Yl = vBiasY;
      V32 Yh = vBiasY;
      Yl = WidenMulAccumulate(d32, BitCast(di16, R), vYR, Yl, Yh);
      Yl = WidenMulAccumulate(d32, BitCast(di16, G), vYG, Yl, Yh);
      Yl = WidenMulAccumulate(d32, BitCast(di16, B), vYB, Yl, Yh);

      if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          auto nextRow = reinterpret_cast<const T *>(mSrc);
          if (y + 1 < height - 1) {
            nextRow = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSrc) + srcStride);
          }

          V16 R1;
          V16 G1;
          V16 B1;
          V16 A1;
          if (std::is_same<T, uint16_t>::value) {
            LoadRGBA<PixelType>(d16, reinterpret_cast<const uint16_t *>(nextRow), R1, G1, B1, A1);
          } else if (std::is_same<T, uint8_t>::value) {
            const Rebind<uint8_t, decltype(d16)> d8;
            LoadRGBTo16<PixelType>(d8, reinterpret_cast<const uint8_t *>(mSrc), R1, G1, B1);
          }

          R = AddAndHalf(d16, R, R1);
          G = AddAndHalf(d16, G, G1);
          B = AddAndHalf(d16, B, B1);
        }
      }

      V32 Cbl = vBiasUV;
      V32 Cbh = vBiasUV;
      Cbl = WidenMulAccumulate(d32, BitCast(di16, R), vCbR, Cbl, Cbh);
      Cbl = WidenMulAccumulate(d32, BitCast(di16, G), vCbG, Cbl, Cbh);
      Cbl = WidenMulAccumulate(d32, BitCast(di16, B), vCbB, Cbl, Cbh);

      V32 Crl = vBiasUV;
      V32 Crh = vBiasUV;
      Crl = WidenMulAccumulate(d32, BitCast(di16, R), vCrR, Crl, Crh);
      Crl = WidenMulAccumulate(d32, BitCast(di16, G), vCrG, Crl, Crh);
      Crl = WidenMulAccumulate(d32, BitCast(di16, B), vCrB, Crl, Crh);

      const auto Y = BitCast(d16,
                             Clamp(Combine(di16,
                                           ShiftRightNarrow<precision>(d32, Yh),
                                           ShiftRightNarrow<precision>(d32, Yl)), vLowCutOffY, vCutOffY));
      const auto Cb = BitCast(d16,
                              Clamp(Combine(di16,
                                            ShiftRightNarrow<precision>(d32, Cbh),
                                            ShiftRightNarrow<precision>(d32, Cbl)), viZeros, vCutOffUV));
      const auto Cr = BitCast(d16,
                              Clamp(Combine(di16,
                                            ShiftRightNarrow<precision>(d32, Crh),
                                            ShiftRightNarrow<precision>(d32, Crl)), viZeros, vCutOffUV));

      if (std::is_same<T, uint16_t>::value) {
        StoreU(Y, d16, reinterpret_cast<uint16_t *>(yDst));
        if (chromaSubsample == YUV_SAMPLE_444) {
          StoreU(Cb, d16, reinterpret_cast<uint16_t *>(uDst));
          StoreU(Cr, d16, reinterpret_cast<uint16_t *>(vDst));
        } else if (chromaSubsample == YUV_SAMPLE_422) {
          const auto cbh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cb)));
          const auto crh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cr)));
          StoreU(cbh, dhu16, reinterpret_cast<uint16_t *>(uDst));
          StoreU(crh, dhu16, reinterpret_cast<uint16_t *>(vDst));
        } else if (chromaSubsample == YUV_SAMPLE_420) {
          if (!(y & 1)) {
            const auto cbh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cb)));
            const auto crh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cr)));
            StoreU(cbh, dhu16, reinterpret_cast<uint16_t *>(uDst));
            StoreU(crh, dhu16, reinterpret_cast<uint16_t *>(vDst));
          }
        }
      } else if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(d16)> d8;
        const Rebind<uint8_t, decltype(dhu16)> dh8;
        StoreU(DemoteTo(d8, Y), d8, reinterpret_cast<uint8_t *>(yDst));
        if (chromaSubsample == YUV_SAMPLE_444) {
          StoreU(DemoteTo(d8, Cb), d8, reinterpret_cast<uint8_t *>(uDst));
          StoreU(DemoteTo(d8, Cr), d8, reinterpret_cast<uint8_t *>(vDst));
        } else if (chromaSubsample == YUV_SAMPLE_422) {
          const auto cbh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cb)));
          const auto crh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cr)));
          StoreU(DemoteTo(dh8, cbh), dh8, reinterpret_cast<uint8_t *>(uDst));
          StoreU(DemoteTo(dh8, crh), dh8, reinterpret_cast<uint8_t *>(vDst));
        } else if (chromaSubsample == YUV_SAMPLE_420) {
          if (!(y & 1)) {
            const auto cbh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cb)));
            const auto crh = BitCast(dhu16, ShiftRightNarrow<1>(du32, SumsOf2(Cr)));
            StoreU(DemoteTo(dh8, cbh), dh8, reinterpret_cast<uint8_t *>(uDst));
            StoreU(DemoteTo(dh8, crh), dh8, reinterpret_cast<uint8_t *>(vDst));
          }
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

      LoadRGB<T, int, PixelType>(mSrc, r, g, b);

      int Y = ((r * static_cast<int>(YR) + g * static_cast<int>(YG) + b * static_cast<int>(YB) + iBiasY) >> precision);
      yDst[0] = std::clamp(Y, static_cast<int>(biasY), cutOffY);
      yDst += 1;
      mSrc += components;

      if (chromaSubsample == YUV_SAMPLE_422) {
        if (x + 1 < width) {
          int r1 = r, g1 = g, b1 = b;

          LoadRGB<T, int, PixelType>(mSrc, r1, g1, b1);

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
          auto nextRow = reinterpret_cast<const T *>(mSrc);
          if (y + 1 < height - 1) {
            nextRow = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(mSrc) + srcStride);
          }

          int r1 = r, g1 = g, b1 = b;
          int r2 = r, g2 = g, b2 = b;
          int r3 = r, g3 = g, b3 = b;

          LoadRGB<T, int, PixelType>(nextRow, r2, g2, b2);

          nextRow += components;

          if (x + 1 < width) {
            LoadRGB<T, int, PixelType>(nextRow, r3, g3, b3);
            LoadRGB<T, int, PixelType>(mSrc, r1, g1, b1);

            int Y1 = ((r1 * static_cast<int>(YR) + g1 * static_cast<int>(YG) + b1 * static_cast<int>(YB) + iBiasY)
                >> precision);
            yDst[0] = std::clamp(Y1, static_cast<int>(biasY), cutOffY);
            yDst += 1;
            mSrc += components;
          }

          r = (r + r1 + r2 + r3) >> 2;
          g = (g + g1 + g2 + g3) >> 2;
          b = (b + b1 + b2 + b3) >> 2;
        } else {
          if (x + 1 < width) {
            int r1 = r, g1 = g, b1 = b;
            LoadRGB<T, int, PixelType>(mSrc, r1, g1, b1);

            int Y1 = ((r1 * static_cast<int>(YR) + g1 * static_cast<int>(YG) + b1 * static_cast<int>(YB) + iBiasY)
                >> precision);
            yDst[0] = std::clamp(Y1, static_cast<int>(biasY), cutOffY);
            yDst += 1;
            mSrc += components;
          }
        }
      }

      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
        int Cb =
            ((r * static_cast<int>(CbR) + g * static_cast<int>(CbG) + b * static_cast<int>(CbB) + iBiasUV)
                >> precision);
        int Cr =
            ((r * static_cast<int>(CrR) + g * static_cast<int>(CrG) + b * static_cast<int>(CrB) + iBiasUV)
                >> precision);

        uDst[0] = Cb;
        vDst[0] = Cr;
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          int Cb =
              ((r * static_cast<int>(CbR) + g * static_cast<int>(CbG) + b * static_cast<int>(CbB) + iBiasUV)
                  >> precision);
          int Cr =
              ((r * static_cast<int>(CrR) + g * static_cast<int>(CrG) + b * static_cast<int>(CrB) + iBiasUV)
                  >> precision);

          uDst[0] = Cb;
          vDst[0] = Cr;
        }
      }

      uDst += 1;
      vDst += 1;
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

template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth, int precision>
void TransformYUVToRGBMatrix(T *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                             const uint32_t width, const uint32_t height,
                             const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                             const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                             const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                             const SparkYuvColorRange colorRange,
                             const SparkYuvTransformMatrix matrix) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  static_assert(
      chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_420 || chromaSubsample == YUV_SAMPLE_422,
      "Unsupported chroma type");
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
  const auto vMaxColors = Set(di16, maxColors);
  const auto zeros = Zero(di16);
  const auto v32Zeros = Zero(d32);

  const auto scale = static_cast<float>(1 << precision);
  const float rangeReformatY = static_cast<float>(maxColors) / static_cast<float>(rangeY);
  const float rangeReformatUV = static_cast<float>(maxColors) / static_cast<float>(rangeUV);

  const int YR = static_cast<int>(::roundf(matrix.Y1 * scale * rangeReformatY)),
      YG = static_cast<int>(::roundf(matrix.Y2 * scale * rangeReformatY)),
      YB = static_cast<int>(::roundf(matrix.Y3 * scale * rangeReformatY));
  const int CbR = static_cast<int>(::roundf(matrix.U1 * scale * rangeReformatUV)),
      CbG = static_cast<int>(::roundf(matrix.U2 * scale * rangeReformatUV)),
      CbB = static_cast<int>(::roundf(matrix.U3 * scale * rangeReformatUV));
  const int CrR = static_cast<int>(::roundf(matrix.V1 * scale * rangeReformatUV)),
      CrG = static_cast<int>(::roundf(matrix.V2 * scale * rangeReformatUV)),
      CrB = static_cast<int>(::roundf(matrix.V3 * scale * rangeReformatUV));

  const auto vYR = Set(di16, YR);
  const auto vYG = Set(di16, YG);
  const auto vYB = Set(di16, YB);
  const auto vCbR = Set(di16, CbR);
  const auto vCbG = Set(di16, CbG);
  const auto vCbB = Set(di16, CbB);
  const auto vCrR = Set(di16, CrR);
  const auto vCrG = Set(di16, CrG);
  const auto vCrB = Set(di16, CrB);

  const int lanes = Lanes(d16);
  const int lanesForward = getYuvChromaPixels(chromaSubsample);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh16);

  const int components = getPixelTypeComponents(PixelType);

  for (int y = 0; y < height; ++y) {
    auto CbSource = reinterpret_cast<const T *>(mUSrc);
    auto CrSource = reinterpret_cast<const T *>(mVSrc);
    auto ySrc = reinterpret_cast<const T *>(mYSrc);
    auto store = reinterpret_cast<T *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      VI16 Y;
      if (std::is_same<T, uint16_t>::value) {
        Y = Sub(BitCast(di16, LoadU(d16, reinterpret_cast<const uint16_t *>(ySrc))), uvCorrIY);
      } else if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(d16)> d8;
        Y = Sub(BitCast(di16, PromoteTo(d16, LoadU(d8, reinterpret_cast<const uint8_t *>(ySrc)))), uvCorrIY);
      } else {
        static_assert("Must not be reached");
      }

      VI16 U;
      VI16 V;
      if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(d16)> d8;
        const Rebind<uint8_t, decltype(dh16)> dh8;
        if (chromaSubsample == YUV_SAMPLE_444) {
          U = Sub(BitCast(di16, PromoteTo(d16, LoadU(d8, reinterpret_cast<const uint8_t *>(CbSource)))), uvCorrection);
          V = Sub(BitCast(di16, PromoteTo(d16, LoadU(d8, reinterpret_cast<const uint8_t *>(CrSource)))), uvCorrection);
        } else if (chromaSubsample == YUV_SAMPLE_420 || chromaSubsample == YUV_SAMPLE_422) {
          auto cbh = PromoteTo(dh16, LoadU(dh8, reinterpret_cast<const uint8_t *>(CbSource)));
          auto crh = PromoteTo(dh16, LoadU(dh8, reinterpret_cast<const uint8_t *>(CrSource)));
          const auto ulc = Sub(BitCast(di16, Combine(d16, cbh, cbh)), uvCorrection);
          U = InterleaveLower(ulc, ulc);
          const auto vlc = Sub(BitCast(di16, Combine(d16, crh, crh)), uvCorrection);
          V = InterleaveLower(vlc, vlc);
        } else {
          static_assert("Must not be reached");
        }
      } else if (std::is_same<T, uint16_t>::value) {
        if (chromaSubsample == YUV_SAMPLE_444) {
          U = Sub(BitCast(di16, LoadU(d16, reinterpret_cast<const uint16_t *>(CbSource))), uvCorrection);
          V = Sub(BitCast(di16, LoadU(d16, reinterpret_cast<const uint16_t *>(CrSource))), uvCorrection);
        } else if (chromaSubsample == YUV_SAMPLE_420 || chromaSubsample == YUV_SAMPLE_422) {
          auto cbh = LoadU(dh16, reinterpret_cast<const uint16_t *>(CbSource));
          auto crh = LoadU(dh16, reinterpret_cast<const uint16_t *>(CrSource));
          const auto ulc = Sub(BitCast(di16, Combine(d16, cbh, cbh)), uvCorrection);
          U = InterleaveLower(ulc, ulc);
          const auto vlc = Sub(BitCast(di16, Combine(d16, crh, crh)), uvCorrection);
          V = InterleaveLower(vlc, vlc);
        } else {
          static_assert("Must not be reached");
        }
      }

      /*
       * int R = (Y * YR + U * YG + V * YB) >> precision;
         int G = (Y * CbR + U * CbG + V * CbB) >> precision;
         int B = (Y * CrR + U * CrG + V * CrB) >> precision;
       */

      V32 rl = v32Zeros;
      V32 rh = v32Zeros;
      rl = WidenMulAccumulate(d32, Y, vYR, rl, rh);
      rl = WidenMulAccumulate(d32, U, vYG, rl, rh);
      rl = WidenMulAccumulate(d32, V, vYB, rl, rh);

      V32 bl = v32Zeros;
      V32 bh = v32Zeros;

      bl = WidenMulAccumulate(d32, Y, vCrR, bl, bh);
      bl = WidenMulAccumulate(d32, U, vCrG, bl, bh);
      bl = WidenMulAccumulate(d32, V, vCrB, bl, bh);

      V32 gl = v32Zeros;
      V32 gh = v32Zeros;

      gl = WidenMulAccumulate(d32, Y, vCbR, gl, gh);
      gl = WidenMulAccumulate(d32, U, vCbG, gl, gh);
      gl = WidenMulAccumulate(d32, V, vCbB, gl, gh);

      // In 12 bit overflow is highly likely so there is a need to handle it slightly in another way
      V16 r, g, b;
      r = BitCast(d16, Clamp(Combine(di16, ShiftRightNarrow<precision>(d32, rh),
                                     ShiftRightNarrow<precision>(d32, rl)), zeros, vMaxColors));
      g = BitCast(d16, Clamp(Combine(di16, ShiftRightNarrow<precision>(d32, gh),
                                     ShiftRightNarrow<precision>(d32, gl)), zeros, vMaxColors));
      b = BitCast(d16, Clamp(Combine(di16, ShiftRightNarrow<precision>(d32, bh),
                                     ShiftRightNarrow<precision>(d32, bl)), zeros, vMaxColors));

      if (std::is_same<T, uint16_t>::value) {
        StoreRGBA<PixelType>(d16, reinterpret_cast<uint16_t *>(store), r, g, b, vAlpha);
      } else if (std::is_same<T, uint8_t>::value) {
        StoreRGBA<PixelType>(d16, reinterpret_cast<uint8_t *>(store), r, g, b, vAlpha);
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
      }

      store += lanes * components;
      ySrc += lanes;

      CbSource += uvLanes;
      CrSource += uvLanes;
    }

    for (; x < width; x += lanesForward) {
      const int uValue = reinterpret_cast<const T *>(CbSource)[0];
      const int vValue = reinterpret_cast<const T *>(CrSource)[0];

      int Y = (static_cast<int>(ySrc[0]) - biasY);
      const int U = (static_cast<int>(uValue) - biasUV);
      const int V = (static_cast<int>(vValue) - biasUV);

      int R = (Y * YR + U * YG + V * YB) >> precision;
      int G = (Y * CbR + U * CbG + V * CbB) >> precision;
      int B = (Y * CrR + U * CrG + V * CrB) >> precision;

      SaturatedStoreRGBA<T, int, PixelType>(store, R, G, B, maxColors, maxColors);

      store += components;
      ySrc += 1;

      if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        if (x + 1 < width) {
          int Y1 = (static_cast<int>(ySrc[0]) - biasY);
          int R1 = (Y1 * YR + U * YG + V * YB) >> precision;
          int G1 = (Y1 * CbR + U * CbG + V * CbB) >> precision;
          int B1 = (Y1 * CrR + U * CrG + V * CrB) >> precision;
          SaturatedStoreRGBA<T, int, PixelType>(store, R1, G1, B1, maxColors, maxColors);
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

}
HWY_AFTER_NAMESPACE();

#endif