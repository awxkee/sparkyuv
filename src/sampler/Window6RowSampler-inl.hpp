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
#if defined(SPARKYUV_WINDOW6_ROW_SAMPLER) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_WINDOW6_ROW_SAMPLER
#undef SPARKYUV_WINDOW6_ROW_SAMPLER
#else
#define SPARKYUV_WINDOW6_ROW_SAMPLER
#endif

#include <hwy/highway.h>
#include "ScaleRowSampler.hpp"
#include <cstdint>
#include <algorithm>
#include "sampler.h"
#include "src/math/math-inl.h"
#include <cmath>

#if HWY_TARGET != HWY_SVE && HWY_TARGET != HWY_SVE2 && HWY_TARGET != HWY_SVE_256 && HWY_TARGET != HWY_SVE2_128
#define WEIGHTED_WINDOW6_HWY 1
#else
#define WEIGHTED_WINDOW6_HWY 0
#endif

#if WEIGHTED_WINDOW6_HWY
#include "sampler-inl.h"
#endif

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
using namespace hwy;
using namespace hwy::HWY_NAMESPACE;
using namespace sparkyuv;

enum WeightedRow6Operation {
  WEIGHTED_ROW6_LANCZOS_SINC
};

template<WeightedRow6Operation op>
class WeightedWindow6RowSampler10Bit : public ScaleRowSampler<uint32_t> {
 public:
  WeightedWindow6RowSampler10Bit(const uint32_t *mSource,
                                 const int srcStride,
                                 const int inputWidth,
                                 const int inputHeight,
                                 uint32_t *mDestination,
                                 const int dstStride,
                                 const int outputWidth,
                                 const int outputHeight) :
      ScaleRowSampler<uint32_t>(mSource,
                                srcStride,
                                inputWidth,
                                inputHeight,
                                mDestination,
                                dstStride,
                                outputWidth,
                                outputHeight) {
    switch (op) {
      case WEIGHTED_ROW6_LANCZOS_SINC: {
        sampler = sparkyuv::Lanczos3Sinc;
      }
        break;
    }
  }

  void sample(const int y) override {
    auto dst = reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t * >(this->mDestination) + y * this->dstStride);

    for (int x = 0; x < this->outputWidth; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      const int a = 3;

      float rgb[4] = {0, 0, 0, 0};

      float kx1 = ::floorf(srcX);
      float ky1 = ::floorf(srcY);

      float weightSum(0.0f);

      for (int j = -a + 1; j <= a; j++) {
        int yj = (int) ky1 + j;
        float dy = float(srcY) - (float(ky1) + (float) j);
        float yWeight = sampler(dy);
        for (int i = -a + 1; i <= a; i++) {
          int xi = (int) kx1 + i;
          float dx = float(srcX) - (float(kx1) + (float) i);
          float weight = sampler(dx) * yWeight;
          weightSum += weight;

          auto row = reinterpret_cast<const uint32_t *>(reinterpret_cast<const uint8_t *>(this->mSource)
              + clamp(yj, 0, this->inputHeight - 1) * this->srcStride);

          const int px = std::clamp(xi, 0, this->inputWidth - 1);

          uint32_t color = row[px];

          float r = 0, g = 0, b = 0, aAlpha = 0;
          parseToFloat(color, r, g, b, aAlpha);

          rgb[0] += r * weight;
          rgb[1] += g * weight;
          rgb[2] += b * weight;
          rgb[3] += aAlpha * weight;
        }
      }

      const int px = x;

      if (weightSum == 0.f) {
        dst[px] = 0;
      } else {
        float revertScale = 1.f / weightSum * maxColors;
        auto R10 = static_cast<uint32_t >(std::clamp(::roundf(rgb[0] * revertScale), 0.0f, (float) maxColors));
        auto G10 = static_cast<uint32_t >(std::clamp(::roundf(rgb[1] * revertScale), 0.0f, (float) maxColors));
        auto B10 = static_cast<uint32_t >(std::clamp(::roundf(rgb[2] * revertScale), 0.0f, (float) maxColors));
        auto A10 = static_cast<uint32_t >(std::clamp(::roundf(rgb[3] / weightSum * 3.f), 0.0f, 3.0f));

        dst[x] = (A10 << 30) | (B10 << 20) | (G10 << 10) | R10;
      }
    }
  }

  ~WeightedWindow6RowSampler10Bit() override = default;

 private:
  const float maxColors = ::powf(2.0f, (float) 10.f) - 1.0f;
  ScaleWeightSampler sampler;

  inline void parseToFloat(const uint32_t rgba1010102, float &r, float &g, float &b, float &a) {
    const uint32_t scalarMask = (1u << 10u) - 1u;
    uint32_t b1 = (rgba1010102) & scalarMask;
    uint32_t g1 = (rgba1010102 >> 10) & scalarMask;
    uint32_t r1 = (rgba1010102 >> 20) & scalarMask;
    uint32_t a1 = (rgba1010102 >> 30) * 3;
    constexpr float colorScale = 1.f / 1023.f;
    constexpr float alphaScale = 1.f / 3.f;
    float rFloat = static_cast<float>(r1) * colorScale;
    float gFloat = static_cast<float>(g1) * colorScale;
    float bFloat = static_cast<float>(b1) * colorScale;
    float aFloat = static_cast<float>(a1) * alphaScale;

    r = rFloat;
    g = gFloat;
    b = bFloat;
    a = aFloat;
  }
};

template<WeightedRow6Operation op, int Components>
class WeightedWindow6RowSampler : public ScaleRowSampler<uint8_t> {
 public:
  WeightedWindow6RowSampler(const uint8_t *mSource, const int srcStride,
                            const int inputWidth, const int inputHeight,
                            uint8_t *mDestination, const int dstStride,
                            const int outputWidth, const int outputHeight) :
      ScaleRowSampler<uint8_t>(mSource,
                               srcStride,
                               inputWidth, inputHeight,
                               mDestination, dstStride,
                               outputWidth, outputHeight) {
    switch (op) {
      case WEIGHTED_ROW6_LANCZOS_SINC: {
        sampler = sparkyuv::Lanczos3Sinc;
#if WEIGHTED_WINDOW6_HWY
        samplerHWY = Lanczos3Sinc;
#endif
      }
        break;
    }
  }

  void sample(const int y) override {
    auto dst = reinterpret_cast<uint8_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + y * this->dstStride);

    const int components = Components;

#if WEIGHTED_WINDOW6_HWY
    const FixedTag<float32_t, 4> dfx4;
    const FixedTag<int32_t, 4> dix4;
    const FixedTag<uint32_t, 4> dux4;
    const FixedTag<uint8_t, 4> du8x4;
    using VI4 = Vec<decltype(dix4)>;
    using VF4 = Vec<decltype(dfx4)>;
    using VU8x4 = Vec<decltype(du8x4)>;

    const VF4 vfZeros = Zero(dfx4);
    const VF4 maxColorsV = Set(dfx4, maxColors);
#endif

    uint32_t x = 0;

#if WEIGHTED_WINDOW6_HWY
#if !NOACCELERATED_SAMPLER
    for (; x + 8 < this->outputWidth && components == 4; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      // only kernel with size 3 is supported
      constexpr int kernelSize = 3;

      float kx1 = ::floorf(srcX);
      float ky1 = ::floorf(srcY);

      float kWeightSum = 0;
      VF4 color = Set(dfx4, 0);

      const int a = kernelSize;
      const int mMaxWidth = this->inputWidth - 1;

      const int appendixLow[4] = {-2, -1, 0, 1};
      const int appendixHigh[4] = {2, 3, 0, 0};

      VF4 srcXV = Set(dfx4, srcX);
      VI4 kx1V = Set(dix4, kx1);
      const VI4 appendixLowV = LoadU(dix4, appendixLow);
      const VI4 appendixHighV = LoadU(dix4, appendixHigh);

      for (int j = -a + 1; j <= a; j++) {
        int yj = (int) ky1 + j;
        float dy = float(srcY) - (float(ky1) + (float) j);
        float yWeight = sampler(dy);
        auto row = reinterpret_cast<const uint8_t *>(this->mSource
            + std::clamp(yj, 0, this->inputHeight - 1) * this->srcStride);
        VF4 yWeightV = Set(dfx4, yWeight);
        VI4 xi = Add(kx1V, appendixLowV);
        VF4 dx = Sub(srcXV, ConvertTo(dfx4, xi));
        VF4 sampleParameter = dx;
        VF4 weights = Mul(samplerHWY(dfx4, sampleParameter), yWeightV);
        kWeightSum += ExtractLane(SumOfLanes(dfx4, weights), 0);
#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
        for (int i = 0; i < 4; ++i) {
          int sizeXPos = std::clamp(ExtractLane(xi, i), 0, mMaxWidth) * components;
          VU8x4 u81 = LoadU(du8x4, reinterpret_cast<const uint8_t *>(&row[sizeXPos]));
          VF4 fr1 = ConvertTo(dfx4, PromoteTo(dix4, u81));
          fr1 = Mul(fr1, Set(dfx4, ExtractLane(weights, i)));
          color = Add(color, fr1);
        }

        xi = Add(kx1V, appendixHighV);
        dx = Sub(srcXV, ConvertTo(dfx4, xi));
        sampleParameter = dx;
        weights = Mul(samplerHWY(dfx4, sampleParameter), yWeightV);
#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
        for (int i = 0; i < 2; ++i) {
          int sizeXPos = std::clamp(ExtractLane(xi, i), 0, mMaxWidth) * components;
          VU8x4 u81 = LoadU(du8x4, reinterpret_cast<const uint8_t *>(&row[sizeXPos]));
          VF4 fr1 = ConvertTo(dfx4, PromoteTo(dix4, u81));
          float weight = ExtractLane(weights, i);
          kWeightSum += weight;
          fr1 = Mul(fr1, Set(dfx4, weight));
          color = Add(color, fr1);
        }
      }

      if (kWeightSum == 0) {
        color = ClampRound(dfx4, color, vfZeros, maxColorsV);
        VU8x4 u8Color = DemoteTo(du8x4, ConvertTo(dux4, color));
        StoreU(u8Color, du8x4, reinterpret_cast<uint8_t *>(&dst[x * components]));
      } else {
        color = ClampRound(dfx4, Div(color, Set(dfx4, kWeightSum)), vfZeros,
                           maxColorsV);
        VU8x4 u8Color = DemoteTo(du8x4, ConvertTo(dux4, color));
        StoreU(u8Color, du8x4, reinterpret_cast<uint8_t *>(&dst[x * components]));
      }
    }
#endif
#endif

    for (; x < this->outputWidth; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      const int a = 3;
      float rgb[components];
      std::fill(rgb, rgb + components, 0.0f);

      float kx1 = ::floorf(srcX);
      float ky1 = ::floorf(srcY);

      float weightSum(0.0f);

      for (int j = -a + 1; j <= a; j++) {
        int yj = (int) ky1 + j;
        float dy = float(srcY) - (float(ky1) + (float) j);
        float yWeight = sampler(dy);
        for (int i = -a + 1; i <= a; i++) {
          int xi = (int) kx1 + i;
          float dx = float(srcX) - (float(kx1) + (float) i);
          float weight = sampler(dx) * yWeight;
          weightSum += weight;

          auto row = reinterpret_cast<const uint8_t *>(this->mSource
              + std::clamp(yj, 0, this->inputHeight - 1) * this->srcStride);

          const int px = std::clamp(xi, 0, this->inputWidth - 1) * components;
#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
          for (int c = 0; c < components; ++c) {
            auto clrf = static_cast<float>(row[px + c]);
            float clr = clrf * weight;
            rgb[c] += clr;
          }
        }
      }

      const int px = x * components;

#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
      for (int c = 0; c < components; ++c) {
        dst[px + c] = static_cast<uint8_t>(std::clamp(::roundf(rgb[c] * weightSum), 0.0f, maxColors));
      }
    }
  }

  ~WeightedWindow6RowSampler() override = default;

 private:
  typedef Vec<FixedTag<float32_t, 4>> (*ScaleWeightSamplerHWY)(FixedTag<float32_t, 4>, Vec<FixedTag<float32_t, 4>>);

  const float maxColors = ::powf(2.0f, (float) 8.f) - 1.0f;
  ScaleWeightSampler sampler;
  ScaleWeightSamplerHWY samplerHWY;
};

template<WeightedRow6Operation op, int Components>
class WeightedWindow6RowSampler16Bit : public ScaleRowSampler<uint16_t> {
 public:
  WeightedWindow6RowSampler16Bit(const uint16_t *mSource, const int srcStride,
                                 const int inputWidth, const int inputHeight,
                                 uint16_t *mDestination, const int dstStride,
                                 const int outputWidth, const int outputHeight,
                                 const int depth) :
      ScaleRowSampler<uint16_t>(mSource,
                                srcStride,
                                inputWidth, inputHeight,
                                mDestination, dstStride,
                                outputWidth, outputHeight),
      maxColors(::powf(2.0f, static_cast<float>(depth)) - 1.0f) {
    switch (op) {
      case WEIGHTED_ROW6_LANCZOS_SINC: {
        sampler = sparkyuv::Lanczos3Sinc;
      }
        break;
    }
  }

  void sample(const int y) override {
    auto dst = reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + y * this->dstStride);

    const int components = Components;

    for (int x = 0; x < this->outputWidth; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      const int a = 3;
      float rgb[components];
      std::fill(rgb, rgb + components, 0.0f);

      float kx1 = ::floorf(srcX);
      float ky1 = ::floorf(srcY);

      float weightSum(0.0f);

      for (int j = -a + 1; j <= a; j++) {
        int yj = (int) ky1 + j;
        float dy = float(srcY) - (float(ky1) + (float) j);
        float yWeight = sampler(dy);
        for (int i = -a + 1; i <= a; i++) {
          int xi = (int) kx1 + i;
          float dx = float(srcX) - (float(kx1) + (float) i);
          float weight = sampler(dx) * yWeight;
          weightSum += weight;

          auto row = reinterpret_cast<const uint16_t *>(reinterpret_cast<const uint8_t *>(this->mSource)
              + std::clamp(yj, 0, this->inputHeight - 1) * this->srcStride);

          const int px = std::clamp(xi, 0, this->inputWidth - 1) * components;
#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
          for (int c = 0; c < components; ++c) {
            auto clrf = static_cast<float>(row[px + c]);
            float clr = clrf * weight;
            rgb[c] += clr;
          }
        }
      }

      const int px = x * components;
      const float invWeightScale = weightSum != 0.f ? 1.f / weightSum : 0.f;

#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
      for (int c = 0; c < components; ++c) {
        dst[px + c] = static_cast<uint8_t>(std::clamp(::roundf(rgb[c] * invWeightScale), 0.0f, maxColors));
      }
    }
  }

  ~WeightedWindow6RowSampler16Bit() override = default;

 private:
  const float maxColors;
  ScaleWeightSampler sampler;
};

template<WeightedRow6Operation op, int Components>
class WeightedWindow6RowSamplerF16Bit : public ScaleRowSampler<uint16_t> {
 public:
  WeightedWindow6RowSamplerF16Bit(const uint16_t *mSource,
                                  const int srcStride,
                                  const int inputWidth,
                                  const int inputHeight,
                                  uint16_t *mDestination,
                                  const int dstStride,
                                  const int outputWidth,
                                  const int outputHeight) :
      ScaleRowSampler<uint16_t>(mSource, srcStride,
                                inputWidth, inputHeight,
                                mDestination, dstStride,
                                outputWidth, outputHeight) {
    switch (op) {
      case WEIGHTED_ROW6_LANCZOS_SINC: {
        sampler = sparkyuv::Lanczos3Sinc;
#if WEIGHTED_WINDOW6_HWY
        samplerHWY = Lanczos3Sinc;
#endif
      }
        break;
    }
  }

  void sample(const int y) override {
    const FixedTag<hwy::float32_t, 4> dfx4;
    const FixedTag<int32_t, 4> dix4;
    const FixedTag<hwy::float16_t, 4> df16x4;
    using VI4 = Vec<decltype(dix4)>;
    using VF4 = Vec<decltype(dfx4)>;
    using VF16x4 = Vec<decltype(df16x4)>;

    const auto src8 = reinterpret_cast<const uint8_t *>(this->mSource);
    auto dst16 = reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + y * this->dstStride);

    const int components = Components;

    uint32_t x = 0;

#if WEIGHTED_WINDOW6_HWY
#if !NOACCELERATED_SAMPLER
    for (; x + 8 < this->outputWidth && components == 4; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      const int a = 3;
      float rgb[components];
      std::fill(rgb, rgb + components, 0.0f);

      float kx1 = ::floorf(srcX);
      float ky1 = ::floorf(srcY);

      float kWeightSum = 0;
      VF4 color = Set(dfx4, 0);

      const int appendixLow[4] = {-2, -1, 0, 1};
      const int appendixHigh[4] = {2, 3, 0, 0};

      const VF4 aVector = Set(dfx4, a);
      VF4 srcXV = Set(dfx4, srcX);
      VI4 kx1V = Set(dix4, kx1);
      const VI4 appendixLowV = LoadU(dix4, appendixLow);
      const VI4 appendixHighV = LoadU(dix4, appendixHigh);

      for (int j = -a + 1; j <= a; j++) {
        int yj = (int) ky1 + j;
        float dy = float(srcY) - (float(ky1) + (float) j);
        float yWeight = sampler(dy);
        auto row =
            reinterpret_cast<const uint16_t *>(src8 + std::clamp(yj, 0, this->inputHeight - 1) * this->srcStride);
        VF4 yWeightV = Set(dfx4, yWeight);
        VI4 xi = Add(kx1V, appendixLowV);
        VF4 dx = Sub(srcXV, ConvertTo(dfx4, xi));
        VF4 sampleParameter = dx;
        VF4 weights = Mul(samplerHWY(dfx4, sampleParameter), yWeightV);
        kWeightSum += ExtractLane(SumOfLanes(dfx4, weights), 0);
        for (int i = 0; i < 4; ++i) {
          int sizeXPos = clamp(ExtractLane(xi, i), 0, mMaxWidth) * components;
          VF16x4 r1 = LoadU(df16x4, reinterpret_cast<const hwy::float16_t *>(&row[sizeXPos]));
          VF4 fr1 = PromoteTo(dfx4, r1);
          fr1 = Mul(fr1, Set(dfx4, ExtractLane(weights, i)));
          color = Add(color, fr1);
        }

        xi = Add(kx1V, appendixHighV);
        dx = Sub(srcXV, ConvertTo(dfx4, xi));
        sampleParameter = dx;
        weights = Mul(samplerHWY(dfx4, sampleParameter), yWeightV);
        for (int i = 0; i < 2; ++i) {
          int sizeXPos = clamp(ExtractLane(xi, i), 0, mMaxWidth) * components;
          VF16x4 r1 = LoadU(df16x4,
                            reinterpret_cast<const hwy::float16_t *>(&row[sizeXPos]));
          VF4 fr1 = PromoteTo(dfx4, r1);
          float weight = ExtractLane(weights, i);
          kWeightSum += weight;
          fr1 = Mul(fr1, Set(dfx4, weight));
          color = Add(color, fr1);
        }
      }

      if (kWeightSum == 0) {
        VF16x4 f16Color = DemoteTo(df16x4, color);
        StoreU(f16Color, df16x4, reinterpret_cast<hwy::float16_t *>(&dst16[x * components]));
      } else {
        VF16x4 f16Color = DemoteTo(df16x4, Div(color, Set(dfx4, kWeightSum)));
        StoreU(f16Color, df16x4, reinterpret_cast<hwy::float16_t *>(&dst16[x * components]));
      }
    }
#endif
#endif

    for (; x < this->outputWidth; ++x) {
      float srcX = (float) x * this->xScale;
      float srcY = (float) y * this->yScale;

      const int a = 3;
      float rgb[components];
      std::fill(rgb, rgb + components, 0.0f);

      float kx1 = ::floorf(srcX);
      float ky1 = ::floorf(srcY);

      float weightSum(0.0f);

      for (int j = -a + 1; j <= a; j++) {
        int yj = (int) ky1 + j;
        float dy = float(srcY) - (float(ky1) + (float) j);
        float yWeight = sampler(dy);
        for (int i = -a + 1; i <= a; i++) {
          int xi = (int) kx1 + i;
          float dx = float(srcX) - (float(kx1) + (float) i);
          float weight = sampler(dx) * yWeight;
          weightSum += weight;

          auto row =
              reinterpret_cast<const uint16_t *>(src8 + std::clamp(yj, 0, this->inputHeight - 1) * this->srcStride);

          const int px = std::clamp(xi, 0, this->inputWidth - 1) * components;

#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
          for (int c = 0; c < components; ++c) {
            float clrf = hwy::F32FromF16(hwy::float16_t::FromBits(row[px + c]));
            float clr = (float) clrf * weight;
            rgb[c] += clr;
          }
        }
      }

      const int px = x * components;
      const float invWeightScale = weightSum != 0.f ? 1.f / weightSum : 0.f;

#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
      for (int c = 0; c < components; ++c) {
        dst16[px + c] = hwy::F16FromF32(rgb[c] * invWeightScale).bits;
      }
    }
  }

  ~WeightedWindow6RowSamplerF16Bit() override = default;

 private:
  const float maxColors = ::powf(2.0f, (float) 8.f) - 1.0f;
  ScaleWeightSampler sampler;

#if WEIGHTED_WINDOW6_HWY
  typedef Vec<FixedTag<float32_t, 4>> (*ScaleWeightSamplerHWY)(FixedTag<float32_t, 4>, Vec<FixedTag<float32_t, 4>>);
  ScaleWeightSamplerHWY samplerHWY;
#endif
};
}
HWY_AFTER_NAMESPACE();

#undef WEIGHTED_WINDOW6_HWY

#endif
