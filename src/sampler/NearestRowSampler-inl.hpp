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

#if defined(SPARKYUV_NEAREST_ROW_SAMPLER) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_NEAREST_ROW_SAMPLER
#undef SPARKYUV_NEAREST_ROW_SAMPLER
#else
#define SPARKYUV_NEAREST_ROW_SAMPLER
#endif

#include "hwy/highway.h"
#include "ScaleRowSampler.hpp"
#include "sampler-inl.h"
#include "sampler.h"
#include <cstdint>
#include <algorithm>

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

using namespace sparkyuv;

template<int components>
class NearestRowSampler : public ScaleRowSampler<uint8_t> {
 public:
  NearestRowSampler(const uint8_t *mSource,
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

  void sample(const int row) override {
    auto dst = reinterpret_cast<uint8_t *>(this->mDestination + row * this->dstStride);
    if (components == 4) {
      for (uint32_t x = 0; x < this->outputWidth; ++x) {
        auto srcX = static_cast<float>(x * this->xScale);
        auto srcY = static_cast<float>(row * this->yScale);

        const int x1 = std::clamp(static_cast<int>(std::floorf(srcX)), 0, this->inputWidth - 1);
        const int y1 = std::clamp(static_cast<int>(std::floorf(srcY)), 0, this->inputHeight - 1);
        auto srcRow = reinterpret_cast<const uint8_t *>(this->mSource + y1 * this->srcStride);
        uint32_t px = reinterpret_cast<const uint32_t *>(srcRow)[x1];
        reinterpret_cast<uint32_t *>(dst)[x] = px;
      }
    } else {
      for (uint32_t x = 0; x < this->outputWidth; ++x) {
        auto srcX = static_cast<float>(x * this->xScale);
        auto srcY = static_cast<float>(row * this->yScale);

        const int x1 = std::clamp(static_cast<int>(std::floorf(srcX)), 0, this->inputWidth - 1);
        const int y1 = std::clamp(static_cast<int>(std::floorf(srcY)), 0, this->inputHeight - 1);
        auto srcRow = reinterpret_cast<const uint8_t *>(this->mSource + y1 * this->srcStride);
        auto srcPtr = &srcRow[x1 * components];
        std::copy(srcPtr, srcPtr + sizeof(uint8_t) * components, &dst[x * components]);
      }
    }
  }

  ~NearestRowSampler() override = default;

 private:
  const float maxColors = std::powf(2.0f, (float) 8.f) - 1.0f;
};

template<int Components>
class NearestRowSampler16Bit : public ScaleRowSampler<uint16_t> {
 public:
  NearestRowSampler16Bit(const uint16_t *mSource,
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

  void sample(const int row) override {
    const int components = Components;
    auto dst = reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + row * this->dstStride);
    for (int x = 0; x < this->outputWidth; ++x) {
      const float srcX = (float) x * this->xScale;
      const float srcY = (float) row * this->yScale;

      const int x1 = std::clamp(static_cast<int>(std::floorf(srcX)), 0, this->inputWidth - 1);
      const int y1 = std::clamp(static_cast<int>(std::floorf(srcY)), 0, this->inputHeight - 1);
      auto srcRow =
          reinterpret_cast<const uint16_t *>(reinterpret_cast<const uint8_t *>(this->mSource) + y1 * this->srcStride);
      auto srcPtr = &srcRow[x1 * components];
      std::copy(srcPtr, srcPtr + sizeof(uint8_t) * components, &dst[x * components]);
    }
  }

  ~NearestRowSampler16Bit() override = default;

};

class NearestRowSampler10Bit : public ScaleRowSampler<uint32_t> {
 public:
  NearestRowSampler10Bit(const uint32_t *mSource,
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

  }

  void sample(const int row) override {
    auto dst = reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(this->mDestination) + row * this->dstStride);
    for (int x = 0; x < this->outputWidth; ++x) {
      const float srcX = (float) x * xScale;
      const float srcY = (float) row * yScale;

      const int x1 = std::clamp(static_cast<int>(std::floorf(srcX)), 0, inputWidth - 1);
      const int y1 = std::clamp(static_cast<int>(std::floorf(srcY)), 0, inputHeight - 1);
      auto srcRow = reinterpret_cast<const uint32_t *>(reinterpret_cast<const uint8_t *>(mSource) + y1 * srcStride);
      dst[x] = srcRow[x1];
    }
  }

  ~NearestRowSampler10Bit() override = default;

 private:
};

}
HWY_AFTER_NAMESPACE();

#endif
