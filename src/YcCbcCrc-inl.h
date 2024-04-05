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

#if defined(SPARKYUV__YUVBT2020CL_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__YUVBT2020CL_INL_H
#undef SPARKYUV__YUVBT2020CL_INL_H
#else
#define SPARKYUV__YUVBT2020CL_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include "Eotf-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

void computeYcCbcCrcCutoffs(const SparkYuvTransferFunction transferFunction,
                            const float kr,
                            const float kb,
                            float &Nb,
                            float &Pb,
                            float &Nr,
                            float &Pr) {
  Nb = Oetf(1.f - kb, transferFunction);
  Pb = 1.f - Oetf(kb, transferFunction);
  Nr = Oetf(1.f - kr, transferFunction);
  Pr = 1.f - Oetf(kr, transferFunction);
}

template<typename D, typename V = Vec<D>, HWY_IF_FLOAT_D(D)>
EOTF_INLINE V computeYcCbcCrcEquation(D d, V dx, V low, V high, V scaleLow, V scaleHigh) {
  const auto zeros = Zero(d);
  auto v = IfThenElse(And(dx >= low, dx <= zeros), Mul(dx, scaleLow), zeros);
  v = IfThenElse(And(dx > zeros, dx < high), Mul(dx, scaleHigh), v);
  return v;
}

