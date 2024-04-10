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

#if defined(SPARKYUV_YCGCO_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YCGCO_INL_H
#undef SPARKYUV_YCGCO_INL_H
#else
#define SPARKYUV_YCGCO_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include "Eotf-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth>
std::enable_if_t<std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, void>
PixelToYcCbcCrcHWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                   const uint32_t width, const uint32_t height,
                   T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                   T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                   T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                   const SparkYuvColorRange colorRange) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  uint16_t bY;
  uint16_t bUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, bY, bUV, rangeY, rangeUV);

  const int precision = 6;
  const float maxColors = std::powf(2.f, bitDepth) - 1.f;
  const int rangeReduction = static_cast<int>(std::roundf((static_cast<float>(rangeY) / static_cast<float>(maxColors)
      * static_cast<float>(1 << precision))));
  const int biasY = static_cast<int>(bY) * (1 << precision);
  const int biasUV = static_cast<int>(bUV) * (1 << precision);

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);
  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const ScalableTag<uint16_t> du16;
  const RebindToSigned<decltype(du16)> di16;
  using VU16 = Vec<decltype(du16)>;
  const Rebind<T, decltype(du16)> d;
  const Half<decltype(di16)> dhi16;
  const Half<decltype(d)> dh;
  const Half<decltype(du16)> dhu16;
  const auto vBiasY = Set(du16, biasY);
  const auto vBiasUV = Set(di16, biasUV);
  const auto vhBiasUV = Set(dhi16, biasUV);
  const auto vRangeReduction = Set(du16, rangeReduction);
  const Rebind<int32_t, decltype(dhu16)> di32;
  const RebindToUnsigned<decltype(di32)> d32;
  const auto viRangeReduction = Set(di32, rangeReduction);
  const auto viBiasY = Set(di32, biasY);
  const auto viBiasUV = Set(di32, biasUV);

  const int lanes = Lanes(d);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh);

  const int lanesForward = (chromaSubsample == YUV_SAMPLE_444) ? 1 : 2;

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<T *>(yStore);
    auto uDst = reinterpret_cast<T *>(uStore);
    auto vDst = reinterpret_cast<T *>(vStore);
    auto mSrc = reinterpret_cast<const T *>(mSource);

    for (; x + lanes < width; x += lanes) {
      VU16 R, G, B;
      if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(du16)> d8;
        LoadRGBTo16<PixelType>(d8, reinterpret_cast<const uint8_t *>(mSrc), R, G, B);
      } else if (std::is_same<T, uint16_t>::value) {
        LoadRGBTo16<PixelType>(du16, reinterpret_cast<const uint16_t *>(mSrc), R, G, B);
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
      }

      if (chromaSubsample == YUV_SAMPLE_444) {
        VU16 Y, Cg, Co;

        if (bitDepth <= 8) {
          R = Mul(R, vRangeReduction);
          G = Mul(G, vRangeReduction);
          B = Mul(B, vRangeReduction);
          const auto hG = ShiftRight<1>(G);
          const auto qRB = ShiftRight<2>(Add(R, B));
          Y = ShiftRight<6>(Add(Add(hG, qRB), vBiasY));
          const auto shG = BitCast(di16, hG);
          const auto sR = BitCast(di16, R);
          const auto sB = BitCast(di16, B);
          Cg = BitCast(du16, ShiftRight<6>(Add(vBiasUV, Sub(shG, BitCast(di16, qRB)))));
          Co = BitCast(du16, ShiftRight<6>(Add(ShiftRight<1>(Sub(sR, sB)), vBiasUV)));
        } else {
          auto Rl = PromoteLowerTo(di32, BitCast(di16, R));
          auto Rh = PromoteUpperTo(di32, BitCast(di16, R));
          auto Gl = PromoteLowerTo(di32, BitCast(di16, G));
          auto Gh = PromoteUpperTo(di32, BitCast(di16, G));
          auto Bl = PromoteLowerTo(di32, BitCast(di16, B));
          auto Bh = PromoteUpperTo(di32, BitCast(di16, B));

          Rl = Mul(Rl, viRangeReduction);
          Gl = Mul(Gl, viRangeReduction);
          Bl = Mul(Bl, viRangeReduction);
          Rh = Mul(Rh, viRangeReduction);
          Gh = Mul(Gh, viRangeReduction);
          Bh = Mul(Bh, viRangeReduction);

          const auto Yl = ShiftRightNarrow<6>(di32, Add(AddAndHalf(di32, Gl, AddAndHalf(di32, Rl, Bl)), viBiasY));
          const auto Yh = ShiftRightNarrow<6>(di32, Add(AddAndHalf(di32, Gh, AddAndHalf(di32, Rh, Bh)), viBiasY));
          Y = BitCast(du16, Combine(di16, Yh, Yl));
          const auto Cgl = ShiftRightNarrow<6>(di32, Add(viBiasUV,
                                                         Sub(ShiftRight<1>(Gl),
                                                             ShiftRight<2>(Add(Rl, Bl)))));
          const auto Cgh = ShiftRightNarrow<6>(di32, Add(viBiasUV,
                                                         Sub(ShiftRight<1>(Gh),
                                                             ShiftRight<2>(Add(Rh, Bh)))));
          Cg = BitCast(du16, Combine(di16, Cgh, Cgl));
          const auto Col = ShiftRightNarrow<6>(di32, Add(ShiftRight<1>(Sub(Rl, Bl)), viBiasUV));
          const auto Coh = ShiftRightNarrow<6>(di32, Add(ShiftRight<1>(Sub(Rh, Bh)), viBiasUV));
          Co = BitCast(du16, Combine(di16, Coh, Col));
        }

        if (std::is_same<T, uint16_t>::value) {
          StoreU(Y, du16, reinterpret_cast<uint16_t *>(yDst));
          StoreU(Cg, du16, reinterpret_cast<uint16_t *>(uDst));
          StoreU(Co, du16, reinterpret_cast<uint16_t *>(vDst));
        } else if (std::is_same<T, uint8_t>::value) {
          const Rebind<uint8_t, decltype(du16)> du8;
          StoreU(DemoteTo(du8, Y), du8, reinterpret_cast<uint8_t *>(yDst));
          StoreU(DemoteTo(du8, Cg), du8, reinterpret_cast<uint8_t *>(uDst));
          StoreU(DemoteTo(du8, Co), du8, reinterpret_cast<uint8_t *>(vDst));
        } else {
          static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
        }
      } else if (chromaSubsample == YUV_SAMPLE_420 || chromaSubsample == YUV_SAMPLE_422) {
        using VHU16 = Vec<decltype(dhu16)>;
        VU16 Y;
        VHU16 Cg, Co;

        // uint16_t 12 bit max colors 4096 then 4096*2 < 2^16 so overflowing is not possible in uint16_t domain
        // Bi-linear sample
        if (bitDepth <= 8) {
          R = Mul(R, vRangeReduction);
          G = Mul(G, vRangeReduction);
          B = Mul(B, vRangeReduction);

          Y = ShiftRight<6>(Add(AddAndHalf(du16, G, AddAndHalf(du16, R, B)), vBiasY));
          const auto sR = BitCast(dhi16, ShiftRightNarrow<1>(d32, SumsOf2(R)));
          const auto sG = BitCast(dhi16, ShiftRightNarrow<1>(d32, SumsOf2(G)));
          const auto sB = BitCast(dhi16, ShiftRightNarrow<1>(d32, SumsOf2(B)));

          Cg = BitCast(dhu16, ShiftRight<6>(Add(vhBiasUV,
                                                Sub(ShiftRight<1>(sG), BitCast(dhi16, ShiftRight<2>(Add(sR, sB)))))));
          Co = BitCast(dhu16, ShiftRight<6>(Add(ShiftRight<1>(Sub(sR, sB)), vhBiasUV)));
        } else {
          auto Rl = PromoteLowerTo(di32, BitCast(di16, R));
          auto Rh = PromoteUpperTo(di32, BitCast(di16, R));
          auto Gl = PromoteLowerTo(di32, BitCast(di16, G));
          auto Gh = PromoteUpperTo(di32, BitCast(di16, G));
          auto Bl = PromoteLowerTo(di32, BitCast(di16, B));
          auto Bh = PromoteUpperTo(di32, BitCast(di16, B));

          Rl = Mul(Rl, viRangeReduction);
          Gl = Mul(Gl, viRangeReduction);
          Bl = Mul(Bl, viRangeReduction);
          Rh = Mul(Rh, viRangeReduction);
          Gh = Mul(Gh, viRangeReduction);
          Bh = Mul(Bh, viRangeReduction);

          const auto Yl = ShiftRightNarrow<6>(di32, Add(Add(ShiftRight<1>(Gl), ShiftRight<2>(Add(Rl, Bl))), viBiasY));
          const auto Yh = ShiftRightNarrow<6>(di32, Add(Add(ShiftRight<1>(Gh), ShiftRight<2>(Add(Rh, Bh))), viBiasY));
          Y = BitCast(du16, Combine(di16, Yh, Yl));
          const auto Cgl = ShiftRightNarrow<6>(di32, Add(viBiasUV,
                                                         Sub(ShiftRight<1>(Gl),
                                                             ShiftRight<2>(Add(Rl, Bl)))));
          const auto Cgh = ShiftRightNarrow<6>(di32, Add(viBiasUV,
                                                         Sub(ShiftRight<1>(Gh),
                                                             ShiftRight<2>(Add(Rh, Bh)))));
          Cg = BitCast(dhu16, ShiftRight<1>(Add(Cgh, Cgl)));
          const auto Col = ShiftRightNarrow<6>(di32, Add(ShiftRight<1>(Sub(Rl, Bl)), viBiasUV));
          const auto Coh = ShiftRightNarrow<6>(di32, Add(ShiftRight<1>(Sub(Rh, Bh)), viBiasUV));
          Co = BitCast(dhu16, ShiftRight<1>(Add(Coh, Col)));
        }

        if (std::is_same<T, uint16_t>::value) {
          StoreU(Y, du16, reinterpret_cast<uint16_t *>(yDst));
          if (chromaSubsample == YUV_SAMPLE_422 || (!(y & 1))) {
            StoreU(Cg, dhu16, reinterpret_cast<uint16_t *>(uDst));
            StoreU(Co, dhu16, reinterpret_cast<uint16_t *>(vDst));
          }
        } else if (std::is_same<T, uint8_t>::value) {
          const Rebind<uint8_t, decltype(du16)> du8;
          const Rebind<uint8_t, decltype(dhu16)> dhu8;
          StoreU(DemoteTo(du8, Y), du8, reinterpret_cast<uint8_t *>(yDst));
          if (chromaSubsample == YUV_SAMPLE_422 || (!(y & 1))) {
            StoreU(DemoteTo(dhu8, Cg), dhu8, reinterpret_cast<uint8_t *>(uDst));
            StoreU(DemoteTo(dhu8, Co), dhu8, reinterpret_cast<uint8_t *>(vDst));
          }
        } else {
          static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
        }
      } else {
        static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                          || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
      }

      mSrc += lanes * components;
      yDst += lanes;
      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
        uDst += uvLanes;
        vDst += uvLanes;
      } else {
        if (!(y & 1)) {
          uDst += uvLanes;
          vDst += uvLanes;
        }
      }
    }

    for (; x < width; x += lanesForward) {
      int r;
      int g;
      int b;

      LoadRGB<T, int, PixelType>(mSrc, r, g, b);

      r *= rangeReduction;
      g *= rangeReduction;
      b *= rangeReduction;

      int hg = g >> 1;

      int Y = static_cast<int>(hg + ((r + b) >> 2) + biasY) >> precision;
      yDst[0] = Y;
      yDst += 1;

      if (chromaSubsample == YUV_SAMPLE_444) {
        mSrc += components;
      } else if (chromaSubsample == YUV_SAMPLE_422) {
        mSrc += components;
        if (x + 1 < width) {
          int r1 = r, g1 = g, b1 = b;
          LoadRGB<T, int, PixelType>(mSrc, r1, g1, b1);

          r1 *= rangeReduction;
          g1 *= rangeReduction;
          b1 *= rangeReduction;

          r = (r + r1) >> 1;
          g = (g + g1) >> 1;
          b = (b + b1) >> 1;

          T Y1 = static_cast<T>(static_cast<int>((g1 >> 1) + ((r1 + b1) >> 2) + biasY) >> precision);
          yDst[0] = Y1;
          yDst += 1;
          mSrc += components;
        }
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        auto oldSrc = mSrc;

        mSrc += components;

        int r1 = r, g1 = g, b1 = b;
        int r2 = r, g2 = g, b2 = b;
        int r3 = r, g3 = g, b3 = b;

        if (x + 1 < width) {
          LoadRGB<T, int, PixelType>(mSrc, r1, g1, b1);

          r1 *= rangeReduction;
          g1 *= rangeReduction;
          b1 *= rangeReduction;

          T Y1 = static_cast<T>(static_cast<int>((g1 >> 1) + ((r1 + b1) >> 2) + biasY) >> precision);
          yDst[0] = Y1;
          yDst += 1;
          mSrc += components;
        }

        if (!(y & 1)) {
          auto nextRow = reinterpret_cast<const T *>(oldSrc);
          if (y + 1 < height - 1) {
            nextRow = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(oldSrc) + srcStride);
          }

          LoadRGB<T, int, PixelType>(nextRow, r2, g2, b2);

          r2 *= rangeReduction;
          g2 *= rangeReduction;
          b2 *= rangeReduction;

          nextRow += components;

          if (x + 1 < width) {
            LoadRGB<T, int, PixelType>(nextRow, r3, g3, b3);

            r3 *= rangeReduction;
            g3 *= rangeReduction;
            b3 *= rangeReduction;
          }

          r = (r + r1 + r2 + r3) >> 2;
          g = (g + g1 + g2 + g3) >> 2;
          b = (b + b1 + b2 + b3) >> 2;
        }
      }

      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
        T Cb = static_cast<T>((((g >> 1) - ((r + b) >> 2)) + biasUV) >> precision);
        T Cr = static_cast<T>((((r - b) >> 1) + biasUV) >> precision);

        uDst[0] = Cb;
        vDst[0] = Cr;
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          T Cb = static_cast<T>((((g >> 1) - ((r + b) >> 2)) + biasUV) >> precision);
          T Cr = static_cast<T>((((r - b) >> 1) + biasUV) >> precision);

          uDst[0] = Cb;
          vDst[0] = Cr;
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

