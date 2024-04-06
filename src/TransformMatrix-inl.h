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
  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, biasY, biasUV, rangeY, rangeUV);

  const auto scale = static_cast<float>(1 << precision);
  const int maxColors = static_cast<int>(std::powf(2.f, static_cast<float>(bitDepth)) - 1.f);
  const float rangeReformatY = static_cast<float>(rangeY) / static_cast<float>(maxColors);
  const float rangeReformatUV = static_cast<float>(rangeUV) / static_cast<float>(maxColors);

  auto YR = static_cast<int16_t>(std::roundf(matrix.Y1 * scale * rangeReformatY)),
      YG = static_cast<int16_t>(std::roundf(matrix.Y2 * scale * rangeReformatY)),
      YB = static_cast<int16_t>(std::roundf(matrix.Y3 * scale * rangeReformatY));
  auto CbR = static_cast<int16_t>(std::roundf(matrix.U1 * scale * rangeReformatUV)),
      CbG = static_cast<int16_t>(std::roundf(matrix.U2 * scale * rangeReformatUV)),
      CbB = static_cast<int16_t>(std::roundf(matrix.U3 * scale * rangeReformatUV));
  auto CrR = static_cast<int16_t>(std::roundf(matrix.V1 * scale * rangeReformatUV)),
      CrG = static_cast<int16_t>(std::roundf(matrix.V2 * scale * rangeReformatUV)),
      CrB = static_cast<int16_t>(std::roundf(matrix.V3 * scale * rangeReformatUV));

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);

  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const ScalableTag<uint16_t> d16;
  const Half<decltype(d16)> dh16;
  const Rebind<int32_t, decltype(dh16)> d32;
  const Half<decltype(d16)> dhu16;
  using V16 = Vec<decltype(d16)>;
  using V32 = Vec<decltype(d32)>;

  const auto iBiasY = static_cast<int>((static_cast<float>(biasY) + 0.5f) * scale);
  const auto iBiasUV = static_cast<int>((static_cast<float>(biasUV) + 0.5f) * scale);

  const int lanes = Lanes(d16);
  const int uvLanes = chromaSubsample == YUV_SAMPLE_444 ? Lanes(d16) : Lanes(dh16);

  const auto coeffTag = d32;

  const auto vYR = Set(coeffTag, YR);
  const auto vYG = Set(coeffTag, YG);
  const auto vYB = Set(coeffTag, YB);

  const auto vCbR = Set(coeffTag, CbR);
  const auto vCbG = Set(coeffTag, CbG);
  const auto vCbB = Set(coeffTag, CbB);

  const auto vCrR = Set(coeffTag, CrR);
  const auto vCrG = Set(coeffTag, CrG);
  const auto vCrB = Set(coeffTag, CrB);

  const auto vBiasY = Set(d32, iBiasY);
  const auto vBiasUV = Set(d32, iBiasUV);

  const int cutOffY = rangeY + biasY;
  const int cutOffUV = rangeUV + biasUV;

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const int lanesForward = (chromaSubsample == YUV_SAMPLE_444) ? 1 : 2;

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

      V32 Rl = PromoteLowerTo(d32, R);
      V32 Rh = PromoteUpperTo(d32, R);
      V32 Bl = PromoteLowerTo(d32, B);
      V32 Bh = PromoteUpperTo(d32, B);
      V32 Gl = PromoteLowerTo(d32, G);
      V32 Gh = PromoteUpperTo(d32, G);
      const auto Yl = ShiftRight<precision>(MulAdd(Rl, vYR,
                                                   MulAdd(Gl, vYG,
                                                          MulAdd(Bl, vYB, vBiasY))));
      const auto Yh = ShiftRight<precision>(MulAdd(Rh, vYR,
                                                   MulAdd(Gh, vYG,
                                                          MulAdd(Bh, vYB, vBiasY))));

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

          Rl = ShiftRight<1>(Add(Rl, PromoteLowerTo(d32, R1)));
          Gl = ShiftRight<1>(Add(Gl, PromoteLowerTo(d32, G1)));
          Bl = ShiftRight<1>(Add(Bl, PromoteLowerTo(d32, B1)));

          Rh = ShiftRight<1>(Add(Rh, PromoteUpperTo(d32, R1)));
          Gh = ShiftRight<1>(Add(Gh, PromoteUpperTo(d32, G1)));
          Bh = ShiftRight<1>(Add(Bh, PromoteUpperTo(d32, B1)));
        }
      }

      const auto Cbl = ShiftRight<precision>(Add(MulAdd(Bl, vCbB,
                                                        MulAdd(Gl, vCbG,
                                                               Mul(Rl, vCbR))), vBiasUV));
      const auto Cbh = ShiftRight<precision>(Add(MulAdd(Bh, vCbB,
                                                        MulAdd(Gh, vCbG,
                                                               Mul(Rh, vCbR))), vBiasUV));

      const auto Crh = ShiftRight<precision>(Add(MulAdd(Rh, vCrR,
                                                        MulAdd(Gh, vCrG,
                                                               Mul(Bh, vCrB))), vBiasUV));

      const auto Crl = ShiftRight<precision>(Add(MulAdd(Rl, vCrR,
                                                        MulAdd(Gl, vCrG,
                                                               Mul(Bl, vCrB))), vBiasUV));

      const auto Y = Combine(d16, DemoteTo(dh16, Yh), DemoteTo(dh16, Yl));
      const auto Cb = Combine(d16, DemoteTo(dh16, Cbh), DemoteTo(dh16, Cbl));
      const auto Cr = Combine(d16, DemoteTo(dh16, Crh), DemoteTo(dh16, Crl));

      if (std::is_same<T, uint16_t>::value) {
        StoreU(Y, d16, reinterpret_cast<uint16_t *>(yDst));
        if (chromaSubsample == YUV_SAMPLE_444) {
          StoreU(Cb, d16, reinterpret_cast<uint16_t *>(uDst));
          StoreU(Cr, d16, reinterpret_cast<uint16_t *>(vDst));
        } else if (chromaSubsample == YUV_SAMPLE_422) {
          const auto cbh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cb)));
          const auto crh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cr)));
          StoreU(cbh, dhu16, reinterpret_cast<uint16_t *>(uDst));
          StoreU(crh, dhu16, reinterpret_cast<uint16_t *>(vDst));
        } else if (chromaSubsample == YUV_SAMPLE_420) {
          if (!(y & 1)) {
            const auto cbh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cb)));
            const auto crh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cr)));
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
          const auto cbh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cb)));
          const auto crh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cr)));
          StoreU(DemoteTo(dh8, cbh), dh8, reinterpret_cast<uint8_t *>(uDst));
          StoreU(DemoteTo(dh8, crh), dh8, reinterpret_cast<uint8_t *>(vDst));
        } else if (chromaSubsample == YUV_SAMPLE_420) {
          if (!(y & 1)) {
            const auto cbh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cb)));
            const auto crh = DemoteTo(dhu16, ShiftRight<1>(SumsOf2(Cr)));
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

template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA, SparkYuvChromaSubsample chromaSubsample, int bitDepth, int precision>
void TransformYUVToRGBMatrix(T *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                             const uint32_t width, const uint32_t height,
                             const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                             const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                             const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                             const SparkYuvColorRange colorRange,
                             const SparkYuvTransformMatrix matrix) {
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

  const int maxColors = static_cast<int>(std::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

  const auto uvCorrection = Set(di16, biasUV);
  const V16 vAlpha = Set(d16, maxColors);
  const auto uvCorrIY = Set(di16, biasY);

  const auto scale = static_cast<float>(1 << precision);
  const float rangeReformatY = static_cast<float>(maxColors) / static_cast<float>(rangeY);
  const float rangeReformatUV = static_cast<float>(maxColors) / static_cast<float>(rangeUV);

  auto YR = static_cast<int16_t>(std::roundf(matrix.Y1 * scale * rangeReformatY)),
      YG = static_cast<int16_t>(std::roundf(matrix.Y2 * scale * rangeReformatY)),
      YB = static_cast<int16_t>(std::roundf(matrix.Y3 * scale * rangeReformatY));
  auto CbR = static_cast<int16_t>(std::roundf(matrix.U1 * scale * rangeReformatUV)),
      CbG = static_cast<int16_t>(std::roundf(matrix.U2 * scale * rangeReformatUV)),
      CbB = static_cast<int16_t>(std::roundf(matrix.U3 * scale * rangeReformatUV));
  auto CrR = static_cast<int16_t>(std::roundf(matrix.V1 * scale * rangeReformatUV)),
      CrG = static_cast<int16_t>(std::roundf(matrix.V2 * scale * rangeReformatUV)),
      CrB = static_cast<int16_t>(std::roundf(matrix.V3 * scale * rangeReformatUV));

  const auto vYR = Set(d32, YR);
  const auto vYG = Set(d32, YG);
  const auto vYB = Set(d32, YB);
  const auto vCbR = Set(d32, CbR);
  const auto vCbG = Set(d32, CbG);
  const auto vCbB = Set(d32, CbB);
  const auto vCrR = Set(d32, CrR);
  const auto vCrG = Set(d32, CrG);
  const auto vCrB = Set(d32, CrB);

  const int lanes = Lanes(d16);
  const int lanesForward = (chromaSubsample == YUV_SAMPLE_444) ? 1 : 2;
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh16);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

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

      const auto Yh = PromoteUpperTo(d32, Y);
      const auto Yl = PromoteLowerTo(d32, Y);

      const auto ul = PromoteLowerTo(d32, U);
      const auto uh = PromoteUpperTo(d32, U);

      const auto vl = PromoteLowerTo(d32, V);
      const auto vh = PromoteUpperTo(d32, V);

      /*
       *       int R = (Y * YR + U * YG + V * YB) >> precision;
      int G = (Y * CbR + U * CbG + V * CbB) >> precision;
      int B = (Y * CrR + U * CrG + V * CrB) >> precision;
       */

      const auto rl = MulAdd(Yl, vYR, MulAdd(ul, vYG, Mul(vl, vYB)));
      const auto rh = MulAdd(Yh, vYR, MulAdd(uh, vYG, Mul(vh, vYB)));

      const auto bl = MulAdd(Yl, vCrR, MulAdd(ul, vCrG, Mul(vl, vCrB)));
      const auto bh = MulAdd(Yh, vCrR, MulAdd(uh, vCrG, Mul(vh, vCrB)));

      const auto gl = MulAdd(Yl, vCbR, MulAdd(ul, vCbG, Mul(vl, vCbB)));
      const auto gh = MulAdd(Yh, vCbR, MulAdd(uh, vCbG, Mul(vh, vCbB)));

      // In 12 bit overflow is highly likely so there is a need to handle it slightly in another way
      V16 r, g, b;
      if (bitDepth == 12 || precision > 8) {
        r = Combine(d16, DemoteTo(dh16, ShiftRight<precision>(rh)), DemoteTo(dh16, ShiftRight<precision>(rl)));
        g = Combine(d16, DemoteTo(dh16, ShiftRight<precision>(gh)), DemoteTo(dh16, ShiftRight<precision>(gl)));
        b = Combine(d16, DemoteTo(dh16, ShiftRight<precision>(bh)), DemoteTo(dh16, ShiftRight<precision>(bl)));
      } else {
        r = ShiftRight<precision>(Combine(d16, DemoteTo(dh16, rh), DemoteTo(dh16, rl)));
        g = ShiftRight<precision>(Combine(d16, DemoteTo(dh16, gh), DemoteTo(dh16, gl)));
        b = ShiftRight<precision>(Combine(d16, DemoteTo(dh16, bh), DemoteTo(dh16, bl)));
      }

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
      const T uValue = reinterpret_cast<const T *>(CbSource)[0];
      const T vValue = reinterpret_cast<const T *>(CrSource)[0];

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
          int B1 = (Y1 * CbR + U * CbG + V * CbB) >> precision;
          int G1 = (Y1 * CrR + U * CrG + V * CrB) >> precision;
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