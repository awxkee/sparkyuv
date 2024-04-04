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

#if defined(SPARKYUV_BILINEAR_ROW_SAMPLER) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_BILINEAR_ROW_SAMPLER
#undef SPARKYUV_BILINEAR_ROW_SAMPLER
#else
#define SPARKYUV_BILINEAR_ROW_SAMPLER
#endif

#include <hwy/highway.h>
#include "ScaleRowSampler.hpp"
#include "sampler.h"
#include "sampler-inl.h"
#include "../yuv-inl.h"
#include <cstdint>
#include <algorithm>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
using namespace sparkyuv;
using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<int Components>
class BilinearRowSampler4Chan8Bit : public ScaleRowSampler<uint8_t> {
 public:
  BilinearRowSampler4Chan8Bit(const uint8_t *mSource,
                              const int srcStride,
                              const int inputWidth,
                              const int inputHeight,
                              uint8_t *mDestination,
                              const int dstStride,
                              const int outputWidth,
                              const int outputHeight) :
      ScaleRowSampler<uint8_t>(mSource,
                               srcStride,
                               inputWidth,
                               inputHeight,
                               mDestination,
                               dstStride,
                               outputWidth,
                               outputHeight) {

  }

  ~BilinearRowSampler4Chan8Bit() override = default;

  void sample(const int row) override {
    const FixedTag<float32_t, 4> dfx4;
    const FixedTag<uint32_t, 4> dix4;
    const FixedTag<uint8_t, 4> du8x4;
    using VU8x4 = Vec<decltype(du8x4)>;

    using VI4 = Vec<decltype(dix4)>;
    using VF4 = Vec<decltype(dfx4)>;
    const uint32_t shift[4] = {0, 1, 2, 3};
    const VI4 shiftV = LoadU(dix4, shift);
    const FixedTag<uint32_t, 4> dux4;
    const VF4 xScaleV = Set(dfx4, this->xScale);
    const VF4 yScaleV = Set(dfx4, this->yScale);

    const VI4 maxWidth = Set(dix4, this->inputWidth - 1);
    const VI4 maxHeight = Set(dix4, this->inputHeight - 1);

    const VI4 addOne = Set(dix4, 1);

    const VF4 vfZeros = Zero(dfx4);
    const VI4 srcStrideV = Set(dix4, this->srcStride);
    const VF4 maxColorsV = Set(dfx4, maxColors);

    auto dst8 = reinterpret_cast<uint8_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + row * this->dstStride);
    auto dst = reinterpret_cast<uint8_t *>(dst8);

    const uint8_t *src8 = this->mSource;
    const int components = Components;

    uint32_t x = 0;

#if !NOACCELERATED_SAMPLER
    for (; x + 8 < this->outputWidth && components == 4; ++x) {
      VI4 currentX = Set(dix4, x);
      VI4 currentXV = Add(currentX, shiftV);
      VF4 currentXVF = Mul(ConvertTo(dfx4, currentXV), xScaleV);
      VF4 currentYVF = Mul(ConvertTo(dfx4, Set(dix4, row)), yScaleV);

      VI4 xi1 = ConvertTo(dix4, Floor(currentXVF));
      VI4 yi1 = Min(ConvertTo(dix4, Floor(currentYVF)), maxHeight);

      VI4 xi2 = Min(Add(xi1, addOne), maxWidth);
      VI4 yi2 = Min(Add(yi1, addOne), maxHeight);

      VF4 dx = Max(Sub(currentXVF, ConvertTo(dfx4, xi1)), vfZeros);
      VF4 dy = Max(Sub(currentYVF, ConvertTo(dfx4, yi1)), vfZeros);

      VI4 row1Add = Mul(yi1, srcStrideV);
      VI4 row2Add = Mul(yi2, srcStrideV);

#if HWY_CXX_LANG
#pragma clang loop unroll(full)
#endif
      for (int i = 0; i < 4; i++) {
        auto row1 = reinterpret_cast<const uint8_t *>(src8 + ExtractLane(row1Add, i));
        auto row2 = reinterpret_cast<const uint8_t *>(src8 + ExtractLane(row2Add, i));

        VU8x4 lane = LoadU(du8x4, reinterpret_cast<const uint8_t *>(&row1[ExtractLane(xi1, i) * components]));
        VF4 c1 = PromoteTo(dfx4, lane);
        lane = LoadU(du8x4, reinterpret_cast<const uint8_t *>(&row1[ExtractLane(xi2, i) * components]));
        VF4 c2 = PromoteTo(dfx4, lane);
        lane = LoadU(du8x4, reinterpret_cast<const uint8_t *>(&row2[ExtractLane(xi1, i) * components]));
        VF4 c3 = PromoteTo(dfx4, lane);
        lane = LoadU(du8x4, reinterpret_cast<const uint8_t *>(&row2[ExtractLane(xi2, i) * components]));
        VF4 c4 = PromoteTo(dfx4, lane);
        VF4 value = Blerp(dfx4, c1, c2, c3, c4, Set(dfx4, ExtractLane(dx, i)),
                          Set(dfx4, ExtractLane(dy, i)));
        VF4 sum = Clamp(Round(value), vfZeros, maxColorsV);
        VU8x4 pixel = DemoteTo(du8x4, ConvertTo(dux4, sum));
        auto u8Store = &dst[ExtractLane(currentXV, i) * components];
        StoreU(pixel, du8x4, u8Store);
      }

      x += components - 1;
    }
#endif

    for (; x < this->outputWidth; ++x) {
      const float srcX = (float) x * this->xScale;
      const float srcY = (float) row * this->yScale;

      const int x1 = static_cast<int>(std::floor(srcX));
      const int y1 = static_cast<int>(std::floor(srcY));

      int x2 = std::min(x1 + 1, this->inputWidth - 1);
      int y2 = std::min(y1 + 1, this->inputHeight - 1);

      float dx = std::max((float) srcX - (float) x1, 0.0f);
      float dy = std::max((float) srcY - (float) y1, 0.0f);

      auto row1 = reinterpret_cast<const uint8_t *>(src8 + y1 * this->srcStride);
      auto row2 = reinterpret_cast<const uint8_t *>(src8 + y2 * this->srcStride);

      for (int c = 0; c < components; ++c) {
        auto c1 = static_cast<float>(row1[x1 * components + c]);
        auto c2 = static_cast<float>(row1[x2 * components + c]);
        auto c3 = static_cast<float>(row2[x1 * components + c]);
        auto c4 = static_cast<float>(row2[x2 * components + c]);

        float result = blerp(c1, c2, c3, c4, dx, dy);
        float f = result;
        f = std::clamp(std::round(f), 0.0f, maxColors);
        dst[x * components + c] = static_cast<uint8_t>(f);
      }
    }
  }

 private:
  const float maxColors = std::powf(2.0f, (float) 8.f) - 1.0f;
};

template<int Components>
class BilinearRowSamplerF16Bit : public ScaleRowSampler<uint16_t> {
 public:
  BilinearRowSamplerF16Bit(const uint16_t *mSource,
                           const int srcStride,
                           const int inputWidth,
                           const int inputHeight,
                           uint16_t *mDestination,
                           const int dstStride,
                           const int outputWidth,
                           const int outputHeight) :
      ScaleRowSampler<uint16_t>(mSource,
                                srcStride,
                                inputWidth,
                                inputHeight,
                                mDestination,
                                dstStride,
                                outputWidth,
                                outputHeight) {

  }

  ~BilinearRowSamplerF16Bit() = default;

  void sample(const int y) {
    const FixedTag<float32_t, 4> dfx4;
    const FixedTag<int32_t, 4> dix4;
    const FixedTag<hwy::float16_t, 4> df16x4;
    using VI4 = Vec<decltype(dix4)>;
    using VF4 = Vec<decltype(dfx4)>;
    using VF16x4 = Vec<decltype(df16x4)>;

    const int shift[4] = {0, 1, 2, 3};
    const VI4 shiftV = LoadU(dix4, shift);
    const VF4 xScaleV = Set(dfx4, this->xScale);
    const VF4 yScaleV = Set(dfx4, this->yScale);
    const VI4 addOne = Set(dix4, 1);
    const VF4 fOneV = Set(dfx4, 1.0f);
    const VI4 maxWidth = Set(dix4, this->inputWidth - 1);
    const VI4 maxHeight = Set(dix4, this->inputHeight - 1);
    const VF4 vfZeros = Zero(dfx4);
    const VI4 srcStrideV = Set(dix4, this->srcStride);

    const auto src8 = reinterpret_cast<const uint8_t *>(this->mSource);
    auto dst16 = reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + y * this->dstStride);

    const int components = Components;

    uint32_t x = 0;

#if !NOACCELERATED_SAMPLER
    for (; x + 8 < this->outputWidth && components == 4; ++x) {
      VI4 currentX = Set(dix4, x);
      VI4 currentXV = Add(currentX, shiftV);
      VF4 currentXVF = Mul(ConvertTo(dfx4, currentXV), xScaleV);
      VF4 currentYVF = Mul(ConvertTo(dfx4, Set(dix4, y)), yScaleV);

      VI4 xi1 = ConvertTo(dix4, Floor(currentXVF));
      VI4 yi1 = Min(ConvertTo(dix4, Floor(currentYVF)), maxHeight);

      VI4 xi2 = Min(Add(xi1, addOne), maxWidth);
      VI4 yi2 = Min(Add(yi1, addOne), maxHeight);

      VI4 row1Add = Mul(yi1, srcStrideV);
      VI4 row2Add = Mul(yi2, srcStrideV);

      VF4 dx = Max(Sub(currentXVF, ConvertTo(dfx4, xi1)), vfZeros);
      VF4 dy = Max(Sub(currentYVF, ConvertTo(dfx4, yi1)), vfZeros);

      #if HWY_CXX_LANG
      #pragma clang loop unroll(full)
      #endif
      for (int i = 0; i < 4; i++) {
        auto row1 = reinterpret_cast<const hwy::float16_t *>(src8 + ExtractLane(row1Add, i));
        auto row2 = reinterpret_cast<const hwy::float16_t *>(src8 + ExtractLane(row2Add, i));
        VF16x4 lane = LoadU(df16x4, &row1[ExtractLane(xi1, i) * components]);
        VF4 c1 = PromoteTo(dfx4, lane);
        lane = LoadU(df16x4, &row1[ExtractLane(xi2, i) * components]);
        VF4 c2 = PromoteTo(dfx4, lane);
        lane = LoadU(df16x4, &row2[ExtractLane(xi1, i) * components]);
        VF4 c3 = PromoteTo(dfx4, lane);
        lane = LoadU(df16x4, &row2[ExtractLane(xi2, i) * components]);
        VF4 c4 = PromoteTo(dfx4, lane);
        VF4 value = Blerp(dfx4, c1, c2, c3, c4, Set(dfx4, ExtractLane(dx, i)),
                          Set(dfx4, ExtractLane(dy, i)));
        VF16x4 pixel = DemoteTo(df16x4, Max(value, vfZeros));
        auto u8Store = reinterpret_cast<hwy::float16_t *>(&dst16[ExtractLane(currentXV, i) * components]);
        StoreU(pixel, df16x4, u8Store);
      }

      x += components - 1;
    }
#endif

    for (; x < this->outputWidth; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      int x1 = static_cast<int>(srcX);
      int y1 = static_cast<int>(srcY);

      int x2 = std::min(x1 + 1, this->inputWidth - 1);
      int y2 = std::min(y1 + 1, this->inputHeight - 1);

      float dx = static_cast<float>(srcX) - static_cast<float>(x1);
      float dy = static_cast<float>(srcY) - static_cast<float>(y1);

      auto row1 = reinterpret_cast<const uint16_t *>(src8 + y1 * this->srcStride);
      auto row2 = reinterpret_cast<const uint16_t *>(src8 + y2 * this->srcStride);

      const int px = x * components;

      for (int c = 0; c < components; ++c) {
        float c1 = hwy::F32FromF16(hwy::float16_t::FromBits(row1[x1 * components + c]));
        float c2 = hwy::F32FromF16(hwy::float16_t::FromBits(row1[x2 * components + c]));
        float c3 = hwy::F32FromF16(hwy::float16_t::FromBits(row2[x1 * components + c]));
        float c4 = hwy::F32FromF16(hwy::float16_t::FromBits(row2[x2 * components + c]));
        float result = blerp(c1, c2, c3, c4, dx, dy);
        dst16[px + c] = hwy::F16FromF32(result).bits;
      }
    }
  }

 private:
  const float maxColors = std::powf(2.0f, (float) 8.f) - 1.0f;
};

template<typename T, int Components>
class BilinearRowSamplerAnyBit : public ScaleRowSampler<T> {
 public:
  BilinearRowSamplerAnyBit(const T *mSource,
                           const int srcStride,
                           const int inputWidth,
                           const int inputHeight,
                           T *mDestination,
                           const int dstStride,
                           const int outputWidth,
                           const int outputHeight) :
      ScaleRowSampler<T>(mSource,
                         srcStride,
                         inputWidth,
                         inputHeight,
                         mDestination,
                         dstStride,
                         outputWidth,
                         outputHeight) {

  }

  ~BilinearRowSamplerAnyBit() = default;

  void sample(const int row) {
    auto dst8 = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(this->mDestination) + row * this->dstStride);
    auto dst = reinterpret_cast<T *>(dst8);

    auto *src8 = reinterpret_cast<const uint8_t *>(this->mSource);

    const int components = Components;

    for (int x = 0; x < this->outputWidth; ++x) {
      const float srcX = (float) x * this->xScale;
      const float srcY = (float) row * this->yScale;

      const int x1 = static_cast<int>(std::floorf(srcX));
      const int y1 = static_cast<int>(std::floorf(srcY));

      int x2 = std::min(x1 + 1, this->inputWidth - 1);
      int y2 = std::min(y1 + 1, this->inputHeight - 1);

      float dx = std::max((float) srcX - (float) x1, 0.0f);
      float dy = std::max((float) srcY - (float) y1, 0.0f);

      auto row1 = reinterpret_cast<const T *>(src8 + y1 * this->srcStride);
      auto row2 = reinterpret_cast<const T *>(src8 + y2 * this->srcStride);

#if HWY_CXX_LANG
#pragma clang loop unroll(full)
#endif
      for (int c = 0; c < components; ++c) {
        auto c1 = static_cast<float>(row1[x1 * components + c]);
        auto c2 = static_cast<float>(row1[x2 * components + c]);
        auto c3 = static_cast<float>(row2[x1 * components + c]);
        auto c4 = static_cast<float>(row2[x2 * components + c]);

        float result = blerp(c1, c2, c3, c4, dx, dy);
        float f = result;
        f = std::clamp(std::roundf(f), 0.0f, maxColors);
        dst[0] = static_cast<uint8_t>(f);
        dst += 1;
      }
    }
  }

 private:
  const float maxColors = std::powf(2.0f, (float) 8.f) - 1.0f;
};

class BilinearRowSampler10Bit : public ScaleRowSampler<uint32_t> {
 public:
  BilinearRowSampler10Bit(const uint32_t *mSource,
                          const int srcStride,
                          const int inputWidth,
                          const int inputHeight,
                          uint32_t *mDestination,
                          const int dstStride,
                          const int outputWidth,
                          const int outputHeight) :
      ScaleRowSampler<uint32_t>(mSource, srcStride, inputWidth, inputHeight,
                                mDestination, dstStride, outputWidth, outputHeight) {

  }

  ~BilinearRowSampler10Bit() override = default;

  void sample(const int y) override {
    const auto src8 = reinterpret_cast<const uint8_t *>(this->mSource);
    auto dst16 = reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + y * this->dstStride);
    for (int x = 0; x < this->outputWidth; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      int x1 = static_cast<int>(srcX);
      int y1 = static_cast<int>(srcY);

      int x2 = std::min(x1 + 1, this->inputWidth - 1);
      int y2 = std::min(y1 + 1, this->inputHeight - 1);

      float dx = static_cast<float>(srcX) - static_cast<float>(x1);
      float dy = static_cast<float>(srcY) - static_cast<float>(y1);

      auto row1 = reinterpret_cast<const uint32_t *>(src8 + y1 * this->srcStride);
      auto row2 = reinterpret_cast<const uint32_t *>(src8 + y2 * this->srcStride);

      auto c1 = static_cast<uint32_t>(row1[x1]);
      auto c2 = static_cast<uint32_t>(row1[x2]);
      auto c3 = static_cast<uint32_t>(row2[x1]);
      auto c4 = static_cast<uint32_t>(row2[x2]);

      float r1, g1, b1, a1;
      float r2, g2, b2, a2;
      float r3, g3, b3, a3;
      float r4, g4, b4, a4;

      parseToFloat(c1, r1, g1, b1, a1);
      parseToFloat(c2, r2, g2, b2, a2);
      parseToFloat(c3, r3, g3, b3, a3);
      parseToFloat(c4, r4, g4, b4, a4);

      float rInter = blerp(r1, r2, r3, r4, dx, dy);
      float gInter = blerp(g1, g2, g3, g4, dx, dy);
      float bInter = blerp(b1, b2, b3, b4, dx, dy);
      float aInter = blerp(a1, a2, a3, a4, dx, dy);

      auto R10 = static_cast<uint32_t >(std::clamp(std::roundf(rInter * maxColors), 0.0f, (float) maxColors));
      auto G10 = static_cast<uint32_t >(std::clamp(std::roundf(gInter * maxColors), 0.0f, (float) maxColors));
      auto B10 = static_cast<uint32_t >(std::clamp(std::roundf(bInter * maxColors), 0.0f, (float) maxColors));
      auto A10 = static_cast<uint32_t >(std::clamp(std::roundf(aInter * 3.f), 0.0f, 3.0f));

      dst16[0] = (A10 << 30) | (R10 << 20) | (G10 << 10) | B10;
      dst16 += 1;
    }
  }

 private:
  const float maxColors = std::powf(2.0f, (float) 10.f) - 1.0f;

  static inline void parseToFloat(const uint32_t rgba1010102, float &r, float &g, float &b, float &a) {
    const uint32_t scalarMask = (1u << 10u) - 1u;
    constexpr float colorsScale = 1.f / 1023.f;
    constexpr float alphaScale = 1.f / 3.f;
    uint32_t b1 = (rgba1010102) & scalarMask;
    uint32_t g1 = (rgba1010102 >> 10) & scalarMask;
    uint32_t r1 = (rgba1010102 >> 20) & scalarMask;
    uint32_t a1 = (rgba1010102 >> 30) * 3;
    float rFloat = static_cast<float>(r1) * colorsScale;
    float gFloat = static_cast<float>(g1) * colorsScale;
    float bFloat = static_cast<float>(b1) * colorsScale;
    float aFloat = static_cast<float>(a1) * alphaScale;

    r = rFloat;
    g = gFloat;
    b = bFloat;
    a = aFloat;
  }
};

} // aire
HWY_AFTER_NAMESPACE();

#endif //AIRE_BILINEARROWSAMPLER_HPP
