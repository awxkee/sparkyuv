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

#if defined(SPARKYUV_CHANNELS_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_CHANNELS_INL_H
#undef SPARKYUV_CHANNELS_INL_H
#else
#define SPARKYUV_CHANNELS_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

#if (HWY_ARCH_ARM_A64 && (HWY_TARGET == HWY_NEON))
#define SPARKYUV_ALLOW_ACCELERATE_RGB1010102 1
#endif

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL>
void
ReformatSurfaceF16ToU(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                      T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                      const uint32_t width, const uint32_t height, const int bitDepth) {
  const ScalableTag<uint16_t> d16;
  using VU = Vec<decltype(d16)>;
  const RebindToSigned<decltype(d16)> di16;
  const Rebind<hwy::float16_t, decltype(di16)> f16;
  const Half<decltype(d16)> dh16;
  const Rebind<hwy::float16_t, decltype(dh16)> fh16;
  const RebindToSigned<decltype(dh16)> dhi16;
  const Rebind<hwy::float32_t, decltype(dh16)> f32;
  const Rebind<uint8_t, decltype(d16)> du8;
  const Rebind<uint32_t, decltype(dh16)> d32;
  const RebindToSigned<decltype(d32)> di32;
  using VF = Vec<decltype(f16)>;
  using VF32 = Vec<decltype(f32)>;
  using VU8 = Vec<decltype(du8)>;
  using VU32 = Vec<decltype(d32)>;

  auto mSrcPixels = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  const float scale = (std::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

#if SPARKYUV_ALLOW_FLOAT16
  const VF vScale = Set(f16, hwy::F16FromF32(scale));
  const VF vAlpha1010102Scale = Set(f16, hwy::F16FromF32(3.0f));
#else
  const VF32 vScale = Set(f32, scale);
  const VF32 vAlpha1010102Scale = Set(f32, 3.f);
#endif

#if SPARKYUV_ALLOW_ACCELERATE_RGB1010102
  const bool useHWY = true;
#else
  const bool useHWY = Surface != SURFACE_RGBA1010102;
#endif

  const int components =
      (Surface == SURFACE_CHANNELS_4 || Surface == SURFACE_RGBA1010102) ? 4 : (Surface == SURFACE_CHANNELS_3 ? 3 : 1);

  const int lanes = Lanes(d16);
  const int halfLanes = lanes / 2;

  for (uint32_t y = 0; y < height; ++y) {

    auto srcPixels = reinterpret_cast<const uint16_t *>(mSrcPixels);
    auto dstPixels = reinterpret_cast<T *>(mDestination);

    uint32_t x = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
    if (useHWY) {
      for (; x + lanes < width; x += lanes) {
        VF in1, in2, in3, in4;

#if SPARKYUV_ALLOW_FLOAT16
        switch (Surface) {
          case SURFACE_CHANNEL: {
            in1 = LoadU(f16, reinterpret_cast<const hwy::float16_t *>(srcPixels));
          }
            break;
          case SURFACE_CHANNELS_3: {
            LoadInterleaved3(f16, reinterpret_cast<const hwy::float16_t *>(srcPixels), in1, in2, in3);
          }
            break;
          case SURFACE_RGBA1010102:
          case SURFACE_CHANNELS_4: {
            LoadInterleaved4(f16, reinterpret_cast<const hwy::float16_t *>(srcPixels), in1, in2, in3, in4);
          }
            break;

        }
        VU out1, out2, out3, out4;
        out1 = ConvertTo(d16, Mul(in1, vScale));
        switch (Surface) {
          case SURFACE_CHANNELS_3: {
            out2 = ConvertTo(d16, Mul(in2, vScale));
            out3 = ConvertTo(d16, Mul(in3, vScale));
          }
            break;
          case SURFACE_CHANNELS_4: {
            out2 = ConvertTo(d16, Mul(in2, vScale));
            out3 = ConvertTo(d16, Mul(in3, vScale));
            out4 = ConvertTo(d16, Mul(in4, vScale));
          }
            break;
          case SURFACE_RGBA1010102: {
            out2 = ConvertTo(d16, Mul(in2, vScale));
            out3 = ConvertTo(d16, Mul(in3, vScale));
            out4 = ConvertTo(d16, Mul(in4, vAlpha1010102Scale));
          }
            break;
        }
#else
        switch (Surface) {
          case SURFACE_CHANNEL: {
            in1 = BitCast(f16, LoadU(d16, reinterpret_cast<const uint16_t *>(srcPixels)));
          }
            break;
          case SURFACE_CHANNELS_3: {
            VU uin1, uin2, uin3;
            LoadInterleaved3(d16, reinterpret_cast<const uint16_t *>(srcPixels), uin1, uin2, uin3);
            in1 = BitCast(f16, uin1);
            in2 = BitCast(f16, uin2);
            in3 = BitCast(f16, uin3);
          }
            break;
          case SURFACE_RGBA1010102:
          case SURFACE_CHANNELS_4: {
            VU uin1, uin2, uin3, uin4;
            LoadInterleaved4(d16, reinterpret_cast<const uint16_t *>(srcPixels), uin1, uin2, uin3, uin4);
            in1 = BitCast(f16, uin1);
            in2 = BitCast(f16, uin2);
            in3 = BitCast(f16, uin3);
            in4 = BitCast(f16, uin4);
          }
            break;
        }

        const VF32 in1l = Mul(PromoteLowerTo(f32, in1), vScale);
        const VF32 in1h = Mul(PromoteUpperTo(f32, in1), vScale);
        VU out1, out2, out3, out4;

        const auto uout1l = DemoteTo(dhi16, ConvertTo(di32, in1l));
        const auto uout1h = DemoteTo(dhi16, ConvertTo(di32, in1h));

        out1 = Combine(d16, BitCast(dh16, uout1h), BitCast(dh16, uout1l));

        switch (Surface) {
          case SURFACE_CHANNELS_3: {
            const VF32 in2l = Mul(PromoteLowerTo(f32, in2), vScale);
            const VF32 in2h = Mul(PromoteUpperTo(f32, in2), vScale);

            const auto uout2l = DemoteTo(dhi16, ConvertTo(di32, in2l));
            const auto uout2h = DemoteTo(dhi16, ConvertTo(di32, in2h));
            out2 = Combine(d16, BitCast(dh16, uout2h), BitCast(dh16, uout2l));

            const VF32 in3l = Mul(PromoteLowerTo(f32, in3), vScale);
            const VF32 in3h = Mul(PromoteUpperTo(f32, in3), vScale);

            const auto uout3l = DemoteTo(dhi16, ConvertTo(di32, in3l));
            const auto uout3h = DemoteTo(dhi16, ConvertTo(di32, in3h));
            out3 = Combine(d16, BitCast(dh16, uout3h), BitCast(dh16, uout3l));
          }
            break;
          case SURFACE_CHANNELS_4: {
            const VF32 in2l = Mul(PromoteLowerTo(f32, in2), vScale);
            const VF32 in2h = Mul(PromoteUpperTo(f32, in2), vScale);

            const auto uout2l = DemoteTo(dhi16, ConvertTo(di32, in2l));
            const auto uout2h = DemoteTo(dhi16, ConvertTo(di32, in2h));
            out2 = Combine(d16, BitCast(dh16, uout2h), BitCast(dh16, uout2l));

            const VF32 in3l = Mul(PromoteLowerTo(f32, in3), vScale);
            const VF32 in3h = Mul(PromoteUpperTo(f32, in3), vScale);

            const auto uout3l = DemoteTo(dhi16, ConvertTo(di32, in3l));
            const auto uout3h = DemoteTo(dhi16, ConvertTo(di32, in3h));
            out3 = Combine(d16, BitCast(dh16, uout3h), BitCast(dh16, uout3l));

            const VF32 in4l = Mul(PromoteLowerTo(f32, in4), vScale);
            const VF32 in4h = Mul(PromoteUpperTo(f32, in4), vScale);

            const auto uout4l = DemoteTo(dhi16, ConvertTo(di32, in4l));
            const auto uout4h = DemoteTo(dhi16, ConvertTo(di32, in4h));
            out4 = Combine(d16, BitCast(dh16, uout4h), BitCast(dh16, uout4l));
          }
            break;
          case SURFACE_RGBA1010102: {
            const VF32 in2l = Mul(PromoteLowerTo(f32, in2), vScale);
            const VF32 in2h = Mul(PromoteUpperTo(f32, in2), vScale);
            const auto uout2l = DemoteTo(dhi16, ConvertTo(di32, in2l));
            const auto uout2h = DemoteTo(dhi16, ConvertTo(di32, in2h));
            out2 = Combine(d16, BitCast(dh16, uout2h), BitCast(dh16, uout2l));

            const VF32 in3l = Mul(PromoteLowerTo(f32, in3), vScale);
            const VF32 in3h = Mul(PromoteUpperTo(f32, in3), vScale);
            const auto uout3l = DemoteTo(dhi16, ConvertTo(di32, in3l));
            const auto uout3h = DemoteTo(dhi16, ConvertTo(di32, in3h));
            out3 = Combine(d16, BitCast(dh16, uout3h), BitCast(dh16, uout3l));

            const VF32 in4l = Mul(PromoteLowerTo(f32, in4), vAlpha1010102Scale);
            const VF32 in4h = Mul(PromoteUpperTo(f32, in4), vAlpha1010102Scale);
            const auto uout4l = DemoteTo(dhi16, ConvertTo(di32, in4l));
            const auto uout4h = DemoteTo(dhi16, ConvertTo(di32, in4h));
            out4 = Combine(d16, BitCast(dh16, uout4h), BitCast(dh16, uout4l));
          }
            break;
          default:break;
        }
#endif

        switch (Surface) {
          case SURFACE_CHANNEL: {
            if (std::is_same<T, uint8_t>::value) {
              StoreU(DemoteTo(du8, out1), du8, reinterpret_cast<uint8_t *>(dstPixels));
            } else {
              StoreU(out1, d16, reinterpret_cast<uint16_t *>(dstPixels));
            }
          }
            break;
          case SURFACE_CHANNELS_3: {
            if (std::is_same<T, uint8_t>::value) {
              StoreInterleaved3(DemoteTo(du8, out1), DemoteTo(du8, out2),
                                DemoteTo(du8, out3), du8, reinterpret_cast<uint8_t *>(dstPixels));
            } else {
              StoreInterleaved3(out1, out2, out3, d16, reinterpret_cast<uint16_t *>(dstPixels));
            }
          }
            break;
          case SURFACE_CHANNELS_4: {
            if (std::is_same<T, uint8_t>::value) {
              StoreInterleaved4(DemoteTo(du8, out1), DemoteTo(du8, out2),
                                DemoteTo(du8, out3), DemoteTo(du8, out4),
                                du8, reinterpret_cast<uint8_t *>(dstPixels));
            } else {
              StoreInterleaved4(out1, out2, out3, out4,
                                d16, reinterpret_cast<uint16_t *>(dstPixels));
            }
          }
            break;
          case SURFACE_RGBA1010102: {
            const auto uout1l = ConvertTo(f32, PromoteLowerTo(di32, BitCast(di16, out1)));
            const auto uout2l = ConvertTo(f32, PromoteLowerTo(di32, BitCast(di16, out2)));
            const auto uout3l = ConvertTo(f32, PromoteLowerTo(di32, BitCast(di16, out3)));
            const auto uout4l = ConvertTo(f32, PromoteLowerTo(di32, BitCast(di16, out3)));

            const auto uout1h = ConvertTo(f32, PromoteUpperTo(di32, BitCast(di16, out1)));
            const auto uout2h = ConvertTo(f32, PromoteUpperTo(di32, BitCast(di16, out2)));
            const auto uout3h = ConvertTo(f32, PromoteUpperTo(di32, BitCast(di16, out3)));
            const auto uout4h = ConvertTo(f32, PromoteUpperTo(di32, BitCast(di16, out3)));

            const auto al = ConvertTo(d32, uout4l);
            const auto rl = ConvertTo(d32, uout1l);
            const auto gl = ConvertTo(d32, uout2l);
            const auto bl = ConvertTo(d32, uout3l);
            const VU32 Lowh = Or(ShiftLeft<30>(al), ShiftLeft<20>(rl));
            const VU32 Lowl = Or(ShiftLeft<10>(gl), bl);
            const VU32 Low = Or(Lowh, Lowl);

            const auto ah = ConvertTo(d32, uout4h);
            const auto rh = ConvertTo(d32, uout1h);
            const auto gh = ConvertTo(d32, uout2h);
            const auto bh = ConvertTo(d32, uout3h);
            const VU32 Highh = Or(ShiftLeft<30>(ah), ShiftLeft<20>(rh));
            const VU32 Highl = Or(ShiftLeft<10>(gh), bh);
            const VU32 High = Or(Highh, Highl);
#if HWY_IS_LITTLE_ENDIAN
            StoreU(Low, d32, reinterpret_cast<uint32_t *>(dstPixels));
            StoreU(High, d32, reinterpret_cast<uint32_t *>(dstPixels) + halfLanes);
#else
            StoreU(High, d32, reinterpret_cast<uint32_t*>(dstPixels) + halfLanes);
            StoreU(Low, d32, reinterpret_cast<uint32_t*>(dstPixels));
#endif
          }
        }

        srcPixels += components * lanes;
        dstPixels += components * lanes;
      }
    }
#pragma clang diagnostic pop

    for (; x < width; ++x) {
      float r;
      float g;
      float b;
      float a;

      uint16_t out0, out1, out2, out3;

      switch (Surface) {
        case SURFACE_CHANNEL: {
          r = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[0])) * scale;
          out0 = static_cast<uint16_t>(r);
        }
          break;
        case SURFACE_CHANNELS_3: {
          r = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[0])) * scale;
          out0 = static_cast<uint16_t>(r);
          g = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[1])) * scale;
          out1 = static_cast<uint16_t>(g);
          b = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[2])) * scale;
          out2 = static_cast<uint16_t>(b);
        }
          break;
        case SURFACE_CHANNELS_4: {
          r = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[0])) * scale;
          out0 = static_cast<uint16_t>(r);
          g = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[1])) * scale;
          out1 = static_cast<uint16_t>(g);
          b = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[2])) * scale;
          out2 = static_cast<uint16_t>(b);
          a = hwy::F32FromF16(hwy::float16_t::FromBits(srcPixels[3])) * scale;
          out3 = static_cast<uint16_t>(a);
        }
          break;
        case SURFACE_RGBA1010102: {
          uint16_t bits0 = srcPixels[0];
          float s1 = hwy::F32FromF16(hwy::float16_t::FromBits(bits0));
          r = std::roundf(s1 * scale);
          out0 = static_cast<uint16_t>(r);
          uint16_t bits1 = srcPixels[1];
          float s2 = hwy::F32FromF16(hwy::float16_t::FromBits(bits1));
          g = std::roundf(s2 * scale);
          out1 = static_cast<uint16_t>(g);
          uint16_t bits2 = srcPixels[2];
          float s3 = hwy::F32FromF16(hwy::float16_t::FromBits(bits2));
          b = std::roundf(s3 * scale);
          out2 = static_cast<uint16_t>(b);
          uint16_t bits3 = srcPixels[3];
          float s4 = hwy::F32FromF16(hwy::float16_t::FromBits(bits3));
          a = std::min(std::roundf(s4 * 3.f), 3.f);
          out3 = static_cast<uint16_t>(a);
        }
          break;
      }

      switch (Surface) {
        case SURFACE_CHANNEL: {
          dstPixels[0] = out0;
        }
          break;
        case SURFACE_CHANNELS_3: {
          dstPixels[0] = out0;
          dstPixels[1] = out1;
          dstPixels[2] = out2;
        }
          break;
        case SURFACE_CHANNELS_4: {
          dstPixels[0] = out0;
          dstPixels[1] = out1;
          dstPixels[2] = out2;
          dstPixels[3] = out3;
        }
          break;
        case SURFACE_RGBA1010102: {
          auto R10 = (uint32_t) out0;
          auto G10 = (uint32_t) out1;
          auto B10 = (uint32_t) out2;
          auto A10 = (uint32_t) out3;

          reinterpret_cast<uint32_t *>(dstPixels)[0] = (A10 << 30) | (R10 << 20) | (G10 << 10) | B10;
        }
          break;
      }

      srcPixels += components;
      dstPixels += components;
    }

    mSrcPixels += srcStride;
    mDestination += newStride;
  }
}

template<class T, SparkYuvSurfaceChannels Surface = sparkyuv::SURFACE_CHANNEL>
void
ReformatSurfaceToF16(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                     uint16_t *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                     const uint32_t width, const uint32_t height, const int bitDepth) {
  const ScalableTag<uint16_t> d16;
  using VU = Vec<decltype(d16)>;
  const Rebind<hwy::float16_t, decltype(d16)> f16;
  const Half<decltype(d16)> dh16;
  const Rebind<hwy::float32_t, decltype(dh16)> f32;
  const Rebind<uint32_t, decltype(dh16)> d32;
  const Rebind<hwy::float16_t, decltype(dh16)> fh16;
  const RebindToSigned<decltype(d16)> di16;
  const RebindToSigned<decltype(d32)> di32;
  using VF32 = Vec<decltype(f32)>;
  const Rebind<uint8_t, decltype(d16)> du8;
  using VF = Vec<decltype(f16)>;
  using VF32 = Vec<decltype(f32)>;
  using VU8 = Vec<decltype(du8)>;
  using VU32 = Vec<decltype(d32)>;

  auto mSrcPixels = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  const float scale = 1.f / (std::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

#if SPARKYUV_ALLOW_FLOAT16
  const VF vScale = Set(f16, hwy::F16FromF32(scale));
  const VF vAlpha1010102Scale = Set(f16, hwy::F16FromF32(1.f/3.0f));
#else
  const VF32 vScale = Set(f32, scale);
  const VF32 vAlpha1010102Scale = Set(f32, 1.f / 3.f);
#endif

#if SPARKYUV_ALLOW_ACCELERATE_RGB1010102
  const bool useHWY = true;
#else
  const bool useHWY = Surface != SURFACE_RGBA1010102;
#endif

  const int components =
      (Surface == SURFACE_CHANNELS_4 || Surface == SURFACE_RGBA1010102) ? 4 : (Surface == SURFACE_CHANNELS_3 ? 3 : 1);

  const int lanes = Lanes(d16);
  const int halfLanes = lanes / 2;
  const auto mask1010102 = Set(d32, (1u << 10u) - 1u);

  for (uint32_t y = 0; y < height; ++y) {

    auto srcPixels = reinterpret_cast<const T *>(mSrcPixels);
    auto dstPixels = reinterpret_cast<uint16_t *>(mDestination);

    uint32_t x = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
    if (useHWY) {
      for (; x + lanes < width; x += lanes) {
        VF in1, in2, in3, in4;

        switch (Surface) {
          case SURFACE_CHANNEL: {
#if SPARKYUV_ALLOW_FLOAT16
            if (std::is_same<T, uint16_t>::value) {
              in1 = ConvertTo(f16, LoadU(d16, reinterpret_cast<const uint16_t *>(srcPixels)));
            } else {
              in1 = ConvertTo(f16, PromoteTo(d16, LoadU(du8, reinterpret_cast<const uint8_t *>(srcPixels))));
            }
#else
            if (std::is_same<T, uint16_t>::value) {
              const auto source = BitCast(di16, LoadU(d16, reinterpret_cast<const uint16_t *>(srcPixels)));
              const auto hi = DemoteTo(fh16, ConvertTo(f32, PromoteUpperTo(di32, source)));
              const auto lo = DemoteTo(fh16, ConvertTo(f32, PromoteLowerTo(di32, source)));
              in1 = Combine(f16, hi, lo);
            } else {
              const auto
                  source = BitCast(di16, PromoteTo(d16, LoadU(du8, reinterpret_cast<const uint8_t *>(srcPixels))));
              const auto hi = DemoteTo(fh16, ConvertTo(f32, PromoteUpperTo(di32, source)));
              const auto lo = DemoteTo(fh16, ConvertTo(f32, PromoteLowerTo(di32, source)));
              in1 = Combine(f16, hi, lo);
            }
#endif
          }
            break;
          case SURFACE_CHANNELS_3: {
            VU uin1, uin2, uin3;
            if (std::is_same<T, uint16_t>::value) {
              LoadInterleaved3(d16, reinterpret_cast<const uint16_t *>(srcPixels), uin1, uin2, uin3);
            } else {
              VU8 u8in1, u8in2, u8in3;
              LoadInterleaved3(du8, reinterpret_cast<const uint8_t *>(srcPixels), u8in1, u8in2, u8in3);
              uin1 = PromoteTo(d16, u8in1);
              uin2 = PromoteTo(d16, u8in2);
              uin3 = PromoteTo(d16, u8in3);
            }
#if HWY_TARGET == HWY_EMU128
            in1 = Int16ToFloat(f16, uin1);
            in2 = Int16ToFloat(f16, uin2);
            in3 = Int16ToFloat(f16, uin3);
#else
            in1 = ConvertTo(f16, uin1);
            in2 = ConvertTo(f16, uin2);
            in3 = ConvertTo(f16, uin3);
#endif
          }
            break;
          case SURFACE_CHANNELS_4: {
            VU uin1, uin2, uin3, uin4;
            if (std::is_same<T, uint16_t>::value) {
              LoadInterleaved4(d16, reinterpret_cast<const uint16_t *>(srcPixels), uin1, uin2, uin3, uin4);
            } else {
              VU8 u8in1, u8in2, u8in3, u8in4;
              LoadInterleaved4(du8, reinterpret_cast<const uint8_t *>(srcPixels), u8in1, u8in2, u8in3, u8in4);
              uin1 = PromoteTo(d16, u8in1);
              uin2 = PromoteTo(d16, u8in2);
              uin3 = PromoteTo(d16, u8in3);
              uin4 = PromoteTo(d16, u8in4);
            }
#if HWY_TARGET == HWY_EMU128
            in1 = Int16ToFloat(f16, uin1);
            in2 = Int16ToFloat(f16, uin2);
            in3 = Int16ToFloat(f16, uin3);
            in4 = Int16ToFloat(f16, uin4);
#else
            in1 = ConvertTo(f16, uin1);
            in2 = ConvertTo(f16, uin2);
            in3 = ConvertTo(f16, uin3);
            in4 = ConvertTo(f16, uin4);
#endif
          }
            break;
          case SURFACE_RGBA1010102: {
#if HWY_IS_LITTLE_ENDIAN
            auto color1010102l = LoadU(d32, reinterpret_cast<const uint32_t *>(srcPixels));
            auto color1010102h = LoadU(d32, reinterpret_cast<const uint32_t *>(srcPixels) + halfLanes);
#else
            auto color1010102l = LoadU(d32, reinterpret_cast<const uint32_t *>(srcPixels) + halfLanes);
            auto color1010102h = LoadU(d32, reinterpret_cast<const uint32_t *>(srcPixels));
#endif
#if SPARKYUV_ALLOW_FLOAT16
            in1 = ConvertTo(f16, Combine(d16, DemoteTo(dh16, And(ShiftRight<20>(color1010102h), mask1010102)),
                                         DemoteTo(dh16, And(ShiftRight<20>(color1010102l), mask1010102))));
            in2 = ConvertTo(f16, Combine(d16, DemoteTo(dh16, And(ShiftRight<10>(color1010102h), mask1010102)),
                                         DemoteTo(dh16, And(ShiftRight<10>(color1010102l), mask1010102))));
            in3 = ConvertTo(f16, Combine(d16, DemoteTo(dh16, And(color1010102h, mask1010102)),
                                         DemoteTo(dh16, And(color1010102l, mask1010102))));
            in4 = ConvertTo(f16, Combine(d16, DemoteTo(dh16, ShiftRight<30>(color1010102h)),
                                         DemoteTo(dh16, ShiftRight<30>(color1010102l))));
#else
#if HWY_TARGET == HWY_EMU128
            const auto in1h = DemoteTo(fh16, ConvertTo(f32, And(ShiftRight<20>(color1010102h), mask1010102)));
            const auto in1l = DemoteTo(fh16, ConvertTo(f32, And(ShiftRight<20>(color1010102l), mask1010102)));
            in1 = Combine(f16, in1h, in1l);
            in2 = Int16ToFloat(f16, Combine(d16, DemoteTo(dh16, And(ShiftRight<10>(color1010102h), mask1010102)),
                                         DemoteTo(dh16, And(ShiftRight<10>(color1010102l), mask1010102))));
            in3 = Int16ToFloat(f16, Combine(d16, DemoteTo(dh16, And(color1010102h, mask1010102)),
                                         DemoteTo(dh16, And(color1010102l, mask1010102))));
            in4 = Int16ToFloat(f16, Combine(d16, DemoteTo(dh16, ShiftRight<30>(color1010102h)),
                                         DemoteTo(dh16, ShiftRight<30>(color1010102l))));
#else
            const auto in1h = DemoteTo(fh16, ConvertTo(f32, And(ShiftRight<20>(color1010102h), mask1010102)));
            const auto in1l = DemoteTo(fh16, ConvertTo(f32, And(ShiftRight<20>(color1010102l), mask1010102)));
            in1 = Combine(f16, in1h, in1l);
            in2 = ConvertTo(f16, Combine(d16, DemoteTo(dh16, And(ShiftRight<10>(color1010102h), mask1010102)),
                                         DemoteTo(dh16, And(ShiftRight<10>(color1010102l), mask1010102))));
            in3 = ConvertTo(f16, Combine(d16, DemoteTo(dh16, And(color1010102h, mask1010102)),
                                         DemoteTo(dh16, And(color1010102l, mask1010102))));
            in4 = ConvertTo(f16, Combine(d16, DemoteTo(dh16, ShiftRight<30>(color1010102h)),
                                         DemoteTo(dh16, ShiftRight<30>(color1010102l))));
#endif
#endif
          }
            break;
        }

        VF out1, out2, out3, out4;
#if SPARKYUV_ALLOW_FLOAT16
        out1 = Mul(in1, vScale);
        switch (Surface) {
          case SURFACE_CHANNELS_3: {
            out2 = Mul(in2, vScale);
            out3 = Mul(in3, vScale);
          }
            break;
          case SURFACE_CHANNELS_4: {
            out2 = Mul(in2, vScale);
            out3 = Mul(in3, vScale);
            out4 = Mul(in4, vScale);
          }
            break;
          case SURFACE_RGBA1010102: {
            out2 = Mul(in2, vScale);
            out3 = Mul(in3, vScale);
            out4 = Mul(in4, vAlpha1010102Scale);
          }
            break;
          default:break;
        }
        switch (Surface) {
          case SURFACE_CHANNEL: {
            StoreU(out1, f16, reinterpret_cast<hwy::float16_t *>(dstPixels));
          }
            break;
          case SURFACE_CHANNELS_3: {
            StoreInterleaved3(out1, out2, out3, f16,
                              reinterpret_cast<hwy::float16_t *>(dstPixels));
          }
            break;
          case SURFACE_RGBA1010102:
          case SURFACE_CHANNELS_4: {
            StoreInterleaved4(out1, out2, out3, out4,
                              f16, reinterpret_cast<hwy::float16_t *>(dstPixels));
          }
            break;
        }
#else
        const VF32 in1l = Mul(PromoteLowerTo(f32, in1), vScale);
        const VF32 in1h = Mul(PromoteUpperTo(f32, in1), vScale);
        out1 = Combine(f16, DemoteTo(fh16, in1h), DemoteTo(fh16, in1l));

        switch (Surface) {
          case SURFACE_CHANNELS_3: {
            const VF32 in2l = Mul(PromoteLowerTo(f32, in2), vScale);
            const VF32 in2h = Mul(PromoteUpperTo(f32, in2), vScale);
            out2 = Combine(f16, DemoteTo(fh16, in2h), DemoteTo(fh16, in2l));

            const VF32 in3l = Mul(PromoteLowerTo(f32, in3), vScale);
            const VF32 in3h = Mul(PromoteUpperTo(f32, in3), vScale);
            out3 = Combine(f16, DemoteTo(fh16, in3h), DemoteTo(fh16, in3l));
          }
            break;
          case SURFACE_CHANNELS_4: {
            const VF32 in2l = Mul(PromoteLowerTo(f32, in2), vScale);
            const VF32 in2h = Mul(PromoteUpperTo(f32, in2), vScale);
            out2 = Combine(f16, DemoteTo(fh16, in2h), DemoteTo(fh16, in2l));

            const VF32 in3l = Mul(PromoteLowerTo(f32, in3), vScale);
            const VF32 in3h = Mul(PromoteUpperTo(f32, in3), vScale);
            out3 = Combine(f16, DemoteTo(fh16, in3h), DemoteTo(fh16, in3l));

            const VF32 in4l = Mul(PromoteLowerTo(f32, in4), vScale);
            const VF32 in4h = Mul(PromoteUpperTo(f32, in4), vScale);
            out4 = Combine(f16, DemoteTo(fh16, in4h), DemoteTo(fh16, in4l));
          }
            break;
          case SURFACE_RGBA1010102: {
            const VF32 in2l = Mul(PromoteLowerTo(f32, in2), vScale);
            const VF32 in2h = Mul(PromoteUpperTo(f32, in2), vScale);
            out2 = Combine(f16, DemoteTo(fh16, in2h), DemoteTo(fh16, in2l));

            const VF32 in3l = Mul(PromoteLowerTo(f32, in3), vScale);
            const VF32 in3h = Mul(PromoteUpperTo(f32, in3), vScale);
            out3 = Combine(f16, DemoteTo(fh16, in3h), DemoteTo(fh16, in3l));

            const VF32 in4l = Mul(PromoteLowerTo(f32, in4), vAlpha1010102Scale);
            const VF32 in4h = Mul(PromoteUpperTo(f32, in4), vAlpha1010102Scale);
            out4 = Combine(f16, DemoteTo(fh16, in4h), DemoteTo(fh16, in4l));
          }
            break;
          default:break;
        }
        switch (Surface) {
          case SURFACE_CHANNEL: {
            StoreU(BitCast(d16, out1), d16, reinterpret_cast<uint16_t *>(dstPixels));
          }
            break;
          case SURFACE_CHANNELS_3: {
            StoreInterleaved3(BitCast(d16, out1),
                              BitCast(d16, out2),
                              BitCast(d16, out3),
                              d16,
                              reinterpret_cast<uint16_t *>(dstPixels));
          }
            break;
          case SURFACE_RGBA1010102:
          case SURFACE_CHANNELS_4: {
            StoreInterleaved4(BitCast(d16, out1), BitCast(d16, out2), BitCast(d16, out3), BitCast(d16, out4),
                              d16, reinterpret_cast<uint16_t *>(dstPixels));
          }
            break;
        }
#endif

        srcPixels += components * lanes;
        dstPixels += components * lanes;
      }
    }

    for (; x < width; ++x) {
      float r;
      float g;
      float b;
      float a;

      uint16_t out0, out1, out2, out3;

      switch (Surface) {
        case SURFACE_CHANNEL: {
          r = static_cast<float>(srcPixels[0]) * scale;
          hwy::float16_t r16 = hwy::F16FromF32(r);
          out0 = r16.bits;
        }
          break;
        case SURFACE_CHANNELS_3: {
          r = static_cast<float >(srcPixels[0]) * scale;
          hwy::float16_t r16 = hwy::F16FromF32(r);
          out0 = r16.bits;
          g = static_cast<float >(srcPixels[1]) * scale;
          hwy::float16_t g16 = hwy::F16FromF32(g);
          out1 = g16.bits;
          b = static_cast<float >(srcPixels[2]) * scale;
          hwy::float16_t b16 = hwy::F16FromF32(b);
          out2 = b16.bits;
        }
          break;
        case SURFACE_RGBA1010102: {
          uint32_t rgba1010102 = reinterpret_cast<const uint32_t *>(srcPixels)[0];

          constexpr uint32_t scalarMask = (1u << 10u) - 1u;
          constexpr float alphaScale = 1.f / 3.f;
          uint32_t bu = (rgba1010102) & scalarMask;
          uint32_t gu = (rgba1010102 >> 10) & scalarMask;
          uint32_t ru = (rgba1010102 >> 20) & scalarMask;
          uint32_t au = (rgba1010102 >> 30);

          r = static_cast<float>(ru) * scale;
          g = static_cast<float>(gu) * scale;
          b = static_cast<float>(bu) * scale;
          a = static_cast<float>(au) * alphaScale;

          hwy::float16_t r16 = hwy::F16FromF32(r);
          out0 = r16.bits;
          hwy::float16_t g16 = hwy::F16FromF32(g);
          out1 = g16.bits;
          hwy::float16_t b16 = hwy::F16FromF32(b);
          out2 = b16.bits;
          hwy::float16_t a16 = hwy::F16FromF32(a);
          out3 = a16.bits;
        }
          break;
        case SURFACE_CHANNELS_4: {
          r = static_cast<float >(srcPixels[0]) * scale;
          hwy::float16_t r16 = hwy::F16FromF32(r);
          out0 = r16.bits;
          g = static_cast<float >(srcPixels[1]) * scale;
          hwy::float16_t g16 = hwy::F16FromF32(g);
          out1 = g16.bits;
          b = static_cast<float >(srcPixels[2]) * scale;
          hwy::float16_t b16 = hwy::F16FromF32(b);
          out2 = b16.bits;
          a = static_cast<float >(srcPixels[3]) * scale;
          hwy::float16_t a16 = hwy::F16FromF32(a);
          out3 = a16.bits;
        }
          break;
      }

      switch (Surface) {
        case SURFACE_CHANNEL: {
          dstPixels[0] = out0;
        }
          break;
        case SURFACE_CHANNELS_3: {
          dstPixels[0] = out0;
          dstPixels[1] = out1;
          dstPixels[2] = out2;
        }
          break;
        case SURFACE_RGBA1010102:
        case SURFACE_CHANNELS_4: {
          dstPixels[0] = out0;
          dstPixels[1] = out1;
          dstPixels[2] = out2;
          dstPixels[3] = out3;
        }
          break;
      }

      srcPixels += components;
      dstPixels += components;
    }

    mSrcPixels += srcStride;
    mDestination += newStride;
  }
}

template<class T,
    SparkYuvReformatPixelType InputPixelType = sparkyuv::REFORMAT_RGB,
    SparkYuvReformatPixelType DestinationPixelType = sparkyuv::REFORMAT_RGBA>
void
ChannelsReformat(const T *SPARKYUV_RESTRICT src, const uint32_t srcStride,
                 T *SPARKYUV_RESTRICT dst, const uint32_t newStride,
                 const uint32_t width, const uint32_t height, const int bitDepth) {

  const ScalableTag<T> d;
  using V = Vec<decltype(d)>;

  const T maxColors = static_cast<T>(std::powf(2.f, static_cast<float>(bitDepth)) - 1.f);

  const int srcPixelsComponents = (InputPixelType == REFORMAT_BGR || InputPixelType == REFORMAT_RGB) ? 3 : 4;
  const int
      destinationComponents = (DestinationPixelType == REFORMAT_BGR || DestinationPixelType == REFORMAT_RGB) ? 3 : 4;

  const T fillingAlpha = maxColors;
  const auto vFillingAlpha = Set(d, fillingAlpha);

  auto mSrcPixels = reinterpret_cast<const uint8_t *>(src);
  auto mDestination = reinterpret_cast<uint8_t *>(dst);

  const int lanes = Lanes(d);

  const auto alpha1010102Diff = bitDepth - 2;
  const auto r1010102Diff = static_cast<int>(std::abs(bitDepth - 10));
  const auto rightDirection1010102 = bitDepth - 10 > 0;

  for (uint32_t y = 0; y < height; ++y) {

    auto srcPixels = reinterpret_cast<const T *>(mSrcPixels);
    auto dstPixels = reinterpret_cast<T *>(mDestination);

    uint32_t x = 0;

    if (InputPixelType != REFORMAT_RGBA1010102 && DestinationPixelType != REFORMAT_RGBA1010102) {
      for (; x + lanes < width; x += lanes) {
        V R;
        V G;
        V B;
        V A;
        switch (InputPixelType) {
          case REFORMAT_RGB: {
            LoadInterleaved3(d, srcPixels, R, G, B);
            A = vFillingAlpha;
          }
            break;
          case REFORMAT_BGR: {
            LoadInterleaved3(d, srcPixels, B, G, R);
            A = vFillingAlpha;
          }
            break;
          case REFORMAT_RGBA:LoadInterleaved4(d, srcPixels, R, G, B, A);
            break;
          case REFORMAT_BGRA:LoadInterleaved4(d, srcPixels, B, G, R, A);
            break;
          case REFORMAT_ARGB:LoadInterleaved4(d, srcPixels, A, R, G, B);
            break;
          case REFORMAT_ABGR:LoadInterleaved4(d, srcPixels, A, B, G, R);
            break;
          default:break;
        }

        switch (DestinationPixelType) {
          case REFORMAT_RGBA:StoreInterleaved4(R, G, B, A, d, dstPixels);
            break;
          case REFORMAT_ABGR:StoreInterleaved4(A, B, G, R, d, dstPixels);
            break;
          case REFORMAT_BGR:StoreInterleaved3(B, G, R, d, dstPixels);
            break;
          case REFORMAT_RGB:StoreInterleaved3(R, G, B, d, dstPixels);
            break;
          case REFORMAT_BGRA:StoreInterleaved4(B, G, R, A, d, dstPixels);
            break;
          case REFORMAT_ARGB:StoreInterleaved4(A, R, G, B, d, dstPixels);
            break;
          default:break;
        }

        srcPixels += srcPixelsComponents * lanes;
        dstPixels += destinationComponents * lanes;
      }
    }

    for (; x < width; ++x) {
      T r;
      T g;
      T b;
      T a;

      switch (InputPixelType) {
        case REFORMAT_RGBA1010102: {
          uint32_t rgba1010102 = reinterpret_cast<const uint32_t *>(srcPixels)[0];
          const uint32_t scalarMask = (1u << 10u) - 1u;
          uint32_t a32, b32, g32, r32;
          if (r1010102Diff != 0) {
            if (rightDirection1010102) {
              b32 = (rgba1010102 & scalarMask) << r1010102Diff;
              g32 = (((rgba1010102 >> 10) & scalarMask) << r1010102Diff);
              r32 = (((rgba1010102 >> 20) & scalarMask) << r1010102Diff);
            } else {
              b32 = ((rgba1010102 & scalarMask) >> r1010102Diff);
              g32 = (((rgba1010102 >> 10) & scalarMask) >> r1010102Diff);
              r32 = (((rgba1010102 >> 20) & scalarMask) >> r1010102Diff);
            }
          } else {
            b32 = static_cast<T>((rgba1010102 & scalarMask));
            g32 = static_cast<T>(((rgba1010102 >> 10) & scalarMask));
            r32 = static_cast<T>(((rgba1010102 >> 20) & scalarMask));
          }
          a32 = static_cast<T>(((rgba1010102 >> 30) * 3) << alpha1010102Diff);
          r = static_cast<T>(r32);
          g = static_cast<T>(g32);
          b = static_cast<T>(b32);
          a = static_cast<T>(a32);
        }
          break;
        case REFORMAT_RGB: {
          r = static_cast<T>(srcPixels[0]);
          g = static_cast<T>(srcPixels[1]);
          b = static_cast<T>(srcPixels[2]);
          a = fillingAlpha;
        }
          break;
        case REFORMAT_RGBA: {
          r = static_cast<T>(srcPixels[0]);
          g = static_cast<T>(srcPixels[1]);
          b = static_cast<T>(srcPixels[2]);
          a = static_cast<T>(srcPixels[3]);
        }
          break;
        case REFORMAT_BGRA: {
          b = static_cast<T>(srcPixels[0]);
          g = static_cast<T>(srcPixels[1]);
          r = static_cast<T>(srcPixels[2]);
          a = static_cast<T>(srcPixels[3]);
        }
          break;
        case REFORMAT_BGR: {
          b = static_cast<T>(srcPixels[0]);
          g = static_cast<T>(srcPixels[1]);
          r = static_cast<T>(srcPixels[2]);
          a = fillingAlpha;
        }
          break;
        case REFORMAT_ARGB: {
          a = static_cast<T>(srcPixels[0]);
          r = static_cast<T>(srcPixels[1]);
          g = static_cast<T>(srcPixels[2]);
          b = static_cast<T>(srcPixels[3]);
        }
          break;
        case REFORMAT_ABGR: {
          a = static_cast<T>(srcPixels[0]);
          b = static_cast<T>(srcPixels[1]);
          g = static_cast<T>(srcPixels[2]);
          r = static_cast<T>(srcPixels[3]);
        }
          break;
      }

      switch (DestinationPixelType) {
        case REFORMAT_RGBA1010102: {
          uint32_t r32 = r, g32 = g, b32 = b, a32 = a;
          a32 = a32 >> alpha1010102Diff;
          if (r1010102Diff != 0) {
            if (rightDirection1010102) {
              r32 = r32 >> r1010102Diff;
              g32 = g32 >> r1010102Diff;
              b32 = b32 >> r1010102Diff;
            } else {
              r32 = r32 << r1010102Diff;
              g32 = g32 << r1010102Diff;
              b32 = b32 << r1010102Diff;
            }
          }
          uint32_t px = (a32 << 30) | (r32 << 20) | (g32 << 10) | b32;
          reinterpret_cast<uint32_t *>(dstPixels)[0] = px;
        }
          break;
        case REFORMAT_RGBA:dstPixels[3] = a;
        case REFORMAT_RGB: {
          dstPixels[0] = r;
          dstPixels[1] = g;
          dstPixels[2] = b;
        }
          break;
        case REFORMAT_BGRA:dstPixels[3] = a;
        case REFORMAT_BGR: {
          dstPixels[2] = r;
          dstPixels[1] = g;
          dstPixels[0] = b;
        }
          break;
        case REFORMAT_ABGR: {
          dstPixels[0] = a;
          dstPixels[3] = r;
          dstPixels[2] = g;
          dstPixels[1] = b;
        }
          break;
        case REFORMAT_ARGB: {
          dstPixels[0] = a;
          dstPixels[1] = r;
          dstPixels[2] = g;
          dstPixels[3] = b;
        }
          break;
      }

      srcPixels += srcPixelsComponents;
      dstPixels += destinationComponents;
    }

    mSrcPixels += srcStride;
    mDestination += newStride;
  }
}

#define CHANNEL_XXX_REFORMAT_DECLARATION_8(srcPixelType, dstPixelType) \
    void srcPixelType##To##dstPixelType##HWY(const uint8_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                               uint8_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                               const uint32_t width, const uint32_t height) {               \
        ChannelsReformat<uint8_t, sparkyuv::REFORMAT_##srcPixelType, sparkyuv::REFORMAT_##dstPixelType>(src, srcStride, dst, dstStride,\
                                                                                                        width, height, 8); \
    }

CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, RGBA1010102)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA1010102, RGBA)

CHANNEL_XXX_REFORMAT_DECLARATION_8(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ARGB, BGR)

CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_8(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_8(ARGB, BGRA)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_8

#define CHANNEL_XXX_REFORMAT_DECLARATION_16(srcPixelType, dstPixelType) \
    void srcPixelType##16To##dstPixelType##16HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                                 uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                                 const uint32_t width, const uint32_t height, const int depth) {\
        ChannelsReformat<uint16_t, sparkyuv::REFORMAT_##srcPixelType, sparkyuv::REFORMAT_##dstPixelType>(src, srcStride, dst, dstStride,\
                                                                                                   width, height, depth); \
    }

#define CHANNEL_XXX_REFORMAT_DECLARATION_1010102(srcPixelType, dstPixelType) \
    void srcPixelType##16To##dstPixelType##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                               uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                               const uint32_t width, const uint32_t height, const int depth) {\
        ChannelsReformat<uint16_t, sparkyuv::REFORMAT_##srcPixelType, sparkyuv::REFORMAT_##dstPixelType>(src, srcStride, dst, dstStride,\
                                                                                                   width, height, depth); \
    }

CHANNEL_XXX_REFORMAT_DECLARATION_1010102(RGBA, RGBA1010102)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_1010102

CHANNEL_XXX_REFORMAT_DECLARATION_16(RGB, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGR, BGRA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGR, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(RGB, ARGB)

CHANNEL_XXX_REFORMAT_DECLARATION_16(RGBA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ABGR, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ARGB, RGB)

CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(RGBA, BGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ABGR, RGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ARGB, BGR)

CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(BGRA, ARGB)
CHANNEL_XXX_REFORMAT_DECLARATION_16(RGBA, ABGR)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ABGR, RGBA)
CHANNEL_XXX_REFORMAT_DECLARATION_16(ARGB, BGRA)

#undef CHANNEL_XXX_REFORMAT_DECLARATION_16

#define CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION(srcPixel, dstPixel, storageType, surfaceType) \
    void srcPixel##To##dstPixel##F16HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                        uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                        const uint32_t width, const uint32_t height) {\
        const int depth = sparkyuv::SURFACE_##surfaceType == sparkyuv::SURFACE_RGBA1010102 ? 10 : 8; \
        ReformatSurfaceToF16<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                                   width, height, depth); \
    }

CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION(RGBA, RGBA, uint8_t, CHANNELS_4)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION(RGB, RGB, uint8_t, CHANNELS_3)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION(Channel, Channel, uint8_t, CHANNELS_3)
CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION(RGBA1010102, RGBA, uint8_t, RGBA1010102)

#undef CHANNEL_XXX_REFORMAT_TO_F16_DECLARATION

#define CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION(pixelType, storageType, surfaceType) \
    void pixelType##To##pixelType##F16HWY(const storageType *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                          uint16_t *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                          const uint32_t width, const uint32_t height, const int depth) {    \
        ReformatSurfaceToF16<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                           width, height, depth); \
    }

CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION(RGBA16, uint16_t, CHANNELS_4)
CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION(RGB16, uint16_t, CHANNELS_3)
CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION(Channel16, uint16_t, CHANNELS_3)

#undef CHANNEL_XXX_REFORMAT16_TO_F16_DECLARATION

#define CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(srcPixel, dstPixel, storageType, surfaceType) \
    void srcPixel##F16To##dstPixel##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                       storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                       const uint32_t width, const uint32_t height) {  \
        const int depth = sparkyuv::SURFACE_##surfaceType == sparkyuv::SURFACE_RGBA1010102 ? 10 : 8; \
        ReformatSurfaceF16ToU<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                            width, height, depth); \
    }

CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(RGBA, RGBA, uint8_t, CHANNELS_4)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(RGB, RGB, uint8_t, CHANNELS_3)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(RGBA, RGBA1010102, uint8_t, RGBA1010102)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(Channel, Channel, uint8_t, CHANNELS_3)

#undef CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION

#define CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(pixelType, storageType, surfaceType) \
    void pixelType##F16To##pixelType##HWY(const uint16_t *SPARKYUV_RESTRICT src, const uint32_t srcStride,\
                                          storageType *SPARKYUV_RESTRICT dst, const uint32_t dstStride,\
                                          const uint32_t width, const uint32_t height, const int depth) {\
        ReformatSurfaceF16ToU<storageType, sparkyuv::SURFACE_##surfaceType>(src, srcStride, dst, dstStride,\
                                                                           width, height, depth); \
    }

CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(RGBA16, uint16_t, CHANNELS_4)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(RGB16, uint16_t, CHANNELS_3)
CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION(Channel16, uint16_t, CHANNELS_3)

#undef CHANNEL_XXX_REFORMAT_F16_TO_U_DECLARATION

}

HWY_AFTER_NAMESPACE();

#endif