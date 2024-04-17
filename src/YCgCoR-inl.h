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
PixelToYCgCoRHWY(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                   const uint32_t width, const uint32_t height,
                   uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                   uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                   uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                   const SparkYuvYCgCoRType rType) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                    || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  const int yBitDepth = rType == sparkyuv::YCGCO_RE ? bitDepth + 2 : bitDepth + 1;
  GetYUVRange(sparkyuv::YUV_RANGE_PC, yBitDepth, biasY, biasUV, rangeY, rangeUV);

  auto yStore = reinterpret_cast<uint8_t *>(yPlane);
  auto uStore = reinterpret_cast<uint8_t *>(uPlane);
  auto vStore = reinterpret_cast<uint8_t *>(vPlane);
  auto mSource = reinterpret_cast<const uint8_t *>(src);

  const int components = getPixelTypeComponents(PixelType);

  const ScalableTag<uint16_t> du16;
  const RebindToSigned<decltype(du16)> di16;
  using VU16 = Vec<decltype(du16)>;
  const Rebind<T, decltype(du16)> d;
  const Half<decltype(d)> dh;
  const Half<decltype(du16)> dhu16;
  const Rebind<uint32_t, decltype(dhu16)> du32;
  const auto vBiasY = Set(di16, biasY);
  const auto vBiasUV = Set(di16, biasUV);

  const int lanes = Lanes(d);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh);
  const int lanesForward = getYuvChromaPixels(chromaSubsample);

  for (uint32_t y = 0; y < height; ++y) {
    uint32_t x = 0;

    auto yDst = reinterpret_cast<uint16_t *>(yStore);
    auto uDst = reinterpret_cast<uint16_t *>(uStore);
    auto vDst = reinterpret_cast<uint16_t *>(vStore);
    auto mSrc = reinterpret_cast<const T *>(mSource);

    for (; x + lanes < width; x += lanes) {
      VU16 R, G, B;
      if (std::is_same<T, uint8_t>::value) {
        const Rebind<uint8_t, decltype(d)> d8;
        LoadRGBTo16<PixelType>(d8, reinterpret_cast<const uint8_t *>(mSrc), R, G, B);
      } else if (std::is_same<T, uint16_t>::value) {
        LoadRGBTo16<PixelType>(du16, reinterpret_cast<const uint16_t *>(mSrc), R, G, B);
      } else {
        static_assert(std::is_same<T, uint8_t>::value || std::is_same<T, uint16_t>::value, "Unexpected storage type");
      }

      const auto iR = BitCast(di16, R);
      const auto iG = BitCast(di16, G);
      const auto iB = BitCast(di16, B);

      const auto iCo = Sub(iR, iB);
      const auto t =  Add(iB, ShiftRight<1>(iCo));
      const auto iCg = Sub(iG, t);
      const auto iY = Add(Add(t, ShiftRight<1>(iCg)), vBiasY);

      const auto Y = BitCast(du16, iY);
      const auto Cg = BitCast(du16, Add(iCg, vBiasUV));
      const auto Co = BitCast(du16, Add(iCo, vBiasUV));

      if (chromaSubsample == YUV_SAMPLE_444) {
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
        // uint16_t 12 bit max colors 4096 then 4096*2 < 2^16 so overflowing is not possible in uint16_t domain
        const auto hCg = ShiftRightNarrow<1>(du32, SumsOf2(Cg));
        const auto hCo = ShiftRightNarrow<1>(du32, SumsOf2(Co));

        if (std::is_same<T, uint16_t>::value) {
          StoreU(Y, du16, reinterpret_cast<uint16_t *>(yDst));
          if (chromaSubsample == YUV_SAMPLE_422 || (!(y & 1))) {
            StoreU(hCg, dhu16, reinterpret_cast<uint16_t *>(uDst));
            StoreU(hCo, dhu16, reinterpret_cast<uint16_t *>(vDst));
          }
        } else if (std::is_same<T, uint8_t>::value) {
          const Rebind<uint8_t, decltype(du16)> du8;
          const Rebind<uint8_t, decltype(dhu16)> dhu8;
          StoreU(DemoteTo(du8, Y), du8, reinterpret_cast<uint8_t *>(yDst));
          if (chromaSubsample == YUV_SAMPLE_422 || (!(y & 1))) {
            StoreU(DemoteTo(dhu8, hCg), dhu8, reinterpret_cast<uint8_t *>(uDst));
            StoreU(DemoteTo(dhu8, hCo), dhu8, reinterpret_cast<uint8_t *>(vDst));
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

      int Co = r - b;
      int t = b + (Co >> 1);
      int Cg = g - t;
      int Y = static_cast<int>(t + (Cg >> 1) + biasY);
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

          int drb1 = r1 - b1;
          int t1 = b1 + (drb1 >> 1);
          int dgt1 = g1 - t1;
          T Y1 = static_cast<int>(t + (dgt1) + biasY);
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

          int drb1 = r1 - b1;
          int t1 = b1 + (drb1 >> 1);
          int dgt1 = g1 - t1;
          T Y1 = static_cast<int>(t + (dgt1) + biasY);
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
        Co = Co + biasUV;
        Cg = Cg + biasUV;

        uDst[0] = Cg;
        vDst[0] = Co;
      } else if (chromaSubsample == YUV_SAMPLE_420) {
        if (!(y & 1)) {
          Co = r - b;
          t = b + (Co >> 1);
          Co += biasUV;
          Cg = (g - t + biasUV);

          uDst[0] = Cg;
          vDst[0] = Co;
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
                    uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const SparkYuvYCgCoRType rType) {                           \
      PixelToYCgCoRHWY<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                               yPlane, yStride,                  \
                                                               uPlane, uStride,                  \
                                                               vPlane, vStride,                  \
                                                               rType);\
}

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCCBCCRC(uint16_t, RGBA, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, RGB, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCCBCCRC(uint8_t, RGBA, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, RGB, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)

PIXEL_TO_YCCBCCRC(uint8_t, RGBA, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, RGB, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)

PIXEL_TO_YCCBCCRC(uint8_t, RGBA, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, RGB, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC(uint8_t, BGRA, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, ABGR, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, ARGB, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, BGR, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint8_t, BGRA, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, ABGR, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, ARGB, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, BGR, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint8_t, BGRA, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, ABGR, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, ARGB, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint8_t, BGR, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
#endif

#if SPARKYUV_FULL_CHANNELS
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
PIXEL_TO_YCCBCCRC(uint16_t, BGRA, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ABGR, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, ARGB, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
PIXEL_TO_YCCBCCRC(uint16_t, BGR, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef PIXEL_TO_YCCBCCRC

template<typename T, SparkYuvDefaultPixelType PixelType = sparkyuv::PIXEL_RGBA,
    SparkYuvChromaSubsample chromaSubsample, int bitDepth>
void YCgCoRToXRGB(T *SPARKYUV_RESTRICT rgbaData, const uint32_t dstStride,
                    const uint32_t width, const uint32_t height,
                    const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,
                    const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,
                    const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,
                    const SparkYuvYCgCoRType rType) {
  static_assert(bitDepth >= 8, "Invalid bit depth");
  static_assert(chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420
                    || chromaSubsample == YUV_SAMPLE_444, "Unexpected chroma sample type");
  auto mYSrc = reinterpret_cast<const uint8_t *>(yPlane);
  auto mUSrc = reinterpret_cast<const uint8_t *>(uPlane);
  auto mVSrc = reinterpret_cast<const uint8_t *>(vPlane);
  auto dst = reinterpret_cast<uint8_t *>(rgbaData);

  const int yBitDepth = rType == sparkyuv::YCGCO_RE ? bitDepth + 2 : bitDepth + 1;

  uint16_t biasY;
  uint16_t biasUV;
  uint16_t rangeY;
  uint16_t rangeUV;
  GetYUVRange(sparkyuv::YUV_RANGE_PC, yBitDepth, biasY, biasUV, rangeY, rangeUV);

  const int maxColors = static_cast<int>(::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

  const int lanesForward = getYuvChromaPixels(chromaSubsample);

  const int components = getPixelTypeComponents(PixelType);

  const ScalableTag<uint16_t> du16;
  const RebindToSigned<decltype(du16)> di16;
  using VU16 = Vec<decltype(du16)>;
  const Rebind<T, decltype(du16)> d;
  const Half<decltype(d)> dh;
  const Half<decltype(du16)> dhu16;
  const auto vBiasY = Set(di16, biasY);
  const auto vBiasUV = Set(di16, biasUV);
  const auto vMaxColors = Set(di16, maxColors);
  const auto viZeros = Zero(di16);
  const auto A16 = Set(du16, maxColors);

  const int lanes = Lanes(d);
  const int uvLanes = (chromaSubsample == YUV_SAMPLE_444) ? lanes : Lanes(dh);

  for (int y = 0; y < height; ++y) {
    auto CgSource = reinterpret_cast<const uint16_t *>(mUSrc);
    auto CoSource = reinterpret_cast<const uint16_t *>(mVSrc);
    auto ySrc = reinterpret_cast<const uint16_t *>(mYSrc);
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

      const auto iY = Sub(BitCast(di16, Y), vBiasY);
      const auto iCg = Sub(BitCast(di16, Cg), vBiasUV);
      const auto iCo = Sub(BitCast(di16, Co), vBiasUV);
      const auto t1 = Sub(iY, ShiftRight<1>(iCg));
      const auto iG = Add(t1, iCg);
      const auto iB = Sub(t1, ShiftRight<1>(iCo));
      const auto iR = Add(iB, iCo);

      G16 = BitCast(du16, Clamp(iG, viZeros, vMaxColors));
      B16 = BitCast(du16, Clamp(iB, viZeros, vMaxColors));
      R16 = BitCast(du16, Clamp(iR, viZeros, vMaxColors));

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

      int Y = (static_cast<int>(ySrc[0]) - biasY);
      int Co = (static_cast<int>(vValue) - biasUV);
      int Cg = (static_cast<int>(uValue) - biasUV);

      int t = Y - (Cg >> 1);
      int G = (t + Cg);
      int B = (t - (Co >> 1));
      int R = (B + Co);

      StoreRGBA<T, int, PixelType>(store, R, G, B, maxColors);

      store += components;
      ySrc += 1;

      if (chromaSubsample == YUV_SAMPLE_422 || chromaSubsample == YUV_SAMPLE_420) {
        if (x + 1 < width) {
          int Y1 = (static_cast<int>(ySrc[0]) - biasY);
          int t1 = Y1 - (Cg >> 1);
          int G1 = t1 + Cg;
          int B1 = t1 - (Co >> 1);
          int R1 = B + Co;

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

#undef SUITABLE_FLOAT_16
}

/**
* @brief It will be good to declare a type of transfer function
 * and compile each variant separately however increase of binary size about 400%
 * and compile time abound 1000% eliminates this benefit
*/
#define YcCbcCrcToXXXX_DECLARATION_R(T, PixelType, bit, yuvname, chroma) \
void yuvname##P##bit##To##PixelType##bit##HWY(T *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                    const uint32_t width, const uint32_t height,                                 \
                    const uint16_t *SPARKYUV_RESTRICT yPlane, const uint32_t yStride,                         \
                    const uint16_t *SPARKYUV_RESTRICT uPlane, const uint32_t uStride,                         \
                    const uint16_t *SPARKYUV_RESTRICT vPlane, const uint32_t vStride,                         \
                    const SparkYuvYCgCoRType rType) {                           \
      YCgCoRToXRGB<T, sparkyuv::PIXEL_##PixelType, chroma, bit>(src, srcStride, width, height,    \
                                                                yPlane, yStride,                  \
                                                                uPlane, uStride,                  \
                                                                vPlane, vStride,                  \
                                                                rType);  \
}

YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 10, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
#endif

YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGBA, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, RGB, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGRA, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ABGR, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, ARGB, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint8_t, BGR, 8, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
#endif

YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGBA, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, RGB, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)

#if SPARKYUV_FULL_CHANNELS
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YCgCoR444, sparkyuv::YUV_SAMPLE_444)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YCgCoR422, sparkyuv::YUV_SAMPLE_422)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGRA, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ABGR, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, ARGB, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
YcCbcCrcToXXXX_DECLARATION_R(uint16_t, BGR, 12, YCgCoR420, sparkyuv::YUV_SAMPLE_420)
#endif

#undef YcCbcCrcToXXXX_DECLARATION_R

}
HWY_AFTER_NAMESPACE();

#endif