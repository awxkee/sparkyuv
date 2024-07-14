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

#include "FastGaussianNeon.h"

#if __aarch64__

#include <arm_neon.h>
#include <algorithm>

namespace sparkyuv {

#if defined(__GNUC__) || defined(__clang__)
#define FAST_GAUSSIAN_INLINE __attribute__((flatten)) inline
#else
#define FAST_GAUSSIAN_INLINE inline
#endif

FAST_GAUSSIAN_INLINE int32x4_t vsld_u8_to_i32(const uint8_t *src, bool useVld) {
  uint8_t safeStore[8] = {0};
  const uint8_t *origin;
  if (useVld) {
    origin = src;
  } else {
    std::copy(src, src + 3, safeStore);
    origin = safeStore;
  }
  auto fullPx = vreinterpret_s16_u16(vget_low_u16(vmovl_u8(vld1_u8(origin))));
  return vmovl_s16(fullPx);
}

void VerticalGaussianPassRGBANeon(uint8_t *data,
                                  const uint32_t stride,
                                  const uint32_t width,
                                  const uint32_t height,
                                  const int radius,
                                  const uint32_t start,
                                  const uint32_t end) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int64_t bufLength = 1023;
  int32_t buffer[1024][4] = {{0}};

  auto radius64 = static_cast<int64_t>(radius);

  auto initialSum = (radius * radius) >> 1;

  for (int64_t x = start; x < static_cast<int64_t> (width) && x < static_cast<int64_t>(end); ++x) {
    int32x4_t dif = vdupq_n_s32(0);
    int32x4_t sum = vdupq_n_s32(initialSum);
    int64_t px = x * 4;
    for (int64_t y = 0 - 2 * radius64; y < static_cast<int64_t>(height); ++y) {
      if (y >= 0) {
        auto src = reinterpret_cast<uint8_t *>(reinterpret_cast<uint8_t *>(data) + static_cast<uint32_t >(y) * stride);
        int64_t arrIndex = (y - radius64) & bufLength;
        int64_t dArrIndex = y & bufLength;

        auto p16 = vqmovun_s32(vcvtq_s32_f32(vrndq_f32(vmulq_n_f32(vcvtq_f32_s32(sum), weight))));
        auto p8 = vqmovn_u16(vcombine_u16(p16, p16));

        src[px] = vget_lane_u8(p8, 0);
        src[px + 1] = vget_lane_u8(p8, 1);
        src[px + 2] = vget_lane_u8(p8, 2);

        int32x4_t bufferValue1 = vld1q_s32(reinterpret_cast<const int *>(&buffer[arrIndex][0]));
        int32x4_t bufferValue2 = vld1q_s32(reinterpret_cast<const int *>(&buffer[dArrIndex][0]));
        bufferValue2 = vshlq_n_s32(bufferValue2, 1);

        dif = vaddq_s32(dif, vsubq_s32(bufferValue1, bufferValue2));
      } else if (y + radius >= 0) {
        int64_t dArrIndex = y & bufLength;
        int32x4_t bufferValue = vld1q_s32(reinterpret_cast<const int *>(&buffer[dArrIndex][0]));
        bufferValue = vshlq_n_s32(bufferValue, 1);
        dif = vsubq_s32(dif, bufferValue);
      }

      auto srcNext =
          reinterpret_cast<uint8_t *>(reinterpret_cast<uint8_t *>(data)
              + std::clamp(static_cast<int32_t >(y + radius), 0, static_cast<int32_t >(height - 1)) * stride);
      int32x4_t p = vsld_u8_to_i32(&srcNext[px], x + 2 < width);
      dif = vaddq_s32(dif, p);
      sum = vaddq_s32(sum, dif);

      auto arrIndex = (y + radius64) & bufLength;

      vst1q_s32(&buffer[arrIndex][0], p);
    }
  }
}

void HorizontalGaussianPassRGBANeon(uint8_t *data,
                                    const uint32_t stride,
                                    const uint32_t width,
                                    const uint32_t height,
                                    const int radius,
                                    const uint32_t start,
                                    const uint32_t end) {
  const float weight = 1.f / (static_cast<float>(radius) * static_cast<float>(radius));

  constexpr int64_t bufLength = 1023;
  int32_t buffer[1024][4] = {{0}};

  auto radius64 = static_cast<int64_t>(radius);

  auto initialSum = (radius * radius) >> 1;

  for (auto y = static_cast<int64_t>(start); y < static_cast<int64_t>(height) && y < static_cast<int64_t>(end); ++y) {
    int32x4_t dif = vdupq_n_s32(0);
    int32x4_t sum = vdupq_n_s32(initialSum);
    for (int64_t x = 0 - 2 * radius64; x < static_cast<int64_t>(width); ++x) {
      if (x >= 0) {
        const int64_t px = x * 4;

        auto src = reinterpret_cast<uint8_t *>(reinterpret_cast<uint8_t *>(data) + static_cast<uint32_t >(y) * stride);
        int64_t arrIndex = (x - radius64) & bufLength;
        int64_t dArrIndex = x & bufLength;

        auto p16 = vqmovun_s32(vcvtq_s32_f32(vrndq_f32(vmulq_n_f32(vcvtq_f32_s32(sum), weight))));
        auto p8 = vqmovn_u16(vcombine_u16(p16, p16));

        src[px] = vget_lane_u8(p8, 0);
        src[px + 1] = vget_lane_u8(p8, 1);
        src[px + 2] = vget_lane_u8(p8, 2);

        int32x4_t bufferValue1 = vld1q_s32(reinterpret_cast<const int *>(&buffer[arrIndex][0]));

        int32x4_t bufferValue2 = vld1q_s32(reinterpret_cast<const int *>(&buffer[dArrIndex][0]));
        bufferValue2 = vshlq_n_s32(bufferValue2, 1);

        dif = vaddq_s32(dif, vsubq_s32(bufferValue1, bufferValue2));
      } else if (x + radius >= 0) {
        int64_t dArrIndex = x & bufLength;
        int32x4_t bufferValue = vld1q_s32(reinterpret_cast<const int *>(&buffer[dArrIndex][0]));
        bufferValue = vshlq_n_s32(bufferValue, 1);
        dif = vsubq_s32(dif, bufferValue);
      }

      auto srcNext = reinterpret_cast<uint8_t *>(reinterpret_cast<uint8_t *>(data) + y * stride);
      int64_t px =
          std::clamp(static_cast<int64_t >(x + radius), static_cast<int64_t >(0), static_cast<int64_t>(width - 1));
      int32x4_t p = vsld_u8_to_i32(&srcNext[px*4], px + 2 < width);
      dif = vaddq_s32(dif, p);
      sum = vaddq_s32(sum, dif);

      auto arrIndex = (x + radius64) & bufLength;

      vst1q_s32(&buffer[arrIndex][0], p);
    }
  }
}

}

#endif