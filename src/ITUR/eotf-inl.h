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

#if defined(SPARKYUV_HWY_OETF_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_HWY_OETF_INL_H
#undef SPARKYUV_HWY_OETF_INL_H
#else
#define SPARKYUV_HWY_OETF_INL_H
#endif

#include "hwy/highway.h"
#include "../math/math-inl.h"

#define EOTF_INLINE inline __attribute__((flatten))

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

static const float betaRec2020 = 0.018053968510807f;
static const float alphaRec2020 = 1.09929682680944f;

using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

EOTF_INLINE float PQEotf(float v, const float sdrReferencePoint) {
  float o = v;
  v = std::max(0.0f, v);
  const float m1 = (2610.0f / 4096.0f) / 4.0f;
  const float m2 = (2523.0f / 4096.0f) * 128.0f;
  const float c1 = 3424.0f / 4096.0f;
  const float c2 = (2413.0f / 4096.0f) * 32.0f;
  const float c3 = (2392.0f / 4096.0f) * 32.0f;
  float p = ::powf(v, 1.0f / m2);
  v = ::powf(std::max(p - c1, 0.0f) / (c2 - c3 * p), 1.0f / m1);
  v *= 10000.0f / sdrReferencePoint;
  return std::copysignf(v, o);
}

template<class D, class V = Vec<D>, HWY_IF_F32_D(D)>
EOTF_INLINE static V PQOetf(D d, V linear) {
  linear = Clamp(Mul(linear, Set(d, 203.f / 10000.f)), Zero(d), Set(d, 1.0f));
  const V powLinear = Pow(d, linear, Set(d, 0.1593017578125f));
  const V num = MulSub(Set(d, 0.1640625f), powLinear, Set(d, 0.1640625f));
  const V den = MulAdd(Set(d, 18.6875f), powLinear, Set(d, 1.0f));
  return Pow(d, Add(Div(num, den), Set(d, 1.0f)), Set(d, 78.84375f));
}

template<class D, class V = Vec<D>, HWY_IF_F16_D(D)>
EOTF_INLINE static V PQOetf(D d, V linear) {
  linear = Clamp(Mul(linear, Set(d, hwy::F16FromF32(203.f / 10000.f))), Zero(d), Set(d, hwy::F16FromF32(1.0f)));
  const V powLinear = Pow(d, linear, Set(d, hwy::F16FromF32(0.1593017578125f)));
  const V num = MulSub(Set(d, hwy::F16FromF32(0.1640625f)), powLinear, Set(d, hwy::F16FromF32(0.1640625f)));
  const V den = MulAdd(Set(d, hwy::F16FromF32(18.6875f)), powLinear, Set(d, 1.0f));
  return Pow(d, Add(Div(num, den), Set(d, hwy::F16FromF32(1.0f))), Set(d, hwy::F16FromF32(78.84375f)));
}

static float PQOetf(float linear) {
  if (linear > 0.0f) {
    // Scale from extended SDR range to [0.0, 1.0].
    linear = std::clamp(linear * 203.f / 10000.f, 0.0f, 1.0f);
    const float powLinear = powf(linear, 0.1593017578125f);
    const float num = 0.1640625f * powLinear - 0.1640625f;
    const float den = 1.0f + 18.6875f * powLinear;
    return powf(1.0f + num / den, 78.84375f);
  } else {
    return 0.0f;
  }
}

static float Curve470MEotf(float gamma) {
  return ::powf(std::clamp(gamma, 0.0f, 1.0f), 2.2f);
}

static float Curve470MOetf(float linear) {
  return ::powf(std::clamp(linear, 0.0f, 1.0f), 1.0f / 2.2f);
}

static float Curve470BGEotf(float gamma) {
  return ::powf(std::clamp(gamma, 0.0f, 1.0f), 2.8f);
}

static float Curve470BGOetf(float linear) {
  return ::powf(std::clamp(linear, 0.0f, 1.0f), 1.0f / 2.8f);
}

static float HLGOetf(float linear) {
  // Scale from extended SDR range to [0.0, 1.0].
  linear = std::clamp(linear * 203.f / 1000.f, 0.0f, 1.0f);
  // Inverse OOTF followed by OETF see Table 5 and Note 5i in ITU-R BT.2100-2 page 7-8.
  linear = ::powf(linear, 1.0f / 1.2f);
  if (linear < 0.0f) {
    return 0.0f;
  } else if (linear <= (1.0f / 12.0f)) {
    return ::sqrtf(3.0f * linear);
  } else {
    return 0.17883277f * logf(12.0f * linear - 0.28466892f) + 0.55991073f;
  }
}

template<class D, typename V>
HWY_API VFromD<D> HLGOetf(D d, V v) {
  for (size_t i = 0; i < MaxLanes(d); ++i) {
    v.raw[i] = HLGOetf(v.raw[i]);
  }
  return v;
}

using hwy::HWY_NAMESPACE::Zero;
using hwy::HWY_NAMESPACE::Vec;
using hwy::HWY_NAMESPACE::TFromD;

template<class DF, class V, HWY_IF_FLOAT(TFromD<DF>)>
static V Curve470MEotf(DF d, V gamma) {
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.0f);
  const auto power = Set(d, 2.2f);
  return sparkyuv::HWY_NAMESPACE::Pow(d, Clamp(gamma, zeros, ones), power);
}

template<class DF, class V, HWY_IF_F32_D(DF)>
static V Curve470BGEotf(DF d, V gamma) {
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.0f);
  const auto power = Set(d, 2.8f);
  return sparkyuv::HWY_NAMESPACE::Pow(d, Clamp(gamma, zeros, ones), power);
}

template<class DF, class V, HWY_IF_F16_D(DF)>
static V Curve470BGEotf(DF d, V gamma) {
  const auto zeros = Zero(d);
  const auto ones = Set(d, hwy::F16FromF32(1.0f));
  const auto power = Set(d, hwy::F16FromF32(2.8f));
  return sparkyuv::HWY_NAMESPACE::Pow(d, Clamp(gamma, zeros, ones), power);
}

template<class DF, class V, HWY_IF_F32_D(DF)>
static V Curve470MOetf(DF d, V gamma) {
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.0f);
  const auto power = Set(d, 1.0f / 2.2f);
  return sparkyuv::HWY_NAMESPACE::Pow(d, Clamp(gamma, zeros, ones), power);
}

template<class DF, class V, HWY_IF_F16_D(DF)>
static V Curve470MOetf(DF d, V gamma) {
  const auto zeros = Zero(d);
  const auto ones = Set(d, hwy::F16FromF32(1.0f));
  const auto power = Set(d, hwy::F16FromF32(1.0f / 2.2f));
  return sparkyuv::HWY_NAMESPACE::Pow(d, Clamp(gamma, zeros, ones), power);
}

template<class DF, class V, HWY_IF_F32_D(DF)>
static V Curve470BGOetf(DF d, V gamma) {
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.0f);
  const auto power = Set(d, 1.0f / 2.8f);
  return sparkyuv::HWY_NAMESPACE::Pow(d, Clamp(gamma, zeros, ones), power);
}

template<class DF, class V, HWY_IF_F16_D(DF)>
static V Curve470BGOetf(DF d, V gamma) {
  const auto zeros = Zero(d);
  const auto ones = Set(d, hwy::F16FromF32(1.0f));
  const auto power = Set(d, hwy::F16FromF32(1.0f / 2.8f));
  return sparkyuv::HWY_NAMESPACE::Pow(d, Clamp(gamma, zeros, ones), power);
}

template<class DF, class V, HWY_IF_F32_D(DF)>
EOTF_INLINE V
HLGEotf(const DF df, V v) {
  v = Max(Zero(df), v);
  using VF32 = Vec<decltype(df)>;
  using T = hwy::HWY_NAMESPACE::TFromD<DF>;
  const VF32 a = Set(df, static_cast<T>(0.17883277f));
  const VF32 b = Set(df, static_cast<T>(0.28466892f));
  const VF32 c = Set(df, static_cast<T>(0.55991073f));
  const VF32 mm = Set(df, static_cast<T>(0.5f));
  const VF32 inversed3 = ApproximateReciprocal(Set(df, static_cast<T>(3.f)));
  const VF32 inversed12 = ApproximateReciprocal(Set(df, static_cast<T>(12.0f)));
  const auto cmp = v < mm;
  auto branch1 = Mul(Mul(v, v), inversed3);
  auto branch2 = Mul(sparkyuv::HWY_NAMESPACE::Exp2f(df, Add(Div(Sub(v, c), a), b)),
                     inversed12);
  return IfThenElse(cmp, branch1, branch2);
}

template<class DF, class V, HWY_IF_F16_D(DF)>
EOTF_INLINE V
HLGEotf(const DF df, V v) {
  v = Max(Zero(df), v);
  using VF32 = Vec<decltype(df)>;
  using T = hwy::HWY_NAMESPACE::TFromD<DF>;
  const VF32 a = Set(df, static_cast<T>(0.17883277f));
  const VF32 b = Set(df, static_cast<T>(0.28466892f));
  const VF32 c = Set(df, static_cast<T>(0.55991073f));
  const VF32 mm = Set(df, static_cast<T>(0.5f));
  const VF32 inversed3 = Set(df, static_cast<T>(1.f / 3.f));
  const VF32 inversed12 = Set(df, static_cast<T>(1.f / 12.0f));
  const auto cmp = v < mm;
  auto branch1 = Mul(Mul(v, v), inversed3);
  auto branch2 = Mul(sparkyuv::HWY_NAMESPACE::Exp2f(df, Add(Div(Sub(v, c), a), b)),
                     inversed12);
  return IfThenElse(cmp, branch1, branch2);
}

template<class D, class V, HWY_IF_FLOAT(TFromD<D>)>
EOTF_INLINE V PQEotf(const D df, V v, const TFromD<D> sdrReferencePoint) {
  const V zeros = Zero(df);
  v = Max(zeros, v);
  using T = hwy::HWY_NAMESPACE::TFromD<D>;
  const T m1 = static_cast<T>((2610.0f / 4096.0f) / 4.0f);
  const T m2 = static_cast<T>((2523.0f / 4096.0f) * 128.0f);
  const V c1 = Set(df, static_cast<T>(3424.0f / 4096.0f));
  const V c2 = Set(df, static_cast<T>((2413.0f / 4096.0f) * 32.0f));
  const V c3 = Set(df, static_cast<T>((2392.0f / 4096.0f) * 32.0f));
  const V p1Power = Set(df, static_cast<T>(1.0f / m2));
  const V p2Power = Set(df, static_cast<T>(1.0f / m1));
  const V p3Power = Set(df, static_cast<T>(10000.0f / sdrReferencePoint));
  const V p = sparkyuv::HWY_NAMESPACE::Pow(df, v, p1Power);
  v = sparkyuv::HWY_NAMESPACE::Pow(df, Div(Max(Sub(p, c1), zeros), Sub(c2, Mul(c3, p))),
                                   p2Power);
  v = Mul(v, p3Power);
  return v;
}

template<class D, typename V = Vec<D>, HWY_IF_F32_D(D)>
EOTF_INLINE V Rec709Oetf(const D df, V value) {
  const auto minCutOff = Set(df, static_cast<TFromD<D>>(0.018053968510807f));
  const auto minPowValue = Set(df, static_cast<TFromD<D>>(4.5f));
  const auto lo = Mul(value, minPowValue);
  const auto zeros = Zero(df);
  const auto ones = Set(df, 1.0f);
  const auto subValue = Set(df, static_cast<TFromD<D>>(0.09929682680944f));
  const auto scalePower = Set(df, static_cast<TFromD<D>>(1.09929682680944f));
  const auto pwrValue = Set(df, static_cast<TFromD<D>>(0.45f));
  const auto ho = MulSub(sparkyuv::HWY_NAMESPACE::Pow(df, value, pwrValue), scalePower, subValue);
  auto Lc = IfThenElse(And(value < minCutOff, value >= zeros), lo, zeros);
  Lc = IfThenZeroElse(value < zeros, Lc);
  Lc = IfThenElse(And(value >= minCutOff, value <= ones), ho, Lc);
  Lc = IfThenElse(value > ones, ones, Lc);
  return Lc;
}

template<class D, typename V = Vec<D>, HWY_IF_F16_D(D)>
EOTF_INLINE V Rec709Oetf(const D df, V value) {
  const auto minCutOff = Set(df, hwy::F16FromF32(0.018053968510807f));
  const auto minPowValue = Set(df, hwy::F16FromF32(4.5f));
  const auto lo = Mul(value, minPowValue);
  const auto zeros = Zero(df);
  const auto ones = Set(df, hwy::F16FromF32(1.0f));
  const auto subValue = Set(df, hwy::F16FromF32(0.09929682680944f));
  const auto scalePower = Set(df, hwy::F16FromF32(1.09929682680944f));
  const auto pwrValue = Set(df, hwy::F16FromF32(0.45f));
  const auto ho = MulSub(sparkyuv::HWY_NAMESPACE::Pow(df, value, pwrValue), scalePower, subValue);
  auto Lc = IfThenElse(And(value < minCutOff, value >= zeros), lo, zeros);
  Lc = IfThenZeroElse(value < zeros, Lc);
  Lc = IfThenElse(And(value >= minCutOff, value <= ones), ho, Lc);
  Lc = IfThenElse(value > ones, ones, Lc);
  return Lc;
}

EOTF_INLINE float SRGBEotf(float v) {
  if (v < 0.0f) {
    return 0.0f;
  } else if (v < 12.92f * 0.0030412825601275209f) {
    return v / 12.92f;
  } else if (v < 1.0f) {
    return ::powf((v + 0.0550107189475866f) / 1.0550107189475866f, 2.4f);
  } else {
    return 1.0f;
  }
}

EOTF_INLINE float Rec709Eotf(float v) {
  if (v < 0.f) {
    return 0.f;
  } else if (v < 4.5f * 0.018053968510807f) {
    return v / 4.5f;
  } else if (v < 1.f) {
    return ::powf((v + 0.09929682680944f) / 1.09929682680944f, 1.f / 0.45f);
  }
  return 1.f;
}

EOTF_INLINE float Rec709Oetf(float linear) {
  if (linear < 0.f) {
    return 0.f;
  } else if (linear < 0.018053968510807f) {
    return linear * 4.5f;
  } else if (linear < 1.f) {
    return 1.09929682680944f * ::powf(linear, 0.45f) - 0.09929682680944f;
  }
  return 1.f;
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V SRGBEotf(D d, V v) {
  const auto highCutOff = Set(d, static_cast<T>(1.0f));
  const auto zeros = Zero(d);
  const auto lowerValueThreshold = Set(d, static_cast<T>(12.92f * 0.0030412825601275209f));
  const auto lowValueDivider = Set(d, static_cast<T>(1.0f) / static_cast<T>(12.92f));
  const auto lowMask = v <= lowerValueThreshold;
  const auto lowValue = Mul(v, lowValueDivider);
  const auto powerStatic = Set(d, T(2.4f));
  const auto addStatic = Set(d, T(0.0550107189475866f));
  const auto scaleStatic = Set(d, static_cast<T>(1.f / 1.0550107189475866f));
  const auto highValue = Pow(d, Mul(Add(v, addStatic), scaleStatic), powerStatic);
  auto result = IfThenElse(And(lowMask, v >= zeros), lowValue, v);
  result = IfThenElse(And(v > lowerValueThreshold, v <= highCutOff), highValue, result);
  result = IfThenElse(v > highCutOff, highCutOff, result);
  result = IfThenElse(v < zeros, zeros, result);
  return result;
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V SRGBEotf(D d, V v) {
  const auto highCutOff = Set(d, hwy::F16FromF32(1.0f));
  const auto zeros = Zero(d);
  const auto lowerValueThreshold = Set(d, hwy::F16FromF32(12.92f * 0.0030412825601275209f));
  const auto lowValueDivider = Set(d, hwy::F16FromF32(1.0f / 12.92f));
  const auto lowMask = v <= lowerValueThreshold;
  const auto lowValue = Mul(v, lowValueDivider);
  const auto powerStatic = Set(d, hwy::F16FromF32(2.4f));
  const auto addStatic = Set(d, hwy::F16FromF32(0.0550107189475866f));
  const auto scaleStatic = Set(d, hwy::F16FromF32(1.f / 1.0550107189475866f));
  const auto highValue = Pow(d, Mul(Add(v, addStatic), scaleStatic), powerStatic);
  auto result = IfThenElse(And(lowMask, v >= zeros), lowValue, v);
  result = IfThenElse(And(v > lowerValueThreshold, v <= highCutOff), highValue, result);
  result = IfThenElse(v > highCutOff, highCutOff, result);
  result = IfThenElse(v < zeros, zeros, result);
  return result;
}

EOTF_INLINE float TransferFunction(float v,
                                   const float c0,
                                   const float c1,
                                   const float c2,
                                   const float c3,
                                   const float cutoff,
                                   const float gamma,
                                   const float c4,
                                   const float c5) {
  if (v < 0) {
    return 0.f;
  } else if (v >= cutoff) {
    return c0 * ::powf(c1 * v + c2, gamma) + c3;
  } else if (v < 1) {
    return c4 * v + c5;
  } else {
    return 1.f;
  }
}

EOTF_INLINE float SRGBOetf(const float linear) {
  if (linear < 0.0f) {
    return 0.0f;
  } else if (linear < 0.0030412825601275209f) {
    return linear * 12.92f;
  } else if (linear < 1.0f) {
    return 1.0550107189475866f * ::powf(linear, 1.0f / 2.4f) - 0.0550107189475866f;
  } else {
    return 1.0f;
  }
}

template<class D, typename V = Vec<D>, HWY_IF_F32_D(D)>
EOTF_INLINE V SRGBOetf(const D df, V v) {
  const auto zeros = Zero(df);
  const auto highCutOff = Set(df, static_cast<TFromD<D>>(1.0f));
  const auto minCutOff = Set(df, static_cast<TFromD<D>>(0.0030412825601275209f));
  const auto minPowValue = Set(df, static_cast<TFromD<D>>(12.92f));
  const auto lowValue = Mul(v, minPowValue);
  const auto subValue = Set(df, static_cast<TFromD<D>>(0.0550107189475866f));
  const auto scalePower = Set(df, static_cast<TFromD<D>>(1.0550107189475866f));
  const auto pwrValue = Set(df, static_cast<TFromD<D>>(1.0f / 2.4f));
  const auto highValue = MulSub(sparkyuv::HWY_NAMESPACE::Pow(df, v, pwrValue), scalePower, subValue);
  auto result = IfThenElse(And(v <= minCutOff, v >= zeros), lowValue, v);
  result = IfThenElse(And(v > minCutOff, v <= highCutOff), highValue, result);
  result = IfThenElse(v > highCutOff, highCutOff, result);
  result = IfThenZeroElse(v < zeros, result);
  return result;
}

template<class D, typename V = Vec<D>, HWY_IF_F16_D(D)>
EOTF_INLINE V SRGBOetf(const D df, V v) {
  const auto zeros = Zero(df);
  const auto highCutOff = Set(df, hwy::F16FromF32(1.0f));
  const auto minCutOff = Set(df, hwy::F16FromF32(0.0030412825601275209f));
  const auto minPowValue = Set(df, hwy::F16FromF32(12.92f));
  const auto lowValue = Mul(v, minPowValue);
  const auto subValue = Set(df, hwy::F16FromF32(0.0550107189475866f));
  const auto scalePower = Set(df, hwy::F16FromF32(1.0550107189475866f));
  const auto pwrValue = Set(df, hwy::F16FromF32(1.0f / 2.4f));
  const auto highValue = MulSub(sparkyuv::HWY_NAMESPACE::Pow(df, v, pwrValue), scalePower, subValue);
  auto result = IfThenElse(And(v <= minCutOff, v >= zeros), lowValue, v);
  result = IfThenElse(And(v > minCutOff, v <= highCutOff), highValue, result);
  result = IfThenElse(v > highCutOff, highCutOff, result);
  result = IfThenZeroElse(v < zeros, result);
  return result;
}

template<class D, typename V = Vec<D>, HWY_IF_F32_D(D)>
EOTF_INLINE V SMPTE428Eotf(const D df, V value) {
  const auto scale = Set(df, static_cast<const TFromD<D>>(1.f / 0.91655527974030934f));
  const auto twoPoint6 = Set(df, static_cast<TFromD<D>>(2.6f));
  const auto zeros = Zero(df);
  return Mul(sparkyuv::HWY_NAMESPACE::Pow(df, Max(value, zeros), twoPoint6), scale);
}

template<class D, typename V = Vec<D>, HWY_IF_F16_D(D)>
EOTF_INLINE V SMPTE428Eotf(const D df, V value) {
  const auto scale = Set(df, hwy::F16FromF32(1.f / 0.91655527974030934f));
  const auto twoPoint6 = Set(df, hwy::F16FromF32(2.6f));
  const auto zeros = Zero(df);
  return Mul(sparkyuv::HWY_NAMESPACE::Pow(df, Max(value, zeros), twoPoint6), scale);
}

EOTF_INLINE float SMPTE428Eotf(const float value) {
  return ::powf(std::max(value, 0.f), 2.6f) / 0.91655527974030934f;
}

EOTF_INLINE float HLGEotf(float v) {
  v = std::max(0.0f, v);
  constexpr float a = 0.17883277f;
  constexpr float b = 0.28466892f;
  constexpr float c = 0.55991073f;
  if (v <= 0.5f)
    v = v * v / 3.0f;
  else
    v = (::expf((v - c) / a) + b) / 12.f;
  return v;
}

template<class D, typename T = Vec<D>, HWY_IF_FLOAT(TFromD<D>)>
EOTF_INLINE T gammaOtf(const D d, T color, TFromD<D> gamma) {
  const auto pw = Set(d, gamma);
  return sparkyuv::HWY_NAMESPACE::Pow(d, color, pw);
}

EOTF_INLINE float dciP3PQGammaCorrection(float linear) {
  return ::powf(linear, 1.f / 2.6f);
}

EOTF_INLINE float gammaOtf(float linear, const float gamma) {
  return ::powf(linear, gamma);
}

template<class D, typename T = Vec<D>, HWY_IF_F32_D(D)>
EOTF_INLINE T Rec601Eotf(const D d, T intensity) {
  const auto lowCutOff = Set(d, 4.5f * 0.018053968510807f);
  const auto fourAnd5 = Set(d, 1.f / 4.5f);
  const auto lowCutOffMask = intensity < lowCutOff;
  const auto tressPassCutOffMask = intensity >= lowCutOff;
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.f);
  const auto lowValues = Mul(intensity, fourAnd5);

  const auto addComp = Set(d, 0.09929682680944f);
  const auto div1099 = Set(d, 1.f / 1.09929682680944f);
  const auto pwScale = Set(d, 1.0f / 0.45f);

  const auto high = sparkyuv::HWY_NAMESPACE::Pow(d, Mul(Add(intensity, addComp), div1099), pwScale);
  auto Lc = IfThenElseZero(And(lowCutOffMask, intensity >= zeros), lowValues);
  Lc = IfThenZeroElse(intensity < zeros, Lc);
  Lc = IfThenElse(And(tressPassCutOffMask, intensity <= ones), high, Lc);
  Lc = IfThenElse(intensity > ones, ones, Lc);
  return Lc;
}

template<class D, typename T = Vec<D>, HWY_IF_F16_D(D)>
EOTF_INLINE T Rec601Eotf(const D d, T intensity) {
  const auto lowCutOff = Set(d, hwy::F16FromF32(4.5f * 0.018053968510807f));
  const auto fourAnd5 = Set(d, hwy::F16FromF32(1.f / 4.5f));
  const auto lowCutOffMask = intensity < lowCutOff;
  const auto tressPassCutOffMask = intensity >= lowCutOff;
  const auto zeros = Zero(d);
  const auto ones = Set(d, hwy::F16FromF32(1.f));
  const auto lowValues = Mul(intensity, fourAnd5);

  const auto addComp = Set(d, hwy::F16FromF32(0.09929682680944f));
  const auto div1099 = Set(d, hwy::F16FromF32(1.f / 1.09929682680944f));
  const auto pwScale = Set(d, hwy::F16FromF32(1.0f / 0.45f));

  const auto high = sparkyuv::HWY_NAMESPACE::Pow(d, Mul(Add(intensity, addComp), div1099), pwScale);
  auto Lc = IfThenElseZero(And(lowCutOffMask, intensity >= zeros), lowValues);
  Lc = IfThenZeroElse(intensity < zeros, Lc);
  Lc = IfThenElse(And(tressPassCutOffMask, intensity <= ones), high, Lc);
  Lc = IfThenElse(intensity > ones, ones, Lc);
  return Lc;
}

EOTF_INLINE float Rec601Eotf(float intensity) {
  if (intensity < 4.5f * 0.018053968510807f) {
    return intensity / 4.5f;
  } else {
    return ::powf((intensity + 0.099f) / 1.099f, 1.0f / 0.45f);
  }
}

EOTF_INLINE float Smpte240Eotf(float gamma) {
  if (gamma < 0.f) {
    return 0.f;
  } else if (gamma < 4.f * 0.022821585529445f) {
    return gamma / 4.f;
  } else if (gamma < 1.f) {
    return ::powf((gamma + 0.111572195921731f) / 1.111572195921731f, 1.f / 0.45f);
  }
  return 1.f;
}

template<class D, typename T = Vec<D>, HWY_IF_F32_D(D)>
EOTF_INLINE T Smpte240Eotf(const D d, T intensity) {
  const auto topValue = Set(d, 4.f * 0.022821585529445f);
  const auto lowValueDivider = Set(d, 1.f / 4.f);
  const auto lowMask = intensity < topValue;
  const auto lowValues = Mul(intensity, lowValueDivider);

  const auto addComp = Set(d, 0.111572195921731f);
  const auto div1099 = Set(d, 1.111572195921731f);
  const auto pwScale = Set(d, 1.0f / 0.45f);

  const auto high = sparkyuv::HWY_NAMESPACE::Pow(d, Div(Add(intensity, addComp), div1099),
                                                 pwScale);
  return IfThenElse(lowMask, lowValues, high);
}

template<class D, typename T = Vec<D>, HWY_IF_F16_D(D)>
EOTF_INLINE T Smpte240Eotf(const D d, T intensity) {
  const auto topValue = Set(d, hwy::F16FromF32(4.f * 0.022821585529445f));
  const auto lowValueDivider = Set(d, hwy::F16FromF32(1.f / 4.f));
  const auto lowMask = intensity < topValue;
  const auto lowValues = Mul(intensity, lowValueDivider);

  const auto addComp = Set(d, hwy::F16FromF32(0.111572195921731f));
  const auto div1099 = Set(d, hwy::F16FromF32(1.111572195921731f));
  const auto pwScale = Set(d, hwy::F16FromF32(1.0f / 0.45f));

  const auto high = sparkyuv::HWY_NAMESPACE::Pow(d, Div(Add(intensity, addComp), div1099),
                                                 pwScale);
  return IfThenElse(lowMask, lowValues, high);
}

EOTF_INLINE float Smpte240Oetf(float linear) {
  if (linear < 0.f) {
    return 0.f;
  } else if (linear < 0.022821585529445f) {
    return linear * 4.f;
  } else if (linear < 1.f) {
    return 1.111572195921731f * ::powf(linear, 0.45f) - 0.111572195921731f;
  }
  return 1.f;
}

template<class D, typename T = Vec<D>, HWY_IF_F32_D(D)>
EOTF_INLINE T Smpte240Oetf(const D d, T intensity) {
  const auto lowCutOff = Set(d, 0.022821585529445f);
  const auto fourAnd5 = Set(d, 4.f);
  const auto lowCutOffMask = intensity < lowCutOff;
  const auto tressPassCutOffMask = intensity >= lowCutOff;
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.f);
  const auto lowValues = Mul(intensity, fourAnd5);

  const auto addComp = Set(d, 1.111572195921731f);
  const auto pwScale = Set(d, 0.45f);

  const auto high = Sub(Mul(sparkyuv::HWY_NAMESPACE::Pow(d, intensity, pwScale), addComp), addComp);
  auto Lc = IfThenElseZero(And(lowCutOffMask, intensity >= zeros), lowValues);
  Lc = IfThenZeroElse(intensity < zeros, Lc);
  Lc = IfThenElse(And(tressPassCutOffMask, intensity <= ones), high, Lc);
  Lc = IfThenElse(intensity > ones, ones, Lc);
  return Lc;
}

template<class D, typename T = Vec<D>, HWY_IF_F16_D(D)>
EOTF_INLINE T Smpte240Oetf(const D d, T intensity) {
  const auto lowCutOff = Set(d, hwy::F16FromF32(0.022821585529445f));
  const auto fourAnd5 = Set(d, hwy::F16FromF32(4.f));
  const auto lowCutOffMask = intensity < lowCutOff;
  const auto tressPassCutOffMask = intensity >= lowCutOff;
  const auto zeros = Zero(d);
  const auto ones = Set(d, hwy::F16FromF32(1.f));
  const auto lowValues = Mul(intensity, fourAnd5);

  const auto addComp = Set(d, hwy::F16FromF32(1.111572195921731f));
  const auto pwScale = Set(d, hwy::F16FromF32(0.45f));

  const auto high = Sub(Mul(sparkyuv::HWY_NAMESPACE::Pow(d, intensity, pwScale), addComp), addComp);
  auto Lc = IfThenElseZero(And(lowCutOffMask, intensity >= zeros), lowValues);
  Lc = IfThenZeroElse(intensity < zeros, Lc);
  Lc = IfThenElse(And(tressPassCutOffMask, intensity <= ones), high, Lc);
  Lc = IfThenElse(intensity > ones, ones, Lc);
  return Lc;
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Rec709Eotf(D d, V v) {
  return Rec601Eotf(d, v);
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Rec709Eotf(D d, V v) {
  return Rec601Eotf(d, v);
}

EOTF_INLINE float Log100Eotf(float gamma) {
  // The function is non-bijective so choose the middle of [0, 0.01].
  const float mid_interval = 0.01f / 2.f;
  return (gamma <= 0.0f) ? mid_interval
                         : ::powf(10.0f, 2.f * (std::min(gamma, 1.f) - 1.0f));
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Eotf(D d, V v) {
  // The function is non-bijective so choose the middle of [0, 0.01].
  const auto midInterval = Set(d, 0.01f / 2.f);
  const auto zeros = Zero(d);
  const auto zerosMask = v > zeros;
  const auto ones = Set(d, 1.f);
  const auto twos = Set(d, 2.f);
  const auto tens = Set(d, 10.f);
  const auto highPart = sparkyuv::HWY_NAMESPACE::Pow(d, tens,
                                                     Add(twos, Sub(Min(v, ones), ones)));
  return IfThenElseZero(zerosMask, highPart);
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Eotf(D d, V v) {
  // The function is non-bijective so choose the middle of [0, 0.01].
  const auto midInterval = Set(d, hwy::F16FromF32(0.01f / 2.f));
  const auto zeros = Zero(d);
  const auto zerosMask = v > zeros;
  const auto ones = Set(d, hwy::F16FromF32(1.f));
  const auto twos = Set(d, hwy::F16FromF32(2.f));
  const auto tens = Set(d, hwy::F16FromF32(10.f));
  const auto highPart = sparkyuv::HWY_NAMESPACE::Pow(d, tens,
                                                     Add(twos, Sub(Min(v, ones), ones)));
  return IfThenElseZero(zerosMask, highPart);
}

EOTF_INLINE float Log100Oetf(float linear) {
  return (linear < 0.01f) ? 0.0f : 1.0f + ::log10f(std::min(linear, 1.f)) * 0.5f;
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Oetf(D d, V v) {
  const auto cutOff = Set(d, 0.01f);
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.f);
  const auto scale = Set(d, 0.5f);
  auto x = IfThenElse(v < cutOff, ones, v);
  return Add(ones, Mul(Log10(d, Min(x, ones)), scale));
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Oetf(D d, V v) {
  const auto cutOff = Set(d, hwy::F16FromF32(0.01f));
  const auto zeros = Zero(d);
  const auto ones = Set(d, hwy::F16FromF32(1.f));
  const auto scale = Set(d, hwy::F16FromF32(0.5f));
  auto x = IfThenElse(v < cutOff, ones, v);
  return Add(ones, Mul(Log10(d, Min(x, ones)), scale));
}

EOTF_INLINE float Log100Sqrt10Eotf(float gamma) {
  // The function is non-bijective so choose the middle of [0, 0.00316227766f[.
  const float mid_interval = 0.00316227766f / 2.f;
  return (gamma <= 0.0f) ? mid_interval
                         : ::powf(10.0f, 2.5f * (std::min(gamma, 1.f) - 1.0f));
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Sqrt10Eotf(D d, V v) {
  // The function is non-bijective so choose the middle of [0, 0.01].
  const auto midInterval = Set(d, 0.00316227766f / 2.f);
  const auto zeros = Zero(d);
  const auto zerosMask = v > zeros;
  const auto ones = Set(d, 1.f);
  const auto twos = Set(d, 2.5f);
  const auto tens = Set(d, 10.f);
  const auto highPart = sparkyuv::HWY_NAMESPACE::Pow(d, tens,
                                                     Add(twos, Sub(Min(v, ones), ones)));
  return IfThenElseZero(zerosMask, highPart);
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Sqrt10Eotf(D d, V v) {
  // The function is non-bijective so choose the middle of [0, 0.01].
  const auto midInterval = Set(d, hwy::F16FromF32(0.00316227766f / 2.f));
  const auto zeros = Zero(d);
  const auto zerosMask = v > zeros;
  const auto ones = Set(d, hwy::F16FromF32(1.f));
  const auto twos = Set(d, hwy::F16FromF32(2.5f));
  const auto tens = Set(d, hwy::F16FromF32(10.f));
  const auto highPart = sparkyuv::HWY_NAMESPACE::Pow(d, tens,
                                                     Add(twos, Sub(Min(v, ones), ones)));
  return IfThenElseZero(zerosMask, highPart);
}

EOTF_INLINE float Log100Sqrt10Oetf(float linear) {
  return (linear < 0.00316227766f) ? 0.0f
                                   : 1.0f + ::log10f(std::min(linear, 1.f)) / 2.5f;
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Sqrt10Oetf(D d, V v) {
  const auto cutOff = Set(d, 0.00316227766f);
  const auto zeros = Zero(d);
  const auto ones = Set(d, 1.f);
  const auto scale = Set(d, 1.f / 2.5f);
  auto x = IfThenElse(v < cutOff, ones, v);
  return Add(ones, Mul(Log10(d, Min(x, ones)), scale));
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Log100Sqrt10Oetf(D d, V v) {
  const auto cutOff = Set(d, hwy::F16FromF32(0.00316227766f));
  const auto zeros = Zero(d);
  const auto ones = Set(d, hwy::F16FromF32(1.f));
  const auto scale = Set(d, hwy::F16FromF32(1.f / 2.5f));
  auto x = IfThenElse(v < cutOff, ones, v);
  return Add(ones, Mul(Log10(d, Min(x, ones)), scale));
}

EOTF_INLINE float Iec61966Eotf(float gamma) {
  if (gamma <= -4.5f * 0.018053968510807f) {
    return ::powf((-gamma + 0.09929682680944f) / -1.09929682680944f, 1.f / 0.45f);
  } else if (gamma < 4.5f * 0.018053968510807f) {
    return gamma / 4.5f;
  }
  return ::powf((gamma + 0.09929682680944f) / 1.09929682680944f, 1.f / 0.45f);
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Iec61966Eotf(D d, V v) {
  const auto lowerLimit = Set(d, 4.5f * 0.018053968510807f);
  const auto theLowestLimit = Neg(lowerLimit);
  const auto additionToLinear = Set(d, 0.09929682680944f);
  const auto sPower = Set(d, 1.f / 0.45f);
  const auto defScale = Set(d, 1.f / 4.5f);
  const auto linearDivider = ApproximateReciprocal(Set(d, 1.09929682680944f));

  const auto lowest = sparkyuv::HWY_NAMESPACE::Pow(d, Mul(Add(Neg(v), additionToLinear),
                                                          Neg(linearDivider)), sPower);
  const auto intmd = Mul(v, defScale);

  const auto highest = sparkyuv::HWY_NAMESPACE::Pow(d,
                                                    Mul(Add(v, additionToLinear), linearDivider),
                                                    sPower);
  const auto lowestMask = v <= theLowestLimit;
  const auto upperMask = v > lowerLimit;

  auto x = IfThenElse(lowestMask, lowest, v);
  x = IfThenElse(And(v > theLowestLimit, v < lowerLimit), Mul(v, defScale), x);
  x = IfThenElse(upperMask, highest, x);
  return x;
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Iec61966Eotf(D d, V v) {
  const auto lowerLimit = Set(d, hwy::F16FromF32(4.5f * 0.018053968510807f));
  const auto theLowestLimit = Neg(lowerLimit);
  const auto additionToLinear = Set(d, hwy::F16FromF32(0.09929682680944f));
  const auto sPower = Set(d, hwy::F16FromF32(1.f / 0.45f));
  const auto defScale = Set(d, hwy::F16FromF32(1.f / 4.5f));
  const auto linearDivider = Set(d, hwy::F16FromF32(1.f / 1.09929682680944f));

  const auto lowest = sparkyuv::HWY_NAMESPACE::Pow(d, Mul(Add(Neg(v), additionToLinear),
                                                          Neg(linearDivider)), sPower);
  const auto intmd = Mul(v, defScale);

  const auto highest = sparkyuv::HWY_NAMESPACE::Pow(d,
                                                    Mul(Add(v, additionToLinear), linearDivider),
                                                    sPower);
  const auto lowestMask = v <= theLowestLimit;
  const auto upperMask = v > lowerLimit;

  auto x = IfThenElse(lowestMask, lowest, v);
  x = IfThenElse(And(v > theLowestLimit, v < lowerLimit), Mul(v, defScale), x);
  x = IfThenElse(upperMask, highest, x);
  return x;
}

EOTF_INLINE float Iec61966Oetf(float linear) {
  if (linear <= -0.018053968510807f) {
    return -1.09929682680944f * ::powf(-linear, 0.45f) + 0.09929682680944f;
  } else if (linear < 0.018053968510807f) {
    return linear * 4.5f;
  }
  return 1.09929682680944f * ::powf(linear, 0.45f) - 0.09929682680944f;
}

template<class D, typename V = Vec<D>, HWY_IF_FLOAT(TFromD<D>)>
EOTF_INLINE V Iec61966Oetf(const D d, V value) {
  const auto minCutOff = Set(d, static_cast<TFromD<D>>(-0.018053968510807f));
  const auto midCutOff = Set(d, static_cast<TFromD<D>>(0.018053968510807f));
  const auto low = MulAdd(Set(d, -1.09929682680944f), Pow(d, Neg(value), Set(d, 0.5f)), Set(d, 0.09929682680944f));
  const auto mid = Mul(value, Set(d, 4.5f));
  const auto high = MulSub(Set(d, 1.09929682680944f), Pow(d, value, Set(d, 0.5f)), Set(d, 0.09929682680944f));
  auto x = IfThenElseZero(value <= minCutOff, low);
  x = IfThenElse(And(value < midCutOff, value > minCutOff), mid, x);
  x = IfThenElse(value > midCutOff, high, x);
  return x;
}

EOTF_INLINE float Bt1361Eotf(float gamma) {
  if (gamma < -0.25f) {
    return -0.25f;
  } else if (gamma < 0.f) {
    return ::powf((gamma - 0.02482420670236f) / -0.27482420670236f, 1.f / 0.45f) /
        -4.f;
  } else if (gamma < 4.5f * 0.018053968510807f) {
    return gamma / 4.5f;
  } else if (gamma < 1.f) {
    return ::powf((gamma + 0.09929682680944f) / 1.09929682680944f, 1.f / 0.45f);
  }
  return 1.f;
}

template<class D, HWY_IF_F32_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Bt1361Eotf(D d, V v) {
  // The function is non-bijective so choose the middle of [0, 0.01].
  const auto lowestPart = Set(d, -0.25f);
  const auto zeros = Zero(d);
  const auto zeroIntensityMod = Set(d, 0.09929682680944f);
  const auto dividerIntensity = Set(d, 1.09929682680944f);
  const auto dividerZeroIntensity = Set(d, -0.27482420670236f);
  const auto sPower = Set(d, 1.f / 0.45f);

  const auto imdScale = Set(d, 1.f / 4.5f);

  const auto imdThreshold = Set(d, 4.5f * 0.018053968510807f);

  const auto lowest = Div(sparkyuv::HWY_NAMESPACE::Pow(d, Div(Sub(v, dividerZeroIntensity),
                                                              dividerZeroIntensity), sPower),
                          Set(d, -4.f));

  const auto highest = sparkyuv::HWY_NAMESPACE::Pow(d, Div(Add(v, zeroIntensityMod),
                                                           zeroIntensityMod), sPower);

  const auto ones = Set(d, 1.f);

  auto x = IfThenElse(v >= ones, ones, v);
  x = IfThenElse(v >= imdThreshold, highest, x);
  x = IfThenElse(And(v < imdThreshold, v >= zeros), Div(v, imdThreshold), x);
  x = IfThenElse(And(v >= lowestPart, v < zeros), lowest, x);
  x = IfThenElse(v < lowestPart, lowestPart, x);
  return x;
}

template<class D, HWY_IF_F16_D(D), typename T = TFromD<D>, typename V = VFromD<D>>
EOTF_INLINE V Bt1361Eotf(D d, V v) {
  // The function is non-bijective so choose the middle of [0, 0.01].
  const auto lowestPart = Set(d, -0.25f);
  const auto zeros = Zero(d);
  const auto zeroIntensityMod = Set(d, 0.09929682680944f);
  const auto dividerIntensity = Set(d, 1.09929682680944f);
  const auto dividerZeroIntensity = Set(d, -0.27482420670236f);
  const auto sPower = Set(d, 1.f / 0.45f);

  const auto imdScale = Set(d, 1.f / 4.5f);

  const auto imdThreshold = Set(d, 4.5f * 0.018053968510807f);

  const auto lowest = Div(sparkyuv::HWY_NAMESPACE::Pow(d, Div(Sub(v, dividerZeroIntensity),
                                                              dividerZeroIntensity), sPower),
                          Set(d, -4.f));

  const auto highest = sparkyuv::HWY_NAMESPACE::Pow(d, Div(Add(v, zeroIntensityMod),
                                                           zeroIntensityMod), sPower);

  const auto ones = Set(d, 1.f);

  auto x = IfThenElse(v >= ones, ones, v);
  x = IfThenElse(v >= imdThreshold, highest, x);
  x = IfThenElse(And(v < imdThreshold, v >= zeros), Div(v, imdThreshold), x);
  x = IfThenElse(And(v >= lowestPart, v < zeros), lowest, x);
  x = IfThenElse(v < lowestPart, lowestPart, x);
  return x;
}

EOTF_INLINE float Bt1361Oetf(float linear) {
  if (linear < -0.25f) {
    return -0.25f;
  } else if (linear < 0.f) {
    return -0.27482420670236f * ::powf(-4.f * linear, 0.45f) + 0.02482420670236f;
  } else if (linear < 0.018053968510807f) {
    return linear * 4.5f;
  } else if (linear < 1.f) {
    return 1.09929682680944f * ::powf(linear, 0.45f) - 0.09929682680944f;
  }
  return 1.f;
}

template<class D, typename V = Vec<D>, HWY_IF_FLOAT(TFromD<D>)>
EOTF_INLINE V Bt1361Oetf(const D d, V value) {
  const auto minCutOff = Set(d, static_cast<TFromD<D>>(0.f));
  const auto midCutOff = Set(d, static_cast<TFromD<D>>(0.018053968510807f));
  const auto lowest = Set(d, -0.25f);
  const auto ones = Set(d, 1.f);
  const auto low =
      MulAdd(Set(d, -0.27482420670236f), Pow(d, Mul(value, Set(d, -4.5f)), Set(d, 0.45f)), Set(d, 0.02482420670236f));
  const auto mid = Mul(value, Set(d, 4.5f));
  const auto high = MulSub(Set(d, 1.09929682680944f), Pow(d, value, Set(d, 0.455f)), Set(d, 0.09929682680944f));
  auto x = IfThenElseZero(value < lowest, lowest);
  x = IfThenElse(And(value < minCutOff, value > lowest), low, x);
  x = IfThenElse(And(value < midCutOff, value > minCutOff), mid, x);
  x = IfThenElse(And(value > midCutOff, value < ones), high, x);
  x = IfThenElse(value > ones, ones, x);
  return x;
}

}
HWY_AFTER_NAMESPACE();
#endif