#define PIXEL_TO_YCGCO(T, PixelType, bit, yuvname, chroma) \
void PixelType##bit##To##yuvname##P##bit##HWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const SparkYuvColorRange colorRange) {                           \
      PixelToYcCbcCrcHWY<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               colorRange);\
}

PIXEL_TO_YCGCO(uint16_t, RGBA, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, RGB, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YCGCO(uint16_t, RGBA, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, RGB, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YCGCO(uint16_t, RGBA, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, RGB, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCGCO(uint16_t, RGBA, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, RGB, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCGCO(uint16_t, RGBA, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, RGB, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCGCO(uint16_t, RGBA, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, RGB, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCGCO(uint8_t, RGBA, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint8_t, RGB, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCGCO(uint8_t, RGBA, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint8_t, RGB, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCGCO(uint8_t, RGBA, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint8_t, RGB, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO(uint16_t, BGRA, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, ABGR, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, ARGB, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, BGR, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, BGRA, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, ABGR, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, ARGB, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, BGR, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, BGRA, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, ABGR, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, ARGB, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, BGR, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO(uint8_t, BGRA, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint8_t, ABGR, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint8_t, ARGB, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint8_t, BGR, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint8_t, BGRA, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint8_t, ABGR, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint8_t, ARGB, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint8_t, BGR, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint8_t, BGRA, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint8_t, ABGR, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint8_t, ARGB, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint8_t, BGR, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCGCO(uint16_t, BGRA, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, ABGR, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, ARGB, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, BGR, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCGCO(uint16_t, BGRA, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, ABGR, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, ARGB, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, BGR, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCGCO(uint16_t, BGRA, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, ABGR, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, ARGB, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCGCO(uint16_t, BGR, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef PIXEL_TO_YCGCO

template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth>
void YcCbcCrcToXRGB(T *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                    const uint32_t width, const uint32_t height,
                    const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                    const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                    const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                    const SparkYuvColorRange colorRange) {
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
  const int precision = 6;
  const int rangeReduction = static_cast<int>(std::roundf((static_cast<float>(maxColors) / static_cast<float>(rangeY)
      * static_cast<float>(1 << precision))));

  const int lanesForward = (chromaSubsample == YUV_SAMPLE_444) ? 1 : 2;

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const ScalableTag<uint16_t> du16;
  const RebindToSigned<decltype(du16)> di16;
  const Half<decltype(di16)> dhi16;
  using VU16 = Vec<decltype(du16)>;
  const Rebind<T, decltype(du16)> d;
  const Half<decltype(d)> dh;
  const Half<decltype(du16)> dhu16;
  const auto vBiasY = Set(di16, biasY);
  const auto vBiasUV = Set(di16, biasUV);
  const auto vMaxColors = Set(di16, maxColors);
  const auto viZeros = Zero(di16);
  const auto A16 = Set(du16, maxColors);
  const Rebind<uint8_t, decltype(du16)> d8;
  const auto vRangeReduction = Set(di16, rangeReduction);
  const Rebind<int32_t, decltype(dhu16)> di32;
  const auto viRangeReduction = Set(di32, rangeReduction);

  const int lanes = Lanes(d);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh);

  for (int y = 0; y < height; ++y) {
    auto CgSource = reinterpret_cast<const T *>(mUSrc);
    auto CoSource = reinterpret_cast<const T *>(mVSrc);
    auto ySrc = reinterpret_cast<const T *>(mYSrc);
    auto store = reinterpret_cast<T *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      VU16 Y, Cg, Co;

      if (std::is_same<T, uint16_t>::value) {
        Y = LoadU(du16, reinterpret_cast<const uint16_t *>(ySrc));
        if (chromaSubsample == YUV_SAMPLE_444) {
          Cg = LoadU(du16, reinterpret_cast<const uint16_t *>(CgSource));
          Co = LoadU(du16, reinterpret_cast<const uint16_t *>(CoSource));
        } else if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
          const auto hCg = LoadU(dhu16, reinterpret_cast<const uint16_t *>(CgSource));
          Cg = Combine(du16, hCg, hCg);
          Cg = InterleaveLower(Cg, Cg);
          const auto hCo = LoadU(dhu16, reinterpret_cast<const uint16_t *>(CoSource));
          Co = Combine(du16, hCo, hCo);
          Co = InterleaveLower(Co, Co);
        } else {
          static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                            || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
        }
      } else if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(du16)> du8;
        Y = PromoteTo(du16, LoadU(du8, reinterpret_cast<const uint8_t *>(ySrc)));
        if (chromaSubsample == YUV_SAMPLE_444) {
          Cg = PromoteTo(du16, LoadU(du8, reinterpret_cast<const uint8_t *>(CgSource)));
          Co = PromoteTo(du16, LoadU(du8, reinterpret_cast<const uint8_t *>(CoSource)));
        } else if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
          const Half<decltype(du8)> dhu8;
          const auto hCg = LoadU(dhu8, reinterpret_cast<const uint8_t *>(CgSource));
          Cg = PromoteTo(du16, Combine(du8, hCg, hCg));
          Cg = InterleaveLower(Cg, Cg);
          const auto hCo = LoadU(dhu8, reinterpret_cast<const uint8_t *>(CoSource));
          Co = PromoteTo(du16, Combine(du8, hCo, hCo));
          Co = InterleaveLower(Co, Co);
        } else {
          static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                            || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
        }
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
      }

      VU16 R16, G16, B16;

      if (bitDepth <= 8) {
        const auto sY = Mul(Sub(BitCast(di16, Y), vBiasY), vRangeReduction);
        const auto sCg = Mul(Sub(BitCast(di16, Cg), vBiasUV), vRangeReduction);
        const auto sCo = Mul(Sub(BitCast(di16, Co), vBiasUV), vRangeReduction);
        const auto t = Sub(sY, sCg);
        G16 = BitCast(du16, Clamp(ShiftRight<6>(Add(sY, sCg)), viZeros, vMaxColors));
        B16 = BitCast(du16, Clamp(ShiftRight<6>(Sub(t, sCo)), viZeros, vMaxColors));
        R16 = BitCast(du16, Clamp(ShiftRight<6>(Add(t, sCo)), viZeros, vMaxColors));
      } else {
        const auto sY = Sub(BitCast(di16, Y), vBiasY);
        const auto sCg = Sub(BitCast(di16, Cg), vBiasUV);
        const auto sCo = Sub(BitCast(di16, Co), vBiasUV);
        const auto sYl = Mul(PromoteLowerTo(di32, sY), viRangeReduction);
        const auto sYh = Mul(PromoteUpperTo(di32, sY), viRangeReduction);
        const auto sCgl = Mul(PromoteLowerTo(di32, sCg), viRangeReduction);
        const auto sCgh = Mul(PromoteUpperTo(di32, sCg), viRangeReduction);
        const auto sCol = Mul(PromoteLowerTo(di32, sCo), viRangeReduction);
        const auto sCoh = Mul(PromoteUpperTo(di32, sCo), viRangeReduction);

        const auto tl = Sub(sYl, sCgl);
        const auto th = Sub(sYh, sCgh);

        const auto Gl = ShiftRightNarrow<6>(di32, Add(sYl, sCgl));
        const auto Gh = ShiftRightNarrow<6>(di32, Add(sYh, sCgh));
        const auto Bl = ShiftRightNarrow<6>(di32, Sub(tl, sCol));
        const auto Bh = ShiftRightNarrow<6>(di32, Sub(th, sCoh));
        const auto Rl = ShiftRightNarrow<6>(di32, Add(tl, sCol));
        const auto Rh = ShiftRightNarrow<6>(di32, Add(th, sCoh));

        G16 = BitCast(du16, Clamp(Combine(di16, Gh, Gl), viZeros, vMaxColors));
        B16 = BitCast(du16, Clamp(Combine(di16, Bh, Bl), viZeros, vMaxColors));
        R16 = BitCast(du16, Clamp(Combine(di16, Rh, Rl), viZeros, vMaxColors));
      }

      if (std::is_same<T, uint16_t>::value) {
        StoreRGBA<PixelType>(du16, reinterpret_cast<uint16_t *>(store), R16, G16, B16, A16);
      } else if (std::is_same<T, uint8_t>::value) {
        StoreRGBA<PixelType>(du16, reinterpret_cast<uint8_t *>(store), R16, G16, B16, A16);
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
      }

      CgSource += uvLanes;
      CoSource += uvLanes;
      ySrc += lanes;
      store += lanes * components;
    }

    for (; x < width; x += lanesForward) {
      const T uValue = reinterpret_cast<const T *>(CgSource)[0];
      const T vValue = reinterpret_cast<const T *>(CoSource)[0];

      const int Y = (static_cast<int>(ySrc[0]) - biasY) * rangeReduction;
      const int Cr = (static_cast<int>(vValue) - biasUV) * rangeReduction;
      const int Cb = (static_cast<int>(uValue) - biasUV) * rangeReduction;

      int t = Y - Cb;

      int G = std::clamp((Y + Cb) >> precision, 0, maxColors);
      int B = std::clamp((t - Cr) >> precision, 0, maxColors);
      int R = std::clamp((t + Cr) >> precision, 0, maxColors);

      StoreRGBA<T, int, PixelType>(store, R, G, B, maxColors);

      store += components;
      ySrc += 1;

      if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        if (x + 1 < width) {
          int Y1 = (static_cast<int>(ySrc[0]) - biasY) * rangeReduction;
          int t1 = Y1 - Cb;
          int G1 = std::clamp((Y1 + Cb) >> precision, 0, maxColors);
          int B1 = std::clamp((t1 - Cr) >> precision, 0, maxColors);
          int R1 = std::clamp((t + Cr) >> precision, 0, maxColors);

          StoreRGBA<T, int, PixelType>(store, R1, G1, B1, maxColors);
          store += components;
          ySrc += 1;
        }
      }

      CgSource += 1;
      CoSource += 1;
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

#define YCGCO_ToXXXX_DECLARATION_R(T, PixelType, bit, yuvname, chroma) \
void yuvname##P##bit##To##PixelType##bit##HWY(T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const sparkyuv::SparkYuvColorRange colorRange) {                           \
      YcCbcCrcToXRGB<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               colorRange);  \
}

YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGB, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGB, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGB, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGR, 10, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGR, 10, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGR, 10, YCgCo420, sparkyuv::YUV_SAMPLE_420)
#endif

YCGCO_ToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, RGB, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, RGB, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, RGB, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YCGCO_ToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, BGR, 8, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, BGR, 8, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint8_t, BGR, 8, YCgCo420, sparkyuv::YUV_SAMPLE_420)
#endif

YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGB, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGB, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, RGB, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGR, 12, YCgCo444, sparkyuv::YUV_SAMPLE_444)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGR, 12, YCgCo422, sparkyuv::YUV_SAMPLE_422)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
YCGCO_ToXXXX_DECLARATION_R(uint16_t, BGR, 12, YCgCo420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef YCGCO_ToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif