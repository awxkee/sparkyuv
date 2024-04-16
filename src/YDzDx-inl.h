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

#if defined(SPARKYUV_YDZDX_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YDZDX_INL_H
#undef SPARKYUV_YDZDX_INL_H
#else
#define SPARKYUV_YDZDX_INL_H
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
PixelToYDzDxHWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                const uint32_t width, const uint32_t height,
                T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                const SparkYuvColorRange colorRange) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                    || chromaSubsample == YUV_SAMPLE_444, "Unexpected type");
  uint16_t bY;
  uint16_t bUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(colorRange, bitDepth, bY, bUV, rangeY, rangeUV);

  const int precision = 8;
  const float maxColors = ::powf(2.f, bitDepth) - 1.f;
  const float frangeReductionY = ::roundf((static_cast<float>(rangeY) / static_cast<float>(maxColors)
      * static_cast<float>(1 << precision)));
  const float frangeReductionUV = ::roundf((static_cast<float>(rangeUV) / static_cast<float>(maxColors)
      * static_cast<float>(1 << precision)));
  const int rangeReductionY = static_cast<int>(frangeReductionY);
  const int rangeReductionUV = static_cast<int>(frangeReductionUV);

  /**
   *
       * E′Y = E′G		(E66)
        E′PB = ( 0.986566 * E′B − E′Y ) ÷ 2	 	(E67)
        E′PR = ( E′R − 0.991902 * E′Y ) ÷ 2

        E'G = E'Y
        E'R = (E'PR * 2 + E'Y) / 0.991902
        E'B = (E'PB * 2 + E'Y) / 0.986566
       **/

  const int coeffPb1 = static_cast<int>(static_cast<float>(0.986566f / 2.f) * frangeReductionUV);
  const int coeffPb2 = static_cast<int>(static_cast<float>(1.f / 2.f) * frangeReductionUV);
  const int coeffPr1 = static_cast<int>(static_cast<float>(1.f / 2.f) * frangeReductionUV);
  const int coeffPr2 = static_cast<int>(static_cast<float>(0.991902f / 2.f) * frangeReductionUV);

  const auto biasY = static_cast<int>((static_cast<float>(bY) + 0.5f) * static_cast<float>(1 << precision));
  const auto biasUV = static_cast<int>((static_cast<float>(bUV) + 0.5f) * static_cast<float>(1 << precision));

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);
  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const int components = (PixelType == PIXEL_BGR || PixelType == PIXEL_RGB) ? 3 : 4;

  const int lanesForward = getYuvChromaPixels(chromaSubsample);

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<T *>(yStore);
    auto uDst = reinterpret_cast<T *>(uStore);
    auto vDst = reinterpret_cast<T *>(vStore);
    auto mSrc = reinterpret_cast<const T *>(mSource);

    for (; x < width; x += lanesForward) {
      int r;
      int g;
      int b;

      LoadRGB<T, int, PixelType>(mSrc, r, g, b);

      int Y = static_cast<int>(g * rangeReductionY + biasY) >> precision;
      yDst[0] = Y;
      yDst += 1;

      if (chromaSubsample == YUV_SAMPLE_444) {
        mSrc += components;
      } else if (chromaSubsample == YUV_SAMPLE_422) {
        mSrc += components;
        if (x + 1 < width) {
          int r1 = r, g1 = g, b1 = b;
          LoadRGB<T, int, PixelType>(mSrc, r1, g1, b1);

          r = (r + r1) >> 1;
          g = (g + g1) >> 1;
          b = (b + b1) >> 1;

          T Y1 = (g1 * rangeReductionY + biasY) >> precision;

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

          T Y1 = (g1 * rangeReductionY + biasY) >> precision;

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

          nextRow += components;

          if (x + 1 < width) {
            LoadRGB<T, int, PixelType>(nextRow, r3, g3, b3);
          }

          r = (r + r1 + r2 + r3) >> 2;
          g = (g + g1 + g2 + g3) >> 2;
          b = (b + b1 + b2 + b3) >> 2;
        }
      }

      if (chromaSubsample == YUV_SAMPLE_444 || chromaSubsample == YUV_SAMPLE_422) {
        T Dz = static_cast<T>((coeffPb1 * b - coeffPb2 * g + biasUV) >> precision);
        T Dx = static_cast<T>((coeffPr1 * r - coeffPr2 * g + biasUV) >> precision);

        uDst[0] = Dz;
        vDst[0] = Dx;
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          T Dz = static_cast<T>((coeffPb1 * b - coeffPb2 * g + biasUV) >> precision);
          T Dx = static_cast<T>((coeffPr1 * r - coeffPr2 * g + biasUV) >> precision);

          uDst[0] = Dz;
          vDst[0] = Dx;
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

#define PIXEL_TO_YDZDX(T, PixelType, bit, yuvname, chroma) \
void PixelType##bit##To##yuvname##P##bit##HWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const SparkYuvColorRange colorRange) {                           \
      PixelToYDzDxHWY<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               colorRange);\
}

PIXEL_TO_YDZDX(uint16_t, RGBA, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, RGB, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YDZDX(uint16_t, RGBA, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, RGB, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YDZDX(uint16_t, RGBA, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, RGB, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YDZDX(uint16_t, RGBA, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, RGB, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YDZDX(uint16_t, RGBA, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, RGB, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YDZDX(uint16_t, RGBA, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, RGB, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YDZDX(uint8_t, RGBA, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint8_t, RGB, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YDZDX(uint8_t, RGBA, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint8_t, RGB, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YDZDX(uint8_t, RGBA, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint8_t, RGB, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDZDX(uint16_t, BGRA, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, ABGR, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, ARGB, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, BGR, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, BGRA, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, ABGR, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, ARGB, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, BGR, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, BGRA, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, ABGR, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, ARGB, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, BGR, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDZDX(uint8_t, BGRA, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint8_t, ABGR, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint8_t, ARGB, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint8_t, BGR, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint8_t, BGRA, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint8_t, ABGR, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint8_t, ARGB, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint8_t, BGR, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint8_t, BGRA, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint8_t, ABGR, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint8_t, ARGB, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint8_t, BGR, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YDZDX(uint16_t, BGRA, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, ABGR, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, ARGB, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, BGR, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YDZDX(uint16_t, BGRA, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, ABGR, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, ARGB, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, BGR, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YDZDX(uint16_t, BGRA, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, ABGR, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, ARGB, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YDZDX(uint16_t, BGR, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef PIXEL_TO_YDZDX

template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth>
void YDzDxToXRGB(T *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                 const uint32_t width, const uint32_t height,
                 const T *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                 const T *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                 const T *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                 const SparkYuvColorRange colorRange) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                    || chromaSubsample == YUV_SAMPLE_444, "Unexpected type");

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
  const int precision = 8;
  const float frangeReductionY = ::roundf((static_cast<float>(maxColors) / static_cast<float>(rangeY)
      * static_cast<float>(1 << precision)));
  const float frangeReductionUV = ::roundf((static_cast<float>(maxColors) / static_cast<float>(rangeUV)
      * static_cast<float>(1 << precision)));
  const int rangeReductionY = static_cast<int>(frangeReductionY);
  const int rangeReductionUV = static_cast<int>(frangeReductionUV);

  /**
   *
       * E′Y = E′G		(E66)
        E′PB = ( 0.986566 * E′B − E′Y ) ÷ 2	 	(E67)
        E′PR = ( E′R − 0.991902 * E′Y ) ÷ 2

        E'G = E'Y
        E'R = (E'PR * 2 + E'Y) / 0.991902
        E'B = (E'PB * 2 + E'Y) / 0.986566
       **/

  const int coeffPb1 = static_cast<int>(static_cast<float>(2.f / 0.986566f) * frangeReductionUV);
  const int coeffPb2 = static_cast<int>(static_cast<float>(1.f / 0.986566f) * frangeReductionUV);
  const int coeffPr1 = static_cast<int>(static_cast<float>(2.f / 0.991902f) * frangeReductionUV);
  const int coeffPr2 = static_cast<int>(static_cast<float>(1.f / 0.991902f) * frangeReductionUV);

  const int lanesForward = getYuvChromaPixels(chromaSubsample);

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
  const Rebind<int32_t, decltype(dhu16)> di32;
  const auto vRangeReductionY = Set(di32, rangeReductionY);
  const auto vRangeReductionUV = Set(di32, rangeReductionUV);
  const auto vCoeffPB1 = Set(di32, coeffPb1);
  const auto vCoeffPB2 = Set(di32, coeffPb2);
  const auto vCoeffPR1 = Set(di32, coeffPr1);
  const auto vCoeffPR2 = Set(di32, coeffPr2);

  const int lanes = Lanes(d);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh);

  for (int y = 0; y < height; ++y) {
    auto DzSource = reinterpret_cast<const T *>(mUSrc);
    auto DxSource = reinterpret_cast<const T *>(mVSrc);
    auto ySrc = reinterpret_cast<const T *>(mYSrc);
    auto store = reinterpret_cast<T *>(dst);

    uint32_t x = 0;

    for (; x + lanes < width; x += lanes) {
      VU16 Y, Dz, Dx;

      if (std::is_same<T, uint16_t>::value) {
        Y = LoadU(du16, reinterpret_cast<const uint16_t *>(ySrc));
        if (chromaSubsample == YUV_SAMPLE_444) {
          Dz = LoadU(du16, reinterpret_cast<const uint16_t *>(DzSource));
          Dx = LoadU(du16, reinterpret_cast<const uint16_t *>(DxSource));
        } else if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
          const auto hDz = LoadU(dhu16, reinterpret_cast<const uint16_t *>(DzSource));
          Dz = Combine(du16, hDz, hDz);
          Dz = InterleaveLower(Dz, Dz);
          const auto hDx = LoadU(dhu16, reinterpret_cast<const uint16_t *>(DxSource));
          Dx = Combine(du16, hDx, hDx);
          Dx = InterleaveLower(Dx, Dx);
        } else {
          static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                            || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
        }
      } else if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(du16)> du8;
        Y = PromoteTo(du16, LoadU(du8, reinterpret_cast<const uint8_t *>(ySrc)));
        if (chromaSubsample == YUV_SAMPLE_444) {
          Dz = PromoteTo(du16, LoadU(du8, reinterpret_cast<const uint8_t *>(DzSource)));
          Dx = PromoteTo(du16, LoadU(du8, reinterpret_cast<const uint8_t *>(DxSource)));
        } else if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
          const Half<decltype(du8)> dhu8;
          const auto hDz = LoadU(dhu8, reinterpret_cast<const uint8_t *>(DzSource));
          Dz = PromoteTo(du16, Combine(du8, hDz, hDz));
          Dz = InterleaveLower(Dz, Dz);
          const auto hDx = LoadU(dhu8, reinterpret_cast<const uint8_t *>(DxSource));
          Dx = PromoteTo(du16, Combine(du8, hDx, hDx));
          Dx = InterleaveLower(Dx, Dx);
        } else {
          static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                            || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
        }
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
      }

      VU16 R16, G16, B16;

      const auto sY = Sub(BitCast(di16, Y), vBiasY);
      const auto sDz = Sub(BitCast(di16, Dz), vBiasUV);
      const auto sDx = Sub(BitCast(di16, Dx), vBiasUV);

      const auto yl = PromoteLowerTo(di32, sY);
      const auto yh = PromoteUpperTo(di32, sY);

      const auto sYl = Mul(yl, vRangeReductionY);
      const auto sYh = Mul(yh, vRangeReductionY);

      const auto sBl = Add(Mul(PromoteLowerTo(di32, sDz), vCoeffPB1), Mul(yl, vCoeffPB2));
      const auto sBh = Add(Mul(PromoteUpperTo(di32, sDz), vCoeffPB1), Mul(yh, vCoeffPB2));
      const auto sRl = Add(Mul(PromoteLowerTo(di32, sDx), vCoeffPR1), Mul(yl, vCoeffPR2));
      const auto sRh = Add(Mul(PromoteUpperTo(di32, sDx), vCoeffPR1), Mul(yh, vCoeffPR2));

      const auto Gl = ShiftRightNarrow<8>(di32, sYl);
      const auto Gh = ShiftRightNarrow<8>(di32, sYh);
      const auto Bl = ShiftRightNarrow<8>(di32, sBl);
      const auto Bh = ShiftRightNarrow<8>(di32, sBh);
      const auto Rl = ShiftRightNarrow<8>(di32, sRl);
      const auto Rh = ShiftRightNarrow<8>(di32, sRh);

      G16 = BitCast(du16, Clamp(Combine(di16, Gh, Gl), viZeros, vMaxColors));
      B16 = BitCast(du16, Clamp(Combine(di16, Bh, Bl), viZeros, vMaxColors));
      R16 = BitCast(du16, Clamp(Combine(di16, Rh, Rl), viZeros, vMaxColors));
      if (std::is_same<T, uint16_t>::value) {
        StoreRGBA<PixelType>(du16, reinterpret_cast<uint16_t *>(store), R16, G16, B16, A16);
      } else if (std::is_same<T, uint8_t>::value) {
        StoreRGBA<PixelType>(du16, reinterpret_cast<uint8_t *>(store), R16, G16, B16, A16);
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
      }

      DzSource += uvLanes;
      DxSource += uvLanes;
      ySrc += lanes;
      store += lanes * components;
    }

    for (; x < width; x += lanesForward) {
      const T uValue = reinterpret_cast<const T *>(DzSource)[0];
      const T vValue = reinterpret_cast<const T *>(DxSource)[0];

      const int Y = (static_cast<int>(ySrc[0]) - biasY);
      const int Dz = (static_cast<int>(vValue) - biasUV);
      const int Dx = (static_cast<int>(uValue) - biasUV);

      int G = std::clamp((Y * rangeReductionY) >> precision, 0, maxColors);
      int B = std::clamp((Dx * coeffPb1 + Y * coeffPb2) >> precision, 0, maxColors);
      int R = std::clamp((Dz * coeffPr1 + Y * coeffPr2) >> precision, 0, maxColors);

      StoreRGBA<T, int, PixelType>(store, R, G, B, maxColors);

      store += components;
      ySrc += 1;

      if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        if (x + 1 < width) {
          int Y1 = (static_cast<int>(ySrc[0]) - biasY);
          int G1 = std::clamp((Y1 * rangeReductionY) >> precision, 0, maxColors);
          int B1 = std::clamp((Dx * coeffPb1 + Y1 * coeffPb2) >> precision, 0, maxColors);
          int R1 = std::clamp((Dz * coeffPr1 + Y * coeffPr2) >> precision, 0, maxColors);

          StoreRGBA<T, int, PixelType>(store, R1, G1, B1, maxColors);
          store += components;
          ySrc += 1;
        }
      }

      DzSource += 1;
      DxSource += 1;
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
                    const sparkyuv::SparkYuvColorRange colorRange) {                           \
      YDzDxToXRGB<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               colorRange);  \
}

YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YDzDx420, sparkyuv::YUV_SAMPLE_420)
#endif

YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YDzDx420, sparkyuv::YUV_SAMPLE_420)
#endif

YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YDzDx444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YDzDx422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YDzDx420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef YcCbcCrcToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif