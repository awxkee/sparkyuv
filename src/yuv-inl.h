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

#if defined(SPARKYUV__YUV_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__YUV_INL_H
#undef SPARKYUV__YUV_INL_H
#else
#define SPARKYUV__YUV_INL_H
#endif

#include "hwy/highway.h"
#include "sparkyuv-def.h"

/**
* VW ReorderWidenMulAccumulate(D d, V a, V b, VW sum0, VW& sum1): widens a and b to TFromD<D>,
* then adds a[i] * b[i] to either sum1[j] or lane j of the return value, where j = P(i) and P is a permutation.
* The only guarantee is that SumOfLanes(d, Add(return_value, sum1)) is the sum of all a[i] * b[i].
* This is useful for computing dot products and the L2 norm.
* The initial value of sum1 before any call to ReorderWidenMulAccumulate must be zero (because it is unused on some platforms).
* It is safe to set the initial value of sum0 to any vector v;
* this has the effect of increasing the total sum by GetLane(SumOfLanes(d, v)) and may be slightly more efficient than later adding v to sum0.
*
*
* However on neon target there is vmlal which works pretty good so for neon it's preferable to accumulate results with
* ReorderWidenMulAccumulate instead of passing zeros
*/
#if (HWY_ARCH_ARM_A64 && (HWY_TARGET == HWY_NEON || HWY_TARGET == HWY_NEON_WITHOUT_AES || HWY_TARGET == HWY_SVE))
#define SPARKYUV_ALLOW_WIDE_MULL_ACCUMULATE 1
#endif

#if HWY_HAVE_FLOAT16
#define SPARKYUV_ALLOW_FLOAT16 1
#endif

#define SPARKYUV_INLINE __attribute__((flatten)) inline

HWY_BEFORE_NAMESPACE();

namespace hwy::HWY_NAMESPACE {

template<typename D, HWY_IF_I16_D(D), HWY_IF_LANES_D(D, 8), typename VH = Vec<Half<D>>>
VH GetEven(D d, Vec<D> v) {
  const Half<D> dh;
  const Rebind<int32_t, decltype(dh)> du32;
  const auto erase = Set(du32, static_cast<uint32_t>(0x0000FFFF));
  return DemoteTo(dh, And(BitCast(du32, v), erase));
}

template<typename D, HWY_IF_I16_D(D), HWY_IF_LANES_D(D, 8), typename VH = Vec<Half<D>>>
VH GetOdd(D d, Vec<D> v) {
  const Half<D> dh;
  const Rebind<int32_t, decltype(dh)> du32;
  return DemoteTo(dh, ShiftRight<16>(BitCast(du32, v)));
}

template<typename D, HWY_IF_U16_D(D), HWY_IF_LANES_D(D, 8), typename VH = Vec<Half<D>>>
VH GetEven(D d, Vec<D> v) {
  const Half<D> dh;
  const Rebind<uint32_t, decltype(dh)> du32;
  const auto erase = Set(du32, static_cast<uint32_t>(0x0000FFFF));
  return DemoteTo(dh, And(BitCast(du32, v), erase));
}

template<typename D, HWY_IF_U16_D(D), HWY_IF_LANES_D(D, 8), typename VH = Vec<Half<D>>>
VH GetOdd(D d, Vec<D> v) {
  const Half<D> dh;
  const Rebind<uint32_t, decltype(dh)> du32;
  return DemoteTo(dh, ShiftRight<16>(BitCast(du32, v)));
}

// Hack for HWY EMU128.

template <class D, HWY_IF_F16_D(D)>
HWY_API Vec<D> Int16ToFloat(D d, Vec<Rebind<uint16_t, D>> v) {
  const RebindToSigned<D> di16;
  const Half<decltype(di16)> dhi16;
  const Rebind<int32_t, decltype(dhi16)> di32;
  const Rebind<float32_t, decltype(dhi16)> df32;
  const Half<decltype(d)> fh16;
  const auto vs = BitCast(di16, v);
  const auto lo = DemoteTo(fh16, ConvertTo(df32, PromoteLowerTo(di32, vs)));
  const auto hi = DemoteTo(fh16, ConvertTo(df32, PromoteUpperTo(di32, vs)));
  return Combine(d, hi, lo);
}

template <class D, HWY_IF_F16_D(D)>
HWY_API Vec<D> Int16ToFloat(D d, Vec<Rebind<int16_t, D>> v) {
  const Half<decltype(d)> dhi16;
  const Rebind<int32_t, decltype(dhi16)> di32;
  const Rebind<float32_t, decltype(dhi16)> df32;
  const Half<decltype(d)> fh16;
  const auto lo = DemoteTo(fh16, ConvertTo(df32, PromoteLowerTo(di32, v)));
  const auto hi = DemoteTo(fh16, ConvertTo(df32, PromoteUpperTo(di32, v)));
  return Combine(d, hi, lo);
}

template<class D, HWY_IF_U8_D(D), HWY_IF_LANES_D(D, 4)>
HWY_API VFromD<D> DemoteTo(D du, Vec<Rebind<float, D>> v) {
  const Rebind<uint32_t, decltype(du)> du32;
  return DemoteTo(du, ConvertTo(du32, v));
}

template<class D, HWY_IF_F32_D(D), HWY_IF_LANES_D(D, 4)>
HWY_API VFromD<D> PromoteTo(D df, Vec<Rebind<uint8_t, D>> v) {
  const Rebind<uint32_t, decltype(df)> du32;
  return ConvertTo(df, PromoteTo(du32, v));
}

#if !HWY_HAVE_FLOAT16 || HWY_TARGET == HWY_EMU128
template <class D, HWY_IF_F16_D(D)>
HWY_API Vec<D> ConvertTo(D d, Vec<Rebind<uint16_t, D>> v) {
  const RebindToSigned<D> di16;
  const Half<decltype(di16)> dhi16;
  const Rebind<int32_t, decltype(dhi16)> di32;
  const Rebind<float32_t, decltype(dhi16)> df32;
  const Half<decltype(d)> fh16;
  const auto vs = BitCast(di16, v);
  const auto lo = DemoteTo(fh16, ConvertTo(df32, PromoteLowerTo(di32, vs)));
  const auto hi = DemoteTo(fh16, ConvertTo(df32, PromoteUpperTo(di32, vs)));
  return Combine(d, hi, lo);
}

template <class D, HWY_IF_F16_D(D)>
HWY_API Vec<D> ConvertTo(D d, Vec<Rebind<int16_t, D>> v) {
  const Half<decltype(d)> dhi16;
  const Rebind<int32_t, decltype(dhi16)> di32;
  const Rebind<float32_t, decltype(dhi16)> df32;
  const Half<decltype(d)> fh16;
  const auto lo = DemoteTo(fh16, ConvertTo(df32, PromoteLowerTo(di32, v)));
  const auto hi = DemoteTo(fh16, ConvertTo(df32, PromoteUpperTo(di32, v)));
  return Combine(d, hi, lo);
}
#endif

}

namespace sparkyuv::HWY_NAMESPACE {
using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<typename D>
SPARKYUV_INLINE static void YUVToRGBLimited(D d,
                                            const Vec<D> Y, const Vec<D> U, const Vec<D> V,
                                            Vec<D> &R, Vec<D> &G, Vec<D> &B,
                                            const Vec<D> lumaCoeff,
                                            const Vec<D> crCoeff, const Vec<D> cbCoeff,
                                            const Vec<D> gCoeff1, const Vec<D> gCoeff2) {
  const auto luma = Mul(Y, lumaCoeff);
  R = MulAdd(crCoeff, V, luma);
  B = MulAdd(cbCoeff, U, luma);
  G = NegMulAdd(gCoeff2, U, NegMulAdd(gCoeff1, V, luma));
}

static void ComputeTransform(const float kr,
                             const float kb,
                             const float biasY,
                             const float biasUV,
                             const float rangeY,
                             const float rangeUV,
                             const float fullRangeRGB,
                             float &YR, float &YG, float &YB,
                             float &CbR, float &CbG, float &CbB,
                             float &CrR, float &CrG, float &CrB) {
  const float kg = 1.0f - kr - kb;
  if (kg == 0.f) {
    throw std::runtime_error("1.0f - kr - kg must not be 0");
  }

  YR = kr * rangeY / fullRangeRGB;
  YG = kg * rangeY / fullRangeRGB;
  YB = kb * rangeY / fullRangeRGB;

  CbR = 0.5f * kr / (1.f - kb) * rangeUV / fullRangeRGB;
  CbG = 0.5f * kg / (1.f - kb) * rangeUV / fullRangeRGB;
  CbB = 0.5f * rangeUV / fullRangeRGB;

  CrR = 0.5f * rangeUV / fullRangeRGB;
  CrG = 0.5f * kg / (1.f - kr) * rangeUV / fullRangeRGB;
  CrB = 0.5f * kb / (1.f - kr) * rangeUV / fullRangeRGB;
}

static void ComputeTransformIntegers(const float kr,
                                     const float kb,
                                     const float biasY,
                                     const float biasUV,
                                     const float rangeY,
                                     const float rangeUV,
                                     const float fullRangeRGB,
                                     const uint16_t precision,
                                     uint16_t &uYR, uint16_t &uYG, uint16_t &uYB,
                                     uint16_t &uCbR, uint16_t &uCbG, uint16_t &uCbB,
                                     uint16_t &uCrR, uint16_t &uCrG, uint16_t &uCrB) {
  float YR, YG, YB;
  float CbR, CbG, CbB;
  float CrR, CrG, CrB;

  const auto scale = static_cast<float>( 1 << precision );

  ComputeTransform(kr, kb, static_cast<float>(biasY), static_cast<float>(biasUV),
                   static_cast<float>(rangeY), static_cast<float>(rangeUV),
                   fullRangeRGB, YR, YG, YB, CbR, CbG, CbB, CrR, CrG, CrB);

  uYR = static_cast<uint16_t>(std::roundf(scale * YR));
  uYG = static_cast<uint16_t>(std::roundf(scale * YG));
  uYB = static_cast<uint16_t>(std::roundf(scale * YB));

  uCbR = static_cast<uint16_t>(std::roundf(scale * CbR));
  uCbG = static_cast<uint16_t>(std::roundf(scale * CbG));
  uCbB = static_cast<uint16_t>(std::roundf(scale * CbB));

  uCrR = static_cast<uint16_t>(std::roundf(scale * CrR));
  uCrG = static_cast<uint16_t>(std::roundf(scale * CrG));
  uCrB = static_cast<uint16_t>(std::roundf(scale * CrB));
}

static void ComputeInverseTransform(const float kr,
                                    const float kb,
                                    const float rangeHigh,
                                    const float rangeLow,
                                    float &CrCoeff,
                                    float &CbCoeff,
                                    float &GCoeff1,
                                    float &GCoeff2) {
  const float range = rangeHigh / rangeLow;
  CrCoeff = (2.f * (1.f - kr)) * range;
  CbCoeff = (2.f * (1.f - kb)) * range;
  const float kg = 1.0f - kr - kb;
  if (kg == 0.f) {
    throw std::runtime_error("1.0f - kr - kg must not be 0");
  }
  GCoeff1 = (2 * ((1 - kr) * kr / kg)) * range;
  GCoeff2 = (2 * ((1 - kb) * kb / kg)) * range;
}

}
HWY_AFTER_NAMESPACE();

#endif