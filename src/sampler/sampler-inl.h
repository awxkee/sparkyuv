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

#if defined(SPARKYUV_SAMPLER_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_SAMPLER_INL_H
#undef SPARKYUV_SAMPLER_INL_H
#else
#define SPARKYUV_SAMPLER_INL_H
#endif

#include "hwy/highway.h"
#include "src/math/math-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
using hwy::HWY_NAMESPACE::Set;
using hwy::HWY_NAMESPACE::FixedTag;
using hwy::HWY_NAMESPACE::Vec;
using hwy::HWY_NAMESPACE::Abs;
using hwy::HWY_NAMESPACE::Mul;
using hwy::HWY_NAMESPACE::Div;
using hwy::HWY_NAMESPACE::Max;
using hwy::HWY_NAMESPACE::Min;
using hwy::HWY_NAMESPACE::Add;
using hwy::HWY_NAMESPACE::Zero;
using hwy::HWY_NAMESPACE::BitCast;
using hwy::HWY_NAMESPACE::ConvertTo;
using hwy::HWY_NAMESPACE::PromoteTo;
using hwy::HWY_NAMESPACE::DemoteTo;
using hwy::HWY_NAMESPACE::Combine;
using hwy::HWY_NAMESPACE::Rebind;
using hwy::HWY_NAMESPACE::Sub;
using hwy::HWY_NAMESPACE::LowerHalf;
using hwy::HWY_NAMESPACE::UpperHalf;
using hwy::HWY_NAMESPACE::LoadInterleaved4;
using hwy::HWY_NAMESPACE::StoreInterleaved4;
using hwy::HWY_NAMESPACE::IfThenZeroElse;
using hwy::float16_t;
using hwy::float32_t;

using hwy::HWY_NAMESPACE::NegMulAdd;
using hwy::HWY_NAMESPACE::MulAdd;
using hwy::HWY_NAMESPACE::IfThenElse;
using hwy::HWY_NAMESPACE::MulSub;
using hwy::HWY_NAMESPACE::ApproximateReciprocal;

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T
BCSplinePartOne(const D df, T x, const T B, const T C, const T tripled, const T doubled) {
  x = Abs(x);
  T mult = Set(df, 1.0f / 6.0f);
  T r1 = NegMulAdd(Set(df, 9), B, NegMulAdd(Set(df, 6.0), C, Set(df, 12)));
  T r2 = MulAdd(Set(df, 6), C, MulSub(Set(df, 12.0f), B, Set(df, 18.0f)));
  T r3 = NegMulAdd(Set(df, 2), B, Set(df, 6));
  return Mul(MulAdd(r1, tripled, MulAdd(r2, doubled, r3)), mult);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T
BCSplinePartTwo(const D df, T x, const T B, const T C, const T tripled, const T doubled) {
  x = Abs(x);
  T mult = Set(df, 1.0f / 6.0f);
  T r1 = MulSub(Set(df, -6.0f), C, B);
  T r2 = MulAdd(Set(df, 6.0), B, Mul(Set(df, 30), C));
  T r3 = MulSub(Set(df, -12), B, Mul(Set(df, 48), C));
  T r4 = MulAdd(Set(df, 8.0), B, Mul(Set(df, 24.0f), C));
  T rr = MulAdd(r1, tripled, MulAdd(r2, doubled, MulAdd(r3, x, r4)));
  return Mul(rr, mult);
}

template<class D, typename V = Vec<D>>
HWY_MATH_INLINE V BCSpline(const D df, V x, const V B, const V C) {
  x = Abs(x);
  const V zeros = Zero(df);
  const V ones = Set(df, 1.0);
  const V two = Set(df, 2.0);
  const V doubled = Mul(x, x);
  const V tripled = Mul(doubled, x);
  auto setZeroMask = x > two;
  auto setP1Mask = x < ones;
  auto setP2Mask = x >= ones;
  V res = Zero(df);
  const V p1 = BCSplinePartOne(df, x, B, C, tripled, doubled);
  const V p2 = BCSplinePartTwo(df, x, B, C, tripled, doubled);
  res = IfThenElse(setP1Mask, p1, zeros);
  res = IfThenElse(setP2Mask, p2, res);
  res = IfThenElse(setZeroMask, zeros, res);
  return res;
}

using hwy::HWY_NAMESPACE::InsertLane;
using hwy::HWY_NAMESPACE::ExtractLane;
using hwy::HWY_NAMESPACE::LoadU;

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T MitchellNetravaliV(const D df, T d) {
  const T C = Set(df, 1.0 / 3.0);
  const T B = Set(df, 1.0 / 3.0);
  return BCSpline<D, T>(df, d, B, C);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T CubicHermiteV(const D df, T d) {
  const T C = Set(df, 0.0);
  const T B = Set(df, 0.0);
  return BCSpline<D, T>(df, d, B, C);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T CubicBSplineV(const D df, T d) {
  const T C = Set(df, 0.0);
  const T B = Set(df, 1.0);
  return BCSpline<D, T>(df, d, B, C);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T BiCubicSplineV(const D df, T x) {
  const hwy::HWY_NAMESPACE::TFromD<D> a = -0.5;
  const T aVec = Set(df, a);
  const T ones = Set(df, 1.0);
  const T two = Set(df, 2.0);
  const T three = Set(df, 3.0);
  const T four = Set(df, 4.0);
  const T five = Set(df, 5.0);
  const T eight = Set(df, 8.0);
  const T zeros = Zero(df);
  x = Abs(x);
  const auto zeroMask = x >= two;
  const auto partOneMask = x < ones;
  const T doubled = Mul(x, x);
  const T triplet = Mul(doubled, x);

  const T partOne = MulAdd(Add(two, aVec), triplet, NegMulAdd(Add(aVec, three), doubled, ones));
  const T fourA = Mul(four, aVec);
  const T eightA = Mul(eight, aVec);
  const T fiveA = Mul(five, aVec);
  const T partTwo = MulAdd(aVec, triplet,
                           NegMulAdd(fiveA, doubled,
                                     MulSub(eightA, x, fourA)));

  x = IfThenElse(partOneMask, partOne, partTwo);
  x = IfThenElse(zeroMask, zeros, x);

  return x;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T SimpleCubicV(const D df, T x) {
  x = Abs(x);
  const T zeros = Zero(df);
  const T ones = Set(df, 1.0);
  const T two = Set(df, 2.0);
  const T doubled = Mul(x, x);
  const T tripled = Mul(doubled, x);
  auto setZeroMask = x > two;
  auto setP1Mask = x < ones;
  auto setP2Mask = x >= ones;
  const T mSix = Set(df, 6.0f);
  const T sixScale = ApproximateReciprocal(mSix);
  T res = Zero(df);
  const T p1 = Mul(MulAdd(MulSub(Set(df, 3), x, mSix), Mul(x, x), Set(df, 4.0f)), sixScale);
  const T p2 = Mul(MulAdd(MulSub(Sub(mSix, x), x, Set(df, 12.0f)), x, Set(df, 8.0f)), sixScale);
  res = IfThenElse(setP1Mask, p1, zeros);
  res = IfThenElse(setP2Mask, p2, res);
  res = IfThenElse(setZeroMask, zeros, res);
  return res;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T sincV(const D d, T x) {
  const T ones = Set(d, 1);
  const T zeros = Zero(d);
  auto maskEqualToZero = x == zeros;
  T sine = hwy::HWY_NAMESPACE::Sin(d, x);
  x = IfThenElse(maskEqualToZero, ones, x);
  T result = Div(sine, x);
  result = IfThenElse(maskEqualToZero, ones, result);
  return result;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T LanczosWindowHWY(const D df, T x, const T a) {
  auto mask = Abs(x) >= a;
  T v = Mul(Set(df, M_PI), x);
  T r = Mul(sincV(df, v), sincV(df, Div(v, a)));
  return IfThenZeroElse(mask, r);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T CatmullRomV(const D df, T d) {
  const T C = Set(df, 0.0);
  const T B = Set(df, 0.5);
  return BCSpline<D, T>(df, d, B, C);
}

using hwy::HWY_NAMESPACE::Lerp;

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T Blerp(const D df, T c00, T c10, T c01, T c11, T tx, T ty) {
  return Lerp(df, Lerp(df, c00, c10, tx), Lerp(df, c01, c11, tx), ty);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T HannWindow(const D df, const T n, const float length) {
  const float size = length * 2;
  const T sizeV = Set(df, size);
  const T lengthV = Set(df, length);
  auto mask = Abs(n) > Set(df, length);
  const T piMulSize = Set(df, M_PI / size);
  T res = hwy::HWY_NAMESPACE::Cos(df, Mul(piMulSize, n));
  res = Mul(Mul(res, res), ApproximateReciprocal(sizeV));
  res = IfThenZeroElse(mask, res);
  return res;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T J1(const D df, T x) {
  T p = Set(df, 0.270112271089232341485679099e+4);
  T q = Set(df, 0.1e+1);

  const auto dX = Mul(x, x);

  p = MulAdd(p, dX, Set(df, -0.4695753530642995859767162166e+7));
  q = MulAdd(q, dX, Set(df, 0.1606931573481487801970916749e+4));

  p = MulAdd(p, dX, Set(df, 0.3413234182301700539091292655e+10));
  q = MulAdd(q, dX, Set(df, 0.1501793594998585505921097578e+7));

  p = MulAdd(p, dX, Set(df, -0.1322983480332126453125473247e+13));
  q = MulAdd(q, dX, Set(df, 0.1013863514358673989967045588e+10));

  p = MulAdd(p, dX, Set(df, 0.2908795263834775409737601689e+15));
  q = MulAdd(q, dX, Set(df, 0.5243710262167649715406728642e+12));

  p = MulAdd(p, dX, Set(df, -0.3588817569910106050743641413e+17));
  q = MulAdd(q, dX, Set(df, 0.2081661221307607351240184229e+15));

  p = MulAdd(p, dX, Set(df, 0.2316433580634002297931815435e+19));
  q = MulAdd(q, dX, Set(df, 0.6092061398917521746105196863e+17));

  p = MulAdd(p, dX, Set(df, -0.6672106568924916298020941484e+20));
  q = MulAdd(q, dX, Set(df, 0.1185770712190320999837113348e+20));

  p = MulAdd(p, dX, Set(df, 0.581199354001606143928050809e+21));
  q = MulAdd(q, dX, Set(df, 0.11623987080032122878585294e+22));

  const auto zeros = Zero(df);
  const auto ones = Set(df, 1.0f);
  q = IfThenElse(q == zeros, ones, q);

  return Div(p, q);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T Q1(const D df, T x) {
  static const float
      Pone[] = {
      0.3511751914303552822533318e+3,
      0.7210391804904475039280863e+3,
      0.4259873011654442389886993e+3,
      0.831898957673850827325226e+2,
      0.45681716295512267064405e+1,
      0.3532840052740123642735e-1
  },
      Qone[] = {
      0.74917374171809127714519505e+4,
      0.154141773392650970499848051e+5,
      0.91522317015169922705904727e+4,
      0.18111867005523513506724158e+4,
      0.1038187585462133728776636e+3,
      0.1e+1
  };

  T p = Set(df, Pone[5]);
  T q = Set(df, Qone[5]);

  const auto zeros = Zero(df);
  const auto ones = Set(df, 1.0f);

  const auto eights = Set(df, 8.0);

  x = IfThenElse(x == zeros, ones, x);

  const auto recX = Div(eights, x);

  const auto dX = Mul(recX, recX);

  for (int i = 4; i >= 0; i--) {
    p = MulAdd(p, dX, Set(df, Pone[i]));
    q = MulAdd(q, dX, Set(df, Pone[i]));
  }

  q = IfThenElse(q == zeros, ones, q);

  auto res = Div(p, q);
  res = IfThenElse(x == zeros, zeros, res);
  return res;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T P1(const D df, T x) {
  static const float
      Pone[] = {
      0.352246649133679798341724373e+5,
      0.62758845247161281269005675e+5,
      0.313539631109159574238669888e+5,
      0.49854832060594338434500455e+4,
      0.2111529182853962382105718e+3,
      0.12571716929145341558495e+1
  },
      Qone[] = {
      0.352246649133679798068390431e+5,
      0.626943469593560511888833731e+5,
      0.312404063819041039923015703e+5,
      0.4930396490181088979386097e+4,
      0.2030775189134759322293574e+3,
      0.1e+1
  };

  T p = Set(df, Pone[5]);
  T q = Set(df, Qone[5]);

  const auto zeros = Zero(df);
  const auto ones = Set(df, 1.0f);

  const auto eights = Set(df, 8.0);

  x = IfThenElse(x == zeros, ones, x);

  const auto recX = Div(eights, x);

  const auto dX = Mul(recX, recX);

  for (int i = 4; i >= 0; i--) {
    p = MulAdd(p, dX, Set(df, Pone[i]));
    q = MulAdd(q, dX, Set(df, Pone[i]));
  }

  q = IfThenElse(q == zeros, ones, q);

  auto res = Div(p, q);
  res = IfThenElse(x == zeros, zeros, res);
  return res;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T BesselOrderOne(const D df, T x) {
  auto p = x;
  x = Abs(x);
  const auto minZValue = Set(df, 1e-8);
  auto zerosMask = x < minZValue;
  auto res = Mul(J1(df, x), p);
  const auto zeros = Zero(df);
  res = IfThenElse(zerosMask, zeros, res);
  return res;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T jinc(const D d, T x) {
  const T ones = Set(d, 1);
  const T zeros = Zero(d);
  auto maskEqualToZero = x == zeros;
  const auto minZValue = Set(d, 1e-8);
  auto zerosMask = x < minZValue;
  x = IfThenElse(maskEqualToZero, ones, x);
  const T pi = Set(d, M_PI);
  T result = Div(BesselOrderOne(d, Mul(pi, x)), x);
  result = IfThenElse(maskEqualToZero, zeros, result);
  result = IfThenElse(zerosMask, Set(d, 0.5 * M_PI), result);
  return result;
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T LanczosJinc(const D df, T x, const T a) {
  auto mask = Abs(x) >= a;
  T v = Mul(Set(df, M_PI), x);
  T r = Mul(jinc(df, v), jinc(df, Div(v, a)));
  return IfThenZeroElse(mask, r);
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T Lanczos3Jinc(const D df, T x) {
  return LanczosJinc(df, x, Set(df, 3.0f));
}

template<class D, typename T = Vec<D>>
HWY_MATH_INLINE T Lanczos3Sinc(const D df, T x) {
  return LanczosWindowHWY(df, x, Set(df, 3.0f));
}
}
HWY_AFTER_NAMESPACE();

#endif