EOTF_INLINE float computeYcCbcCrcEquation(float dx, float low, float high, float scaleLow, float scaleHigh) {
  float Eb;
  if (dx >= -low && dx <= 0.f) {
    Eb = dx * scaleLow;
  } else if (dx > 0 && dx < high) {
    Eb = dx * scaleHigh;
  } else {
    Eb = 0.f;
  }
  return Eb;
}
/**
* @brief It will be good to declare a type of transfer function
 * and compile each variant separately however increase of binary size about 400%
 * and compile time abound 1000% sacrifices this benefit
*/
template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth>
std::enable_if_t<std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, void>
PixelToYcCbcCrcHWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                   const uint32_t width, const uint32_t height,
                   T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                   T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                   T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                   const float kr, const float kb, const SparkYuvColorRange colorRange,
                   const SparkYuvTransferFunction TransferFunction) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, biasY, biasUV, rangeY, rangeUV);

  using SignedT = typename std::make_signed<T>::type;

  const int maxColors = static_cast<int>(std::powf(2.f, static_cast<float>(bitDepth)) - 1.f);
  const float linearScale = 1.f / static_cast<float>(maxColors);

  float Nb, Pb, Nr, Pr;
  computeYcCbcCrcCutoffs(TransferFunction, kr, kb, Nb, Pb, Nr, Pr);

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);
  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const float EpbLow = 1.f / (2.f * Nb);
  const float EpbHigh = 1.f / (2.f * Pb);
  const float EprLow = 1.f / (2.f * Nr);
  const float EprHigh = 1.f / (2.f * Pr);

  const float kg = 1.f - kr - kb;

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const int lanesForward = (chromaSubsample == YUV_SAMPLE_444) ? 1 : 2;

  const float divisor2 = 1.f / 2.f;
  const float divisor4 = 1.f / 4.f;

  const ScalableTag<uint16_t> du16;
  const Half<decltype(du16)> dhu16;
  const Rebind<T, decltype(du16)> d;
  const Half<decltype(d)> dh;
  const int lanes = Lanes(d);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh);
  const Rebind<uint32_t, decltype(dhu16)> du32;
  const Rebind<float, decltype(du32)> df;
  using VF = Vec<decltype(df)>;
  using VU = Vec<decltype(du16)>;

  const auto vScaleRangeY = Set(df, rangeY);
  const auto vScaleRangeUV = Set(df, rangeUV);
  const auto vBiasY = Set(df, biasY);
  const auto vBiasUV = Set(df, biasUV);
  const auto vNb = Set(df, -Nb);
  const auto vPb = Set(df, Pb);
  const auto vNr = Set(df, -Nr);
  const auto vPr = Set(df, Pr);
  const auto vEpbLow = Set(df, EpbLow);
  const auto vEpbHigh = Set(df, EpbHigh);
  const auto vEprLow = Set(df, EprLow);
  const auto vEprHigh = Set(df, EprHigh);
  const auto vLinearScale = Set(df, linearScale);
  const auto vKr = Set(df, kr);
  const auto vKb = Set(df, kb);
  const auto vKg = Set(df, kg);
  const auto alpha = Set(d, maxColors);
  const auto v420Scale = Set(df, 0.5f);

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<T *>(yStore);
    auto uDst = reinterpret_cast<T *>(uStore);
    auto vDst = reinterpret_cast<T *>(vStore);
    auto mSrc = reinterpret_cast<const T *>(mSource);

    for (; x + lanes < width; x += lanes) {
      VF Rh, Rl, Gh, Gl, Bh, Bl, Ah, Al;
      if (std::is_same<T, uint16_t>::value) {
        VU R16, G16, B16, A16;
        LoadRGBA<PixelType>(du16, reinterpret_cast<const uint16_t *>(mSrc), R16, G16, B16, A16);

        Rh = ConvertTo(df, PromoteUpperTo(du32, R16));
        Rl = ConvertTo(df, PromoteLowerTo(du32, R16));
        Gh = ConvertTo(df, PromoteUpperTo(du32, G16));
        Gl = ConvertTo(df, PromoteLowerTo(du32, G16));
        Bh = ConvertTo(df, PromoteUpperTo(du32, B16));
        Bl = ConvertTo(df, PromoteLowerTo(du32, B16));
      } else if (std::is_same<T, uint8_t>::value) {
        VU R16, G16, B16, A16;
        LoadRGB<PixelType>(du16, reinterpret_cast<const uint8_t *>(mSrc), R16, G16, B16);

        Rh = ConvertTo(df, PromoteTo(du32, UpperHalf(dhu16, R16)));
        Rl = ConvertTo(df, PromoteLowerTo(du32, R16));
        Gh = ConvertTo(df, PromoteTo(du32, UpperHalf(dhu16, G16)));
        Gl = ConvertTo(df, PromoteLowerTo(du32, G16));
        Bh = ConvertTo(df, PromoteTo(du32, UpperHalf(dhu16, B16)));
        Bl = ConvertTo(df, PromoteLowerTo(du32, B16));
      }

      Rh = Mul(Rh, vLinearScale);
      Rl = Mul(Rl, vLinearScale);
      Gh = Mul(Gh, vLinearScale);
      Gl = Mul(Gl, vLinearScale);
      Bh = Mul(Bh, vLinearScale);
      Bl = Mul(Bl, vLinearScale);

      auto Eh = Oetf(df, MulAdd(Eotf(df, Rh, TransferFunction), vKr,
                                MulAdd(Eotf(df, Gh, TransferFunction), vKg,
                                       Mul(Eotf(df, Bh, TransferFunction), vKb))), TransferFunction);
      auto El = Oetf(df, MulAdd(Eotf(df, Rl, TransferFunction), vKr,
                                MulAdd(Eotf(df, Gl, TransferFunction), vKg,
                                       Mul(Eotf(df, Bl, TransferFunction), vKb))), TransferFunction);

      auto Yh = Add(Round(Mul(Eh, vScaleRangeY)), vBiasY);
      auto Yl = Add(Round(Mul(El, vScaleRangeY)), vBiasY);

      if (std::is_same<T, uint16_t>::value) {
        const auto Y = Combine(du16, DemoteTo(dhu16, ConvertTo(du32, Yh)),
                               DemoteTo(dhu16, ConvertTo(du32, Yl)));
        StoreU(Y, du16, reinterpret_cast<uint16_t *>(yDst));
      } else if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(du16)> du8;
        using V8 = Vec<decltype(du8)>;
        const Half<decltype(du8)> dhu8;
        const auto Y = Combine(du8, DemoteTo(dhu8, ConvertTo(du32, Yh)),
                               DemoteTo(dhu8, ConvertTo(du32, Yl)));
        StoreU(Y, du8, reinterpret_cast<uint8_t *>(yDst));
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected type");
      }

      if (chromaSubsample == YUV_SAMPLE_420 || chromaSubsample == YUV_SAMPLE_422) {
        if (chromaSubsample == YUV_SAMPLE_420) {
          if (!(y & 1) && y + 1 < height) {
            VF vRh, vRl, vGh, vGl, vBh, vBl, vAh, vAl;
            if (std::is_same<T, uint16_t>::value) {
              auto nextRow = reinterpret_cast<const uint16_t*>(reinterpret_cast<const uint8_t *>(mSrc) + srcStride);
              VU R16, G16, B16, A16;
              LoadRGBA<PixelType>(du16, reinterpret_cast<const uint16_t *>(nextRow), R16, G16, B16, A16);

              vRh = ConvertTo(df, PromoteUpperTo(du32, R16));
              vRl = ConvertTo(df, PromoteLowerTo(du32, R16));
              vBh = ConvertTo(df, PromoteUpperTo(du32, B16));
              vBl = ConvertTo(df, PromoteLowerTo(du32, B16));
            } else if (std::is_same<T, uint8_t>::value) {
              const Rebind<uint8_t, decltype(du16)> du8;
              VU R16, G16, B16, A16;
              auto nextRow = reinterpret_cast<const uint8_t*>(reinterpret_cast<const uint8_t *>(mSrc) + srcStride);
              LoadRGB<PixelType>(du16, reinterpret_cast<const uint8_t *>(nextRow), R16, G16, B16);

              vRh = ConvertTo(df, PromoteTo(du32, UpperHalf(dhu16, R16)));
              vRl = ConvertTo(df, PromoteLowerTo(du32, R16));
              vBh = ConvertTo(df, PromoteTo(du32, UpperHalf(dhu16, B16)));
              vBl = ConvertTo(df, PromoteLowerTo(du32, B16));
            }

            vRh = Mul(vRh, vLinearScale);
            vRl = Mul(vRl, vLinearScale);
            vBh = Mul(vBh, vLinearScale);
            vBl = Mul(vBl, vLinearScale);

            Rh = Mul(Add(Rh, vRh), v420Scale);
            Rl = Mul(Add(Rl, vRl), v420Scale);
            Bh = Mul(Add(Bh, vBh), v420Scale);
            Bl = Mul(Add(Bl, vBl), v420Scale);
          }
        }

        if (chromaSubsample == YUV_SAMPLE_422 || (!(y & 1))) {
          const Half<decltype(df)> dfh2;
          const auto Ebh = computeYcCbcCrcEquation(df, Sub(Bh, Eh), vNb, vPb, vEpbLow, vEpbHigh);
          const auto Ebl = computeYcCbcCrcEquation(df, Sub(Bl, El), vNb, vPb, vEpbLow, vEpbHigh);
          const auto Erh = computeYcCbcCrcEquation(df, Sub(Rh, Eh), vNr, vPr, vEprLow, vEprHigh);
          const auto Erl = computeYcCbcCrcEquation(df, Sub(Rl, El), vNr, vPr, vEprLow, vEprHigh);

          const auto bh64 = SumsOf2(Ebh);
          const auto bl64 = SumsOf2(Ebl);
          const auto Eb = Mul(Combine(df, DemoteTo(dfh2, bh64), DemoteTo(dfh2, bl64)), v420Scale);
          const auto rh64 = SumsOf2(Erh);
          const auto rl64 = SumsOf2(Erl);
          const auto Er = Mul(Combine(df, DemoteTo(dfh2, rh64), DemoteTo(dfh2, rl64)), v420Scale);

          auto CbF32 = Add(Round(Mul(Eb, vScaleRangeUV)), vBiasUV);
          auto CrF32 = Add(Round(Mul(Er, vScaleRangeUV)), vBiasUV);

          if (std::is_same<T, uint16_t>::value) {
            const auto Cb = DemoteTo(dhu16, ConvertTo(du32, CbF32));
            StoreU(Cb, dhu16, reinterpret_cast<uint16_t *>(uDst));
            const auto Cr = DemoteTo(dhu16, ConvertTo(du32, CrF32));
            StoreU(Cr, dhu16, reinterpret_cast<uint16_t *>(vDst));
          } else if (std::is_same<T, uint8_t>::value) {
            const Rebind<uint8_t, decltype(du16)> du8;
            using V8 = Vec<decltype(du8)>;
            const Half<decltype(du8)> dhu8;
            const auto Cb = DemoteTo(dhu8, ConvertTo(du32, CbF32));
            StoreU(Cb, dhu8, reinterpret_cast<uint8_t *>(uDst));
            const auto Cr = DemoteTo(dhu8, ConvertTo(du32, CrF32));
            StoreU(Cr, dhu8, reinterpret_cast<uint8_t *>(vDst));
          } else {
            static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected type");
          }
        }
      } else if (chromaSubsample == YUV_SAMPLE_444) {
        auto Ebh = computeYcCbcCrcEquation(df, Sub(Bh, Eh), vNb, vPb, vEpbLow, vEpbHigh);
        auto Ebl = computeYcCbcCrcEquation(df, Sub(Bl, El), vNb, vPb, vEpbLow, vEpbHigh);
        auto Erh = computeYcCbcCrcEquation(df, Sub(Rh, Eh), vNr, vPr, vEprLow, vEprHigh);
        auto Erl = computeYcCbcCrcEquation(df, Sub(Rl, El), vNr, vPr, vEprLow, vEprHigh);

        auto Cbh = Add(Round(Mul(Ebh, vScaleRangeUV)), vBiasUV);
        auto Cbl = Add(Round(Mul(Ebl, vScaleRangeUV)), vBiasUV);
        auto Crh = Add(Round(Mul(Erh, vScaleRangeUV)), vBiasUV);
        auto Crl = Add(Round(Mul(Erl, vScaleRangeUV)), vBiasUV);

        if (std::is_same<T, uint16_t>::value) {
          const auto Cb = Combine(du16, DemoteTo(dhu16, ConvertTo(du32, Cbh)),
                                  DemoteTo(dhu16, ConvertTo(du32, Cbl)));
          StoreU(Cb, du16, reinterpret_cast<uint16_t *>(uDst));
          const auto Cr = Combine(du16, DemoteTo(dhu16, ConvertTo(du32, Crh)), DemoteTo(dhu16, ConvertTo(du32, Crl)));
          StoreU(Cr, du16, reinterpret_cast<uint16_t *>(vDst));
        } else if (std::is_same<T, uint8_t>::value) {
          const Rebind<uint8_t, decltype(du16)> du8;
          using V8 = Vec<decltype(du8)>;
          const Half<decltype(du8)> dhu8;
          const auto Cb = Combine(du8, DemoteTo(dhu8, ConvertTo(du32, Cbh)),
                                  DemoteTo(dhu8, ConvertTo(du32, Cbl)));
          StoreU(Cb, du8, reinterpret_cast<uint8_t *>(uDst));
          const auto Cr = Combine(du8, DemoteTo(dhu8, ConvertTo(du32, Crh)),
                                  DemoteTo(dhu8, ConvertTo(du32, Crl)));
          StoreU(Cr, du8, reinterpret_cast<uint8_t *>(vDst));
        } else {
          static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected type");
        }
      } else {
        static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                          || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
      }

      yDst += lanes;
      uDst += uvLanes;
      vDst += uvLanes;
      mSrc += lanes*components;
    }

    for (; x < width; x += lanesForward) {
      float r;
      float g;
      float b;

      LoadRGB<T, float, PixelType>(mSrc, r, g, b);

      r *= linearScale;
      g *= linearScale;
      b *= linearScale;

      const float dE = Eotf(r, TransferFunction) * kr + Eotf(g, TransferFunction) * kg + Eotf(b, TransferFunction) * kb;
      const float E = Oetf(dE, TransferFunction);

      T Yc = static_cast<T>(static_cast<SignedT>(std::roundf(E * static_cast<float>(rangeY))) + biasY);
      yDst[0] = Yc;
      yDst += 1;

      if (chromaSubsample == YUV_SAMPLE_422) {
        mSrc += components;
        if (x + 1 < width) {
          float r1 = r, g1 = g, b1 = b;

          LoadRGB<T, float, PixelType>(mSrc, r1, g1, b1);

          r1 *= linearScale;
          g1 *= linearScale;
          b1 *= linearScale;

          r = (r + r1) * divisor2;
          g = (g + g1) * divisor2;
          b = (b + b1) * divisor2;

          const float
              dE1 = Eotf(r1, TransferFunction) * kr + Eotf(g1, TransferFunction) * kg + Eotf(b1, TransferFunction) * kb;
          const float E1 = Oetf(dE1, TransferFunction);

          T Yc1 = static_cast<T>(std::roundf(E1 * static_cast<float>(rangeY))) + biasY;
          yDst[0] = Yc1;
          yDst += 1;
          mSrc += components;
        }
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        float r1 = r, g1 = g, b1 = b;
        float r2 = r, g2 = g, b2 = b;
        float r3 = r, g3 = g, b3 = b;

        auto oldSrc = mSrc;

        mSrc += components;

        if (x + 1 < width) {
          LoadRGB<T, float, PixelType>(mSrc, r1, g1, b1);
          r1 *= linearScale;
          g1 *= linearScale;
          b1 *= linearScale;

          const float dE1 =
              Eotf(r1, TransferFunction) * kr + Eotf(g1, TransferFunction) * kg + Eotf(b1, TransferFunction) * kb;
          const float E1 = Oetf(dE1, TransferFunction);

          T Yc1 = static_cast<T>(std::roundf(E1 * static_cast<float>(rangeY))) + biasY;
          yDst[0] = Yc1;
          yDst += 1;
          mSrc += components;
        }

        if (!(y & 1)) {
          auto nextRow = reinterpret_cast<const T *>(oldSrc);
          if (y + 1 < height - 1) {
            nextRow = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(oldSrc) + srcStride);
          }
          LoadRGB<T, float, PixelType>(nextRow, r2, g2, b2);

          r2 *= linearScale;
          g2 *= linearScale;
          b2 *= linearScale;

          nextRow += components;

          if (x + 1 < width) {
            LoadRGB<T, float, PixelType>(nextRow, r3, g3, b3);

            r3 *= linearScale;
            g3 *= linearScale;
            b3 *= linearScale;
          }

          r = (r + r1 + r2 + r3) * divisor4;
          g = (g + g1 + g2 + g3) * divisor4;
          b = (b + b1 + b2 + b3) * divisor4;
        }
      } else {
        mSrc += components;
      }

      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
        auto Eb = computeYcCbcCrcEquation(b - E, Nb, Pb, EpbLow, EpbHigh);
        auto Er = computeYcCbcCrcEquation(r - E, Nr, Pr, EprLow, EprHigh);

        T Cbc = static_cast<T>(static_cast<SignedT>(std::roundf(Eb * static_cast<float>(rangeUV))) + biasUV);
        T Crc = static_cast<T>(static_cast<SignedT>(std::roundf(Er * static_cast<float>(rangeUV))) + biasUV);

        uDst[0] = Cbc;
        vDst[0] = Crc;
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          auto Eb = computeYcCbcCrcEquation(b - E, Nb, Pb, EpbLow, EpbHigh);
          auto Er = computeYcCbcCrcEquation(r - E, Nr, Pr, EprLow, EprHigh);
          T Cbc = static_cast<T>(static_cast<SignedT>(std::roundf(Eb * static_cast<float>(rangeUV))) + biasUV);
          T Crc = static_cast<T>(static_cast<SignedT>(std::roundf(Er * static_cast<float>(rangeUV))) + biasUV);

          uDst[0] = Cbc;
          vDst[0] = Crc;
        }
      }

      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        uDst += 1;
        vDst += 1;
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

/**
* @brief It will be good to declare a type of transfer function
 * and compile each variant separately however increase of binary size about 400%
 * and compile time abound 1000% sacrifices this benefit
*/
#define PIXEL_TO_YCCBCCRC(T, PixelType, bit, yuvname, chroma) \
void PixelType##bit##To##yuvname##P##bit##HWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const float kr, const float kb,                                              \
                    const SparkYuvColorRange colorRange,                                         \
                    const SparkYuvTransferFunction transferFunction) {                           \
      PixelToYcCbcCrcHWY<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               kr, kb,                           \
                                                               colorRange, transferFunction);\
}

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCCBCCRC(uint8_t, RGBA, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, RGB, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCCBCCRC(uint8_t, RGBA, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, RGB, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCCBCCRC(uint8_t, RGBA, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, RGB, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC(uint8_t, BGRA, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, ABGR, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, ARGB, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, BGR, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, BGRA, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, ABGR, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, ARGB, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, BGR, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, BGRA, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, ABGR, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, ARGB, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, BGR, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef PIXEL_TO_YCCBCCRC

template<typename T>
EOTF_INLINE T YcCbcCrcInverse(T Y, T v, T scaleLow, T scaleHigh, T low, T high) {
  T l = v * scaleLow;
  T h = v * scaleHigh;
  T diffl = l;
  T diffh = h;
  T x;
  if (diffl >= -low && diffl <= 0) {
    x = l + Y;
  } else if (diffh > 0 && diffh <= high) {
    x = h + Y;
  } else {
    x = 0;
  }
  return x;
}

template<class D, class V = Vec<D>>
EOTF_INLINE V YcCbcCrcInverse(D d, V Y, V v, V scaleLow, V scaleHigh, V low, V high) {
  const V l = Mul(v, scaleLow);
  const V h = Mul(v, scaleHigh);
  const V diffl = l;
  const V diffh = h;
  const V zeros = Zero(d);
  auto cv = IfThenElse(And(diffl >= low, diffl <= zeros), Add(l, Y), zeros);
  cv = IfThenElse(And(diffh > zeros, diffh <= high), Add(h, Y), cv);
  return cv;
}

/**
* @brief It will be good to declare a type of transfer function
 * and compile each variant separately however increase of binary size about 400%
 * and compile time abound 1000% sacrifices this benefit
*/
template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth>
void YcCbcCrcToXRGB(T *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                    const uint32_t width, const uint32_t height,
                    const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                    const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                    const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                    const float kr, const float kb, const SparkYuvColorRange colorRange,
                    const SparkYuvTransferFunction transferFunction) {
  static_assert(bitDepth >= 8, "Invalid bit depth");

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

  float Nb, Pb, Nr, Pr;
  computeYcCbcCrcCutoffs(transferFunction, kr, kb, Nb, Pb, Nr, Pr);

  const float EpbLow = (2.f * Nb);
  const float EpbHigh = (2.f * Pb);
  const float EprLow = (2.f * Nr);
  const float EprHigh = (2.f * Pr);

  const float kg = 1.f - kr - kb;

  const float scaleRangeY = 1.f / static_cast<float>(rangeY);
  const float scaleRangeUV = 1.f / static_cast<float>(rangeUV);

  const int lanesForward = (chromaSubsample == YUV_SAMPLE_444) ? 1 : 2;

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;
  const auto fMaxColors = static_cast<float>(maxColors);

  const float ekg = 1.f / kg;

  const ScalableTag<uint16_t> du16;
  const Half<decltype(du16)> dhu16;
  const Rebind<T, decltype(du16)> d;
  const Half<decltype(d)> dh;
  const int lanes = Lanes(d);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh);

  const Rebind<uint32_t, decltype(dhu16)> du32;
#if SPARKYUV_ALLOW_FLOAT16
  const Rebind<hwy::float16_t, decltype(du16)> df;
#else
  const RebindToSigned<decltype(du32)> dfi32;
  const Rebind<hwy::float32_t, decltype(dfi32)> df;
#endif

  using VF = Vec<decltype(df)>;

#if SPARKYUV_ALLOW_FLOAT16
  const auto vScaleRangeY = Set(df, hwy::F16FromF32(scaleRangeY));
  const auto vScaleRangeUV = Set(df, hwy::F16FromF32(scaleRangeUV));
  const auto vBiasY = Set(df, hwy::F16FromF32(biasY));
  const auto vBiasUV = Set(df, hwy::F16FromF32(biasUV));
  const auto vNb = Set(df, hwy::F16FromF32(-Nb));
  const auto vPb = Set(df, hwy::F16FromF32(Pb));
  const auto vNr = Set(df, hwy::F16FromF32(-Nr));
  const auto vPr = Set(df, hwy::F16FromF32(Pr));
  const auto vEpbLow = Set(df, hwy::F16FromF32(EpbLow));
  const auto vEpbHigh = Set(df, hwy::F16FromF32(EpbHigh));
  const auto vEprLow = Set(df, hwy::F16FromF32(EprLow));
  const auto vEprHigh = Set(df, hwy::F16FromF32(EprHigh));
  const auto vMaxColors = Set(df, hwy::F16FromF32(fMaxColors));
  const auto vKr = Set(df, hwy::F16FromF32(kr));
  const auto vKb = Set(df, hwy::F16FromF32(kb));
  const auto vEkg = Set(df, hwy::F16FromF32(ekg));
  const auto vZeros = Zero(df);
  const auto alpha = Set(d, hwy::F16FromF32(maxColors));
#else
  const auto vScaleRangeY = Set(df, scaleRangeY);
  const auto vScaleRangeUV = Set(df, scaleRangeUV);
  const auto vBiasY = Set(df, biasY);
  const auto vBiasUV = Set(df, biasUV);
  const auto vNb = Set(df, -Nb);
  const auto vPb = Set(df, Pb);
  const auto vNr = Set(df, -Nr);
  const auto vPr = Set(df, Pr);
  const auto vEpbLow = Set(df, EpbLow);
  const auto vEpbHigh = Set(df, EpbHigh);
  const auto vEprLow = Set(df, EprLow);
  const auto vEprHigh = Set(df, EprHigh);
  const auto vMaxColors = Set(df, fMaxColors);
  const auto vKr = Set(df, kr);
  const auto vKb = Set(df, kb);
  const auto vEkg = Set(df, ekg);
  const auto vZeros = Zero(df);
  const auto alpha = Set(d, maxColors);
#endif

  const auto A = Set(du16, maxColors);

  for (int y = 0; y < height; ++y) {
    auto CbSource = reinterpret_cast<const T *>(mUSrc);
    auto CrSource = reinterpret_cast<const T *>(mVSrc);
    auto ySrc = reinterpret_cast<const T *>(mYSrc);
    auto store = reinterpret_cast<T *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
#if SPARKYUV_ALLOW_FLOAT16
      VF Y, Cb, Cr;
      if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(d)> tg;
        const auto uY = PromoteTo(du16, LoadU(tg, reinterpret_cast<const uint8_t *>(ySrc)));
        Y = Mul(Sub(ConvertTo(df, uY), vBiasY), vScaleRangeY);

        if (chromaSubsample == YUV_SAMPLE_444) {
          const auto uCb = PromoteTo(du16, LoadU(tg, reinterpret_cast<const uint8_t *>(CbSource)));
          Cb = Mul(Sub(ConvertTo(df, uCb), vBiasUV), vScaleRangeUV);

          const auto uCr = PromoteTo(du16, LoadU(tg, reinterpret_cast<const uint8_t *>(CrSource)));
          Cr = Mul(Sub(ConvertTo(df, uCr), vBiasUV), vScaleRangeUV);
        } else if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
          const Half<decltype(tg)> tgh;
          const auto uCb = PromoteTo(dhu16, LoadU(tgh, reinterpret_cast<const uint8_t *>(CbSource)));
          Cb = Mul(Sub(ConvertTo(df, Combine(du16, uCb, uCb)), vBiasUV), vScaleRangeUV);

          const auto uCr = PromoteTo(dhu16, LoadU(tgh, reinterpret_cast<const uint8_t *>(CrSource)));
          Cr = Mul(Sub(ConvertTo(df, Combine(du16, uCr, uCr)), vBiasUV), vScaleRangeUV);

          Cr = InterleaveLower(df, Cr, Cr);
          Cb = InterleaveLower(df, Cb, Cb);
        } else {
          static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                            || chromaSubsample == YUV_SAMPLE_444, "Unexpected type");
        }
      } else if (std::is_same<T, uint16_t>::value) {
        const auto uY = LoadU(du16, reinterpret_cast<const uint16_t *>(ySrc));
        Y = Mul(Sub(ConvertTo(df, uY), vBiasY), vScaleRangeY);

        if (chromaSubsample == YUV_SAMPLE_444) {
          const auto uCb = LoadU(du16, reinterpret_cast<const uint16_t *>(CbSource));
          Cb = Mul(Sub(ConvertTo(df, uCb), vBiasUV), vScaleRangeUV);

          const auto uCr = LoadU(du16, reinterpret_cast<const uint16_t *>(CrSource));
          Cr = Mul(Sub(ConvertTo(df, uCr), vBiasUV), vScaleRangeUV);
        } else if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
          const auto uCb = LoadU(dhu16, reinterpret_cast<const uint16_t *>(CbSource));
          Cb = Mul(Sub(ConvertTo(df, Combine(du16, uCb, uCb)), vBiasUV), vScaleRangeUV);

          const auto uCr = LoadU(dhu16, reinterpret_cast<const uint16_t *>(CrSource));
          Cr = Mul(Sub(ConvertTo(df, Combine(du16, uCr, uCr)), vBiasUV), vScaleRangeUV);

          Cr = InterleaveLower(df, Cr, Cr);
          Cb = InterleaveLower(df, Cb, Cb);
        } else {
          static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                            || chromaSubsample == YUV_SAMPLE_444, "Unexpected type");
        }
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected type");
      }

      auto Bf = YcCbcCrcInverse(df, Y, Cb, vEpbLow, vEpbHigh, vNb, vPb);
      auto Rf = YcCbcCrcInverse(df, Y, Cr, vEprLow, vEprHigh, vNr, vPr);

      auto Gf = Oetf(df, Mul(Sub(Eotf(df, Y, transferFunction),
                                 MulAdd(Eotf(df, Rf, transferFunction), vKr,
                                        Mul(Eotf(df, Bf, transferFunction), vKb))), vEkg), transferFunction);

      auto Ru = ConvertTo(du16, Clamp(Mul(Rf, vMaxColors), vZeros, vMaxColors));
      auto Bu = ConvertTo(du16, Clamp(Mul(Bf, vMaxColors), vZeros, vMaxColors));
      auto Gu = ConvertTo(du16, Clamp(Mul(Gf, vMaxColors), vZeros, vMaxColors));

      if (std::is_same<T, uint8_t>::value) {
        StoreRGBA<PixelType>(du16, reinterpret_cast<uint8_t *>(store), Ru, Gu, Bu, A);
      } else if (std::is_same<T, uint16_t>::value) {
        using VU = Vec<decltype(du16)>;
        VU R = Ru, G = Gu, B = Bu;
        StoreRGBA<PixelType>(du16, reinterpret_cast<uint16_t *>(store), R, G, B, A);
      }
#else
      const auto uY = LoadU(d, reinterpret_cast<const T *>(ySrc));
      const auto Yl = Mul(Sub(ConvertTo(df, PromoteLowerTo(du32, uY)), vBiasY), vScaleRangeY);
      const auto Yh = Mul(Sub(ConvertTo(df, PromoteTo(du32, UpperHalf(dh, uY))), vBiasY), vScaleRangeY);

      VF Cbl, Cbh, Crl, Crh;

      if (chromaSubsample == YUV_SAMPLE_444) {
        const auto uCb = LoadU(d, reinterpret_cast<const T *>(CbSource));
        Cbl = Mul(Sub(ConvertTo(df, PromoteLowerTo(du32, uCb)), vBiasUV), vScaleRangeUV);
        Cbh = Mul(Sub(ConvertTo(df, PromoteTo(du32, UpperHalf(dh, uCb))), vBiasUV), vScaleRangeUV);

        const auto uCr = LoadU(d, reinterpret_cast<const T *>(CrSource));
        Crl = Mul(Sub(ConvertTo(df, PromoteLowerTo(du32, uCr)), vBiasUV), vScaleRangeUV);
        Crh = Mul(Sub(ConvertTo(df, PromoteTo(du32, UpperHalf(dh, uCr))), vBiasUV), vScaleRangeUV);
      } else if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        const auto fuCb = LoadU(dh, reinterpret_cast<const T *>(CbSource));
        const auto cuCb = Combine(d, fuCb, fuCb);
        const auto uCb = InterleaveLower(d, cuCb, cuCb);

        Cbl = Mul(Sub(ConvertTo(df, PromoteLowerTo(du32, uCb)), vBiasUV), vScaleRangeUV);
        Cbh = Mul(Sub(ConvertTo(df, PromoteTo(du32, UpperHalf(dh, uCb))), vBiasUV), vScaleRangeUV);

        const auto fuCr = BitCast(dh, LoadU(dh, reinterpret_cast<const T *>(CrSource)));
        const auto cuCr = Combine(d, fuCr, fuCr);
        const auto uCr = InterleaveLower(d, cuCr, cuCr);

        Crl = Mul(Sub(ConvertTo(df, PromoteLowerTo(du32, uCr)), vBiasUV), vScaleRangeUV);
        Crh = Mul(Sub(ConvertTo(df, PromoteTo(du32, UpperHalf(dh, uCr))), vBiasUV), vScaleRangeUV);
      }

      auto Bl = YcCbcCrcInverse(df, Yl, Cbl, vEpbLow, vEpbHigh, vNb, vPb);
      auto Rl = YcCbcCrcInverse(df, Yl, Crl, vEprLow, vEprHigh, vNr, vPr);

      auto Bh = YcCbcCrcInverse(df, Yh, Cbh, vEpbLow, vEpbHigh, vNb, vPb);
      auto Rh = YcCbcCrcInverse(df, Yh, Crh, vEprLow, vEprHigh, vNr, vPr);

      auto Gl = Oetf(df, Mul(Sub(Eotf(df, Yl, transferFunction),
                                 MulAdd(Eotf(df, Rl, transferFunction), vKr,
                                        Mul(Eotf(df, Bl, transferFunction), vKb))), vEkg), transferFunction);
      auto Gh = Oetf(df, Mul(Sub(Eotf(df, Yh, transferFunction),
                                 MulAdd(Eotf(df, Rh, transferFunction), vKr,
                                        Mul(Eotf(df, Bh, transferFunction), vKb))), vEkg), transferFunction);

      Bl = Clamp(Mul(Bl, vMaxColors), vZeros, vMaxColors);
      Rl = Clamp(Mul(Rl, vMaxColors), vZeros, vMaxColors);
      Gl = Clamp(Mul(Gl, vMaxColors), vZeros, vMaxColors);
      Bh = Clamp(Mul(Bh, vMaxColors), vZeros, vMaxColors);
      Rh = Clamp(Mul(Rh, vMaxColors), vZeros, vMaxColors);
      Gh = Clamp(Mul(Gh, vMaxColors), vZeros, vMaxColors);

      const auto R = BitCast(d, Combine(d, DemoteTo(dh, ConvertTo(du32, Rh)), DemoteTo(dh, ConvertTo(du32, Rl))));
      const auto G = BitCast(d, Combine(d, DemoteTo(dh, ConvertTo(du32, Gh)), DemoteTo(dh, ConvertTo(du32, Gl))));
      const auto B = BitCast(d, Combine(d, DemoteTo(dh, ConvertTo(du32, Bh)), DemoteTo(dh, ConvertTo(du32, Bl))));

      StoreRGBA<PixelType>(d, store, R, G, B, alpha);
#endif

      ySrc += lanes;
      CbSource += uvLanes;
      CrSource += uvLanes;
      store += lanes * components;
    }

    for (; x < width; x += lanesForward) {
      const T uValue = reinterpret_cast<const T *>(CbSource)[0];
      const T vValue = reinterpret_cast<const T *>(CrSource)[0];

      auto Y = static_cast<float>(static_cast<int>(ySrc[0]) - biasY) * scaleRangeY;
      const auto Cr = static_cast<float>(static_cast<int>(vValue) - biasUV) * scaleRangeUV;
      const auto Cb = static_cast<float>(static_cast<int>(uValue) - biasUV) * scaleRangeUV;

      auto B = YcCbcCrcInverse<float>(Y, Cb, EpbLow, EpbHigh, Nb, Pb);
      auto R = YcCbcCrcInverse<float>(Y, Cr, EprLow, EprHigh, Nr, Pr);
      auto G = Oetf((Eotf(Y, transferFunction) - kr * Eotf(R, transferFunction) - kb * Eotf(B, transferFunction)) * ekg,
                    transferFunction);

      R = std::clamp(std::roundf(R * fMaxColors), 0.f, fMaxColors);
      B = std::clamp(std::roundf(B * fMaxColors), 0.f, fMaxColors);
      G = std::clamp(std::roundf(G * fMaxColors), 0.f, fMaxColors);

      StoreRGBA<T, float, PixelType>(store, R, G, B, maxColors);
      store += components;
      ySrc += 1;

      if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        if (x + 1 < width) {
          auto Y1 = static_cast<float>(static_cast<int>(ySrc[0]) - biasY) * scaleRangeY;
          auto B1 = YcCbcCrcInverse<float>(Y1, Cb, EpbLow, EpbHigh, Nb, Pb);
          auto R1 = YcCbcCrcInverse<float>(Y1, Cr, EprLow, EprHigh, Nr, Pr);
          auto G1 =
              Oetf((Eotf(Y1, transferFunction) - kr * Eotf(R1, transferFunction) - kb * Eotf(B1, transferFunction))
                       * ekg,
                   transferFunction);

          R1 = std::clamp(std::roundf(R1 * fMaxColors), 0.f, fMaxColors);
          B1 = std::clamp(std::roundf(B1 * fMaxColors), 0.f, fMaxColors);
          G1 = std::clamp(std::roundf(G1 * fMaxColors), 0.f, fMaxColors);

          StoreRGBA<T, float, PixelType>(store, R1, G1, B1, maxColors);
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

#undef SUITABLE_FLOAT_16
}

/**
* @brief It will be good to declare a type of transfer function
 * and compile each variant separately however increase of binary size about 400%
 * and compile time abound 1000% sacrifices this benefit
*/
#define YcCbcCrcToXXXX_DECLARATION_R(T, PixelType, bit, yuvname, chroma) \
void yuvname##P##bit##To##PixelType##bit##HWY(T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const float kr, const float kb,                                              \
                    const sparkyuv::SparkYuvColorRange colorRange,                                         \
                    const sparkyuv::SparkYuvTransferFunction transferFunction) {                           \
      YcCbcCrcToXRGB<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               kr, kb,                           \
                                                               colorRange, transferFunction);  \
}

YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
#endif

YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
#endif

YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YcCbcCrc444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YcCbcCrc422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YcCbcCrc420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef YcCbcCrcToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif