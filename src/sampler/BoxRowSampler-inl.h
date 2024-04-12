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

#if defined(SPARKYUV_BOX_ROW_SAMPLER) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_BOX_ROW_SAMPLER
#undef SPARKYUV_BOX_ROW_SAMPLER
#else
#define SPARKYUV_BOX_ROW_SAMPLER
#endif

#include "hwy/highway.h"
#include "ScaleRowSampler.hpp"
#include "sampler-inl.h"
#include "sampler.h"
#include "../sparkyuv-internal.h"
#include <cstdint>
#include <algorithm>
#include <cmath>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<typename T, sparkyuv::BoxSamplerPixType PixelType, int Components>
class BoxSampler : public ScaleRowSampler<T> {
 public:
  BoxSampler(const T *mSource,
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

  ~BoxSampler() = default;

  void sample(const int row) override {
    auto dst = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(this->mDestination) + row * this->dstStride);

    auto src8 = reinterpret_cast<const uint8_t *>(this->mSource);

    const int components = Components;
#if SPARKYUV_ALLOW_FLOAT16
    const FixedTag<hwy::float16_t, 8> df16;
    const Half<decltype(df16)> dhf16;
    const auto v16Scale = Set(dhf16, hwy::F16FromF32(1.f / 4.f));
#endif

    uint32_t x = 0;
    if (PixelType == sparkyuv::BOX_UINT16 && components == 4) {
      for (; x + 2 < this->outputWidth; ++x) {
        auto srcX = static_cast<float>(x * this->xScale);
        auto srcY = static_cast<float>(row * this->yScale);

        const int x1 = static_cast<int>(::floorf(srcX));
        const int y1 = static_cast<int>(::floorf(srcY));

        const int y2 = std::min(y1 + 1, this->inputHeight - 1);

        const FixedTag<uint16_t, 8> d;
        const Half<decltype(d)> dh;
        const RepartitionToWide<decltype(d)> d32;
        const auto row1 = LoadU(d, &reinterpret_cast<const uint16_t *>(src8 + y1 * this->srcStride)[x1*4]);
        const auto row2 = LoadU(d, &reinterpret_cast<const uint16_t *>(src8 + y2 * this->srcStride)[x1*4]);
        const auto row1Upper = PromoteUpperTo(d32, row1);
        const auto row2Upper = PromoteUpperTo(d32, row2);
        const auto row1Lower = PromoteLowerTo(d32, row1);
        const auto row2Lower = PromoteLowerTo(d32, row2);
        const auto newWidePX = ShiftRight<2>(Add(Add(Add(row1Lower, row1Upper), row2Lower), row2Upper));
        const auto newPX = DemoteTo(dh, newWidePX);
        StoreU(newPX, dh, reinterpret_cast<uint16_t *>(dst));

        dst += 4;
      }
    } else if (PixelType == sparkyuv::BOX_UINT8) {
      for (; x + 2 < this->outputWidth; ++x) {
        auto srcX = static_cast<float>(x * this->xScale);
        auto srcY = static_cast<float>(row * this->yScale);

        const int x1 = static_cast<int>(::floorf(srcX));
        const int y1 = static_cast<int>(::floorf(srcY));

        const int y2 = std::min(y1 + 1, this->inputHeight - 1);

        const FixedTag<uint8_t, 8> d;
        const Half<decltype(d)> dh;
        const Rebind<uint16_t, decltype(d)> d16;
        const RepartitionToWide<decltype(d16)> d32;
        const auto row1 = LoadU(d, &reinterpret_cast<const uint8_t *>(src8 + y1 * this->srcStride)[x1*4]);
        const auto row2 = LoadU(d, &reinterpret_cast<const uint8_t *>(src8 + y2 * this->srcStride)[x1*4]);
        const auto sums = AddWide(d16, row1, row2);
        const auto newWidePX = ShiftRightNarrow<2>(d32, SumsOf2(sums));
        const auto newPX = DemoteTo(dh, newWidePX);
        StoreU(newPX, dh, reinterpret_cast<uint8_t *>(dst));
        dst += 4;
      }
    } else if (PixelType == sparkyuv::BOX_FLOAT16) {
      for (; x + 2 < this->outputWidth; ++x) {
        auto srcX = static_cast<float>(x * this->xScale);
        auto srcY = static_cast<float>(row * this->yScale);

        const int x1 = static_cast<int>(::floorf(srcX));
        const int y1 = static_cast<int>(::floorf(srcY));

        const int y2 = std::min(y1 + 1, this->inputHeight - 1);

#if SPARKYUV_ALLOW_FLOAT16
        const auto row1 = LoadU(df16, &reinterpret_cast<const hwy::float16_t *>(src8 + y1 * this->srcStride)[x1*4]);
        const auto row2 = LoadU(df16, &reinterpret_cast<const hwy::float16_t *>(src8 + y2 * this->srcStride)[x1*4]);
        const auto newWidePX = Mul(DemoteTo(df16, SumsOf2(Add(row1, row2))), v16Scale);
        const auto newPX = newWidePX;
        StoreU(newPX, dhf16, reinterpret_cast<hwy::float16_t *>(dst));
#else
        const FixedTag<uint16_t, 8> d;
        const Half<decltype(d)> dh;
        const RepartitionToWide<decltype(d)> d32;
        const Rebind<float32_t, decltype(d32)> f32;
        const Rebind<float16_t, decltype(f32)> f16;
        const auto vScale = Set(f32, 1.f / 4.f);
        const auto row1 = LoadU(d, &reinterpret_cast<const uint16_t *>(src8 + y1 * this->srcStride)[x1*4]);
        const auto row2 = LoadU(d, &reinterpret_cast<const uint16_t *>(src8 + y2 * this->srcStride)[x1*4]);
        const auto row1Upper = PromoteTo(f32, DemoteTo(f16, ConvertTo(f32, PromoteUpperTo(d32, row1))));
        const auto row2Upper = PromoteTo(f32, DemoteTo(f16, ConvertTo(f32, PromoteUpperTo(d32, row2))));
        const auto row1Lower = PromoteTo(f32, DemoteTo(f16, ConvertTo(f32, PromoteLowerTo(d32, row1))));
        const auto row2Lower = PromoteTo(f32, DemoteTo(f16, ConvertTo(f32, PromoteLowerTo(d32, row2))));
        const auto newWidePX = DemoteTo(f16, Mul(Add(Add(Add(row1Lower, row1Upper), row2Lower), row2Upper), vScale));
        const auto newPX = BitCast(dh, newWidePX);
        StoreU(newPX, dh, reinterpret_cast<uint16_t *>(dst));
#endif

        dst += 4;
      }
    }

    for (; x < this->outputWidth; ++x) {
      const float srcX = (float) x * this->xScale;
      const float srcY = (float) row * this->yScale;

      const int x1 = static_cast<int>(::floorf(srcX));
      const int y1 = static_cast<int>(::floorf(srcY));

      int x2 = std::min(x1 + 1, this->inputWidth - 1);
      int y2 = std::min(y1 + 1, this->inputHeight - 1);

      auto row1 = reinterpret_cast<const T *>(src8 + y1 * this->srcStride);
      auto row2 = reinterpret_cast<const T *>(src8 + y2 * this->srcStride);

      if (PixelType != sparkyuv::BOX_RGBA1010102 && PixelType != sparkyuv::BOX_FLOAT16) {
#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
        for (int c = 0; c < components; ++c) {
          auto c1 = static_cast<uint32_t>(row1[x1 * components + c]);
          auto c2 = static_cast<uint32_t>(row1[x2 * components + c]);
          auto c3 = static_cast<uint32_t>(row2[x1 * components + c]);
          auto c4 = static_cast<uint32_t>(row2[x2 * components + c]);

          uint32_t result = (c1 + c2 + c3 + c4) >> 2;
          dst[0] = static_cast<T>(result);
          dst += 1;
        }
      } else if (PixelType == sparkyuv::BOX_FLOAT16) {
#if defined(__clang__)
#pragma clang loop unroll(full)
#endif
        for (int c = 0; c < components; ++c) {
          constexpr float scale = 1.f / 4.f;
          auto c1 = hwy::F32FromF16(hwy::float16_t::FromBits(row1[x1 * components + c]));
          auto c2 = hwy::F32FromF16(hwy::float16_t::FromBits(row1[x2 * components + c]));
          auto c3 = hwy::F32FromF16(hwy::float16_t::FromBits(row2[x1 * components + c]));
          auto c4 = hwy::F32FromF16(hwy::float16_t::FromBits(row2[x2 * components + c]));

          float result = (c1 + c2 + c3 + c4) * scale;
          dst[0] = static_cast<T>(hwy::F16FromF32(result).bits);
          dst += 1;
        }
      } else if (PixelType == sparkyuv::BOX_RGBA1010102) {
        uint32_t p1 = reinterpret_cast<const uint32_t *>(row1)[x1];
        uint32_t p2 = reinterpret_cast<const uint32_t *>(row1)[x2];
        uint32_t p3 = reinterpret_cast<const uint32_t *>(row2)[x1];
        uint32_t p4 = reinterpret_cast<const uint32_t *>(row2)[x2];

        uint32_t r1, g1, b1, a1;
        uint32_t r2, g2, b2, a2;
        uint32_t r3, g3, b3, a3;
        uint32_t r4, g4, b4, a4;

        sparse1010102(p1, r1, g1, b1, a1);
        sparse1010102(p2, r2, g2, b2, a2);
        sparse1010102(p3, r3, g3, b3, a3);
        sparse1010102(p4, r4, g4, b4, a4);

        uint32_t r = (r1 + r2 + r3 + r4) >> 2;
        uint32_t g = (g1 + g2 + g3 + g4) >> 2;
        uint32_t b = (b1 + b2 + b3 + b4) >> 2;
        uint32_t a = (a1 + a2 + a3 + a4) >> 2;

        reinterpret_cast<uint32_t *>(dst)[0] = (a << 30) | (r << 20) | (g << 10) | b;

        if (std::is_same<T, uint8_t>::value) {
          dst += 4;
        } else if (std::is_same<T, uint32_t>::value) {
          dst += 1;
        }
      }
    }
  }

  inline void sparse1010102(const uint32_t rgba1010102, uint32_t &r, uint32_t &g, uint32_t &b, uint32_t &a) {
    constexpr uint32_t scalarMask = (1u << 10u) - 1u;
    uint32_t b1 = (rgba1010102) & scalarMask;
    uint32_t g1 = (rgba1010102 >> 10) & scalarMask;
    uint32_t r1 = (rgba1010102 >> 20) & scalarMask;
    uint32_t a1 = (rgba1010102 >> 30) * 3;

    r = r1;
    g = g1;
    b = b1;
    a = a1;
  }
};

}
HWY_AFTER_NAMESPACE();

#endif