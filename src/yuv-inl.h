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

#if defined(SPARKYUV_YUV_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV_YUV_INL_H
#undef SPARKYUV_YUV_INL_H
#else
#define SPARKYUV_YUV_INL_H
#endif

#include "hwy/highway.h"
#include "sparkyuv-def.h"
#include "sparkyuv-internal.h"
#include <algorithm>
#include <cmath>

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
#if (HWY_ARCH_ARM_A64 && (HWY_TARGET == HWY_NEON || HWY_TARGET == HWY_NEON_WITHOUT_AES))
#define SPARKYUV_ALLOW_WIDE_MULL_ACCUMULATE 1
#endif

#if HWY_HAVE_FLOAT16 && (HWY_TARGET != HWY_SVE && HWY_TARGET != HWY_SVE2)
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

template<typename D, HWY_IF_U8_D(D), typename VH = Vec<Half<D>>>
VH GetOdd(D d, Vec<D> v) {
  const Half<D> dh;
  const Rebind<uint16_t, decltype(dh)> du16;
  return DemoteTo(dh, ShiftRight<8>(BitCast(du16, v)));
}

template<typename D, HWY_IF_U8_D(D), typename VH = Vec<Half<D>>>
VH GetEven(D d, Vec<D> v) {
  const Half<D> dh;
  const Rebind<uint32_t, decltype(dh)> du16;
  const auto erase = Set(du16, static_cast<uint16_t>(0x00FF));
  return DemoteTo(dh, And(BitCast(du16, v), erase));
}

// Hack for HWY EMU128.

template<class D, HWY_IF_F16_D(D)>
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

template<class D, HWY_IF_F16_D(D)>
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

#if (HWY_TARGET == HWY_NEON || HWY_TARGET == HWY_NEON_WITHOUT_AES) && HWY_ARCH_ARM_A64

template<int n, class D, HWY_IF_U16_D(D), typename DN = Rebind<MakeNarrow<TFromD<D>>, D>, typename VN = Vec<DN>>
HWY_API VN ShiftRightNarrow(D /* tag */, Vec<D> v) {
  return Vec64<uint8_t>(vshrn_n_u16(v.raw, n));
}

template<int n, class D, HWY_IF_I32_D(D), typename DN = Rebind<int16_t, D>, typename VN = Vec<DN>>
HWY_API VN ShiftRightNarrow(D /* tag */, Vec128<TFromD<D>> v) {
  return Vec64<int16_t>(vshrn_n_s32(v.raw, n));
}

template<int n, class D, HWY_IF_U32_D(D), typename DN = Rebind<uint16_t, D>, typename VN = Vec<DN>>
HWY_API VN ShiftRightNarrow(D /* tag */, Vec<D> v) {
  return Vec64<uint16_t>(vshrn_n_u32(v.raw, n));
}

template<class D, HWY_IF_U8_D(D), typename DW = RepartitionToWide<D>, typename VW = Vec<DW>>
HWY_API VW AddWide(D /* tag */, Vec<D> v, Vec<D> x) {
  return Vec128<uint16_t>(vaddl_u8(v.raw, x.raw));
}

template<class D, HWY_IF_U8_D(D), typename DW = RepartitionToWide<D>, typename VW = Vec<DW>>
HWY_API VW AddHighWide(D /* tag */, Vec<D> v, Vec<D> x) {
  return Vec128<uint16_t>(vaddl_high_u8(v.raw, x.raw));
}

template<class D, HWY_IF_U8_D(D), typename DW = RepartitionToWide<D>, typename VW = Vec<DW>>
HWY_API VW WidenMulHigh(D /* tag */, Vec<D> v, Vec<D> x) {
  return Vec128<uint16_t>(vmull_high_u8(v.raw, x.raw));
}

template<class D, HWY_IF_U8_D(D), typename DW = Rebind<uint16_t, D>, typename VW = Vec<DW>>
HWY_API VW WidenMul(D /* tag */, Vec<D> v, Vec<D> x) {
  return Vec128<uint16_t>(vmull_u8(v.raw, x.raw));
}

/** Widen Mul Add **/

//template<class D, HWY_IF_U8_D(D)>
//HWY_API Vec128<uint16_t> WidenMulAdd(D /* tag */, Vec64<uint8_t> mul, Vec64<uint8_t> x, Vec128<uint16_t> add) {
//  return Vec128<uint16_t>(vmlal_u8(add.raw, mul.raw, x.raw));
//}
//
//template<class D, HWY_IF_I8_D(D)>
//HWY_API Vec128<int16_t> WidenMulAdd(D /* tag */, Vec64<int8_t> mul, Vec64<int8_t> x, Vec128<int16_t> add) {
//  return Vec128<int16_t>(vmlal_s8(add.raw, mul.raw, x.raw));
//}
//
//template<class D, HWY_IF_I16_D(D)>
//HWY_API Vec128<int32_t> WidenMulAdd(D /* tag */, Vec64<int16_t> mul, Vec64<int16_t> x, Vec128<int32_t> add) {
//  return Vec128<int32_t>(vmlal_s16(add.raw, mul.raw, x.raw));
//}
//
//template<class D, HWY_IF_U16_D(D)>
//HWY_API Vec128<uint32_t> WidenMulAdd(D /* tag */, Vec64<uint16_t> mul, Vec64<uint16_t> x, Vec128<uint32_t> add) {
//  return Vec128<uint32_t>(vmlal_u16(add.raw, mul.raw, x.raw));
//}
//
//template<class D, HWY_IF_I32_D(D)>
//HWY_API Vec128<int64_t> WidenMulAdd(D /* tag */, Vec64<int32_t> mul, Vec64<int32_t> x, Vec128<int64_t> add) {
//  return Vec128<int64_t>(vmlal_s32(add.raw, mul.raw, x.raw));
//}
//
//template<class D, HWY_IF_U32_D(D)>
//HWY_API Vec128<uint64_t> WidenMulAdd(D /* tag */, Vec64<uint32_t> mul, Vec64<uint32_t> x, Vec128<uint64_t> add) {
//  return Vec128<uint64_t>(vmlal_u32(add.raw, mul.raw, x.raw));
//}

/** Widen High Mul Add **/

//template<class D, HWY_IF_U32_D(D)>
//HWY_API Vec128<uint64_t> WidenHighMulAdd(D /* tag */, Vec128<uint32_t> mul, Vec128<uint32_t> x, Vec128<uint64_t> add) {
//#if HWY_ARCH_ARM_A64
//  return Vec128<uint64_t>(vmlal_high_u32(add.raw, mul.raw, x.raw));
//#else
//  const Full64<uint32_t> dh;
//  return Vec128<uint64_t>(
//      vmlal_u32(add.raw, UpperHalf(dh, mul).raw, UpperHalf(dh, x).raw));
//#endif
//}
//
//template<class D, HWY_IF_I32_D(D)>
//HWY_API Vec128<int64_t> WidenHighMulAdd(D /* tag */, Vec128<int32_t> mul, Vec128<int32_t> x, Vec128<int64_t> add) {
//#if HWY_ARCH_ARM_A64
//  return Vec128<int64_t>(vmlal_high_s32(add.raw, mul.raw, x.raw));
//#else
//  const Full64<int32_t> dh;
//  return Vec128<int64_t>(
//      vmlal_s32(add.raw, UpperHalf(dh, mul).raw, UpperHalf(dh, x).raw));
//#endif
//}
//
//template<class D, HWY_IF_I16_D(D)>
//HWY_API Vec128<int32_t> WidenHighMulAdd(D /* tag */, Vec128<int16_t> mul, Vec128<int16_t> x, Vec128<int32_t> add) {
//#if HWY_ARCH_ARM_A64
//  return Vec128<int32_t>(vmlal_high_s16(add.raw, mul.raw, x.raw));
//#else
//  const Full64<int16_t> dh;
//  return Vec128<int32_t>(
//      vmlal_s16(add.raw, UpperHalf(dh, mul).raw, UpperHalf(dh, x).raw));
//#endif
//}
//
//template<class D, HWY_IF_U16_D(D)>
//HWY_API Vec128<uint32_t> WidenHighMulAdd(D /* tag */, Vec128<uint16_t> mul, Vec128<uint16_t> x, Vec128<uint32_t> add) {
//#if HWY_ARCH_ARM_A64
//  return Vec128<uint32_t>(vmlal_high_u16(add.raw, mul.raw, x.raw));
//#else
//  const Full64<uint16_t> dh;
//  return Vec128<uint32_t>(
//      vmlal_u16(add.raw, UpperHalf(dh, mul).raw, UpperHalf(dh, x).raw));
//#endif
//}
//
//template<class D, HWY_IF_U8_D(D)>
//HWY_API Vec128<uint16_t> WidenHighMulAdd(D /* tag */, Vec128<uint8_t> mul, Vec128<uint8_t> x, Vec128<uint16_t> add) {
//#if HWY_ARCH_ARM_A64
//  return Vec128<uint16_t>(vmlal_high_u8(add.raw, mul.raw, x.raw));
//#else
//  const Full64<uint8_t> dh;
//  return Vec128<uint16_t>(
//      vmlal_u8(add.raw, UpperHalf(dh, mul).raw, UpperHalf(dh, x).raw));
//#endif
//}
//
//template<class D, HWY_IF_I8_D(D)>
//HWY_API Vec128<int16_t> WidenHighMulAdd(D /* tag */, Vec128<int8_t> mul, Vec128<int8_t> x, Vec128<int16_t> add) {
//#if HWY_ARCH_ARM_A64
//  return Vec128<int16_t>(vmlal_high_s8(add.raw, mul.raw, x.raw));
//#else
//  const Full64<int8_t> dh;
//  return Vec128<int16_t>(
//      vmlal_s8(add.raw, UpperHalf(dh, mul).raw, UpperHalf(dh, x).raw));
//#endif
//}

template<class D, HWY_IF_U8_D(D), typename DW = Rebind<uint16_t, D>, typename VW = Vec<DW>>
HWY_API VW WidenMulW(D d, Vec<D> v, VW x) {
  const DW dw;
  return Mul(PromoteTo(dw, v), x);
}

template<int n, class D, HWY_IF_U8_D(D), typename DW = Rebind<MakeWide<TFromD<D>>, D>, typename VW = Vec<DW>>
HWY_API VW ShiftLeftWide(D d, Vec<D> v) {
  return Vec128<uint16_t>(vshll_n_u8(v.raw, n));
}

template<class D, HWY_IF_U16_D(D)>
HWY_API Vec<D> AddAndHalf(D d, Vec<D> v1, Vec<D> v2) {
  return Vec128<uint16_t>(vhaddq_u16(v1.raw, v2.raw));
}

template<class D, HWY_IF_I16_D(D)>
HWY_API Vec<D> AddAndHalf(D d, Vec<D> v1, Vec<D> v2) {
  return Vec128<int16_t>(vhaddq_s16(v1.raw, v2.raw));
}

template<class D, HWY_IF_I32_D(D)>
HWY_API Vec<D> AddAndHalf(D d, Vec<D> v1, Vec<D> v2) {
  return Vec128<int32_t>(vhaddq_s32(v1.raw, v2.raw));
}

#else

//template<class D, HWY_IF_INTEGER(TFromD<D>), class DW = Rebind<MakeWide<TFromD<D>>, D>, class VW = VFromD<DW>>
//HWY_API VW WidenMulAdd(D d, VFromD<D> mul, VFromD<D> x, VW add) {
//  const DW dw;
//  return MulAdd(PromoteTo(dw, mul), PromoteTo(dw, x), add);
//}
//
//template<class D, HWY_IF_INTEGER(TFromD<D>), class DW = RepartitionToWide<D>, class VW = VFromD<DW>>
//HWY_API VW WidenHighMulAdd(D d, VFromD<D> mul, VFromD<D> x, VW add) {
//  const DW dw;
//  return MulAdd(PromoteUpperTo(dw, mul), PromoteUpperTo(dw, x), add);
//}

template<int n, class D, HWY_IF_U16_D(D), typename D8 = Rebind<uint8_t, D>, typename VN = Vec<D8>>
HWY_API VN ShiftRightNarrow(D d, Vec<D> v) {
  const D8 d8;
  return DemoteTo(d8, ShiftRight<n>(v));
}

template<int n, class D, HWY_IF_U8_D(D), typename D16 = Rebind<uint16_t, D>, typename V16 = Vec<D16>,
    std::enable_if<n >= 8, int>::type = 0>
HWY_API V16 ShiftLeftWide(D d, Vec<D> v) {
  const D16 d16;
  return ShiftLeft<n>(PromoteTo(d16, v));
}

template<int n, class D, HWY_IF_U8_D(D), typename D16 = Rebind<uint16_t, D>, typename V16 = Vec<D16>,
    std::enable_if<n < 8, int>::type = 0>
HWY_API V16 ShiftLeftWide(D d, Vec<D> v) {
  const D16 d16;
  return PromoteTo(d16, ShiftLeft<n>(v));
}

template<int n, class D, HWY_IF_I32_D(D), typename DN = Rebind<int16_t, D>, typename VN = Vec<DN>>
HWY_API VN ShiftRightNarrow(D d, Vec<D> v) {
  const Rebind<int16_t, D> d16;
  return DemoteTo(d16, ShiftRight<n>(v));
}

template<int n, class D, HWY_IF_U32_D(D), typename DN = Rebind<uint16_t, D>, typename VN = Vec<DN>>
HWY_API VN ShiftRightNarrow(D d, Vec<D> v) {
  const Rebind<uint16_t, D> d16;
  return DemoteTo(d16, ShiftRight<n>(v));
}

template<class D, HWY_IF_U8_D(D), typename DW = RepartitionToWide<D>, typename VW = Vec<DW>>
HWY_API VW WidenMulHigh(D d, Vec<D> v, Vec<D> x) {
  const DW dw;
  return Mul(PromoteUpperTo(dw, v), PromoteUpperTo(dw, x));
}

template<class D, HWY_IF_U8_D(D), typename DW = Rebind<uint16_t, D>, typename VW = Vec<DW>>
HWY_API VW WidenMul(D d, Vec<D> v, Vec<D> x) {
  const DW dw;
  return Mul(PromoteTo(dw, v), PromoteTo(dw, x));
}

template<class D, HWY_IF_U8_D(D), typename DW = RepartitionToWide<D>, typename VW = Vec<DW>>
HWY_API VW AddWide(D d, Vec<D> v, Vec<D> x) {
  const DW dw;
  return Add(PromoteTo(dw, v), PromoteTo(dw, x));
}

template<class D, HWY_IF_U8_D(D), typename DW = RepartitionToWide<D>, typename VW = Vec<DW>>
HWY_API VW AddHighWide(D d, Vec<D> v, Vec<D> x) {
  const DW dw;
  return Add(PromoteUpperTo(dw, v), PromoteUpperTo(dw, x));
}

template<class D, HWY_IF_U16_D(D)>
HWY_API Vec<D> AddAndHalf(D d, Vec<D> v1, Vec<D> v2) {
  return ShiftRight<1>(Add(v1, v2));
}

template<class D, HWY_IF_I16_D(D)>
HWY_API Vec<D> AddAndHalf(D d, Vec<D> v1, Vec<D> v2) {
  return ShiftRight<1>(Add(v1, v2));
}

template<class D, HWY_IF_I32_D(D)>
HWY_API Vec<D> AddAndHalf(D d, Vec<D> v1, Vec<D> v2) {
  return ShiftRight<1>(Add(v1, v2));
}

#endif

//template<class D, HWY_IF_INTEGER(TFromD<D>), typename DW = RepartitionToWide<D>, typename VW = Vec<DW>>
//HWY_API VW WidenMulAccumulate(D d, Vec<D> mul, Vec<D> x, VW low, VW& high) {
//  const Half<D> dh;
//  high = WidenHighMulAdd(d, mul, x, high);
//  return WidenMulAdd(dh, LowerHalf(mul), LowerHalf(x), low);
//}

template<sparkyuv::SparkYuvDefaultPixelType PixelType,
    class D, typename V = Vec<D>, HWY_IF_U8_D(D),
    typename D16 = Rebind<uint16_t, D>, typename V16 = Vec<D16>>
HWY_API void LoadRGBTo16(D d, const uint8_t *src, V16 &R, V16 &G, V16 &B) {
  using V8 = Vec<decltype(d)>;
  V R8, G8, B8, A8;
  switch (PixelType) {
    case sparkyuv::PIXEL_RGB:LoadInterleaved3(d, reinterpret_cast<const uint8_t *>(src), R8, G8, B8);
      break;
    case sparkyuv::PIXEL_BGR:LoadInterleaved3(d, reinterpret_cast<const uint8_t *>(src), B8, G8, R8);
      break;
    case sparkyuv::PIXEL_RGBA:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), R8, G8, B8, A8);
      break;
    case sparkyuv::PIXEL_BGRA:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), B8, G8, R8, A8);
      break;
    case sparkyuv::PIXEL_ARGB:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), A8, R8, G8, B8);
      break;
    case sparkyuv::PIXEL_ABGR:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), A8, B8, G8, R8);
      break;
  }
  D16 d16;
  R = PromoteTo(d16, R8);
  G = PromoteTo(d16, G8);
  B = PromoteTo(d16, B8);
}

template<sparkyuv::SparkYuvDefaultPixelType PixelType,
    class D, typename V = Vec<D>, HWY_IF_U16_D(D),
    typename D16 = Rebind<uint16_t, D>, typename V16 = Vec<D16>>
HWY_API void LoadRGBTo16(D d, const uint16_t *src, V16 &R, V16 &G, V16 &B) {
  V16 A;
  switch (PixelType) {
    case sparkyuv::PIXEL_RGB:LoadInterleaved3(d, reinterpret_cast<const uint16_t *>(src), R, G, B);
      break;
    case sparkyuv::PIXEL_BGR:LoadInterleaved3(d, reinterpret_cast<const uint16_t *>(src), B, G, R);
      break;
    case sparkyuv::PIXEL_RGBA:LoadInterleaved4(d, reinterpret_cast<const uint16_t *>(src), R, G, B, A);
      break;
    case sparkyuv::PIXEL_BGRA:LoadInterleaved4(d, reinterpret_cast<const uint16_t *>(src), B, G, R, A);
      break;
    case sparkyuv::PIXEL_ARGB:LoadInterleaved4(d, reinterpret_cast<const uint16_t *>(src), A, R, G, B);
      break;
    case sparkyuv::PIXEL_ABGR:LoadInterleaved4(d, reinterpret_cast<const uint16_t *>(src), A, B, G, R);
      break;
  }
}

template<sparkyuv::SparkYuvDefaultPixelType PixelType,
    class D, HWY_IF_U8_D(D), typename V = Vec<D>,
    typename D16 = Rebind<uint16_t, D>, typename V16 = Vec<D16>>
HWY_API void LoadRGBATo16(D d, uint8_t *src, V16 &R, V16 &G, V16 &B, V16 &A) {
  V R8, G8, B8, A8;
  switch (PixelType) {
    case sparkyuv::PIXEL_RGB:LoadInterleaved3(d, reinterpret_cast<const uint8_t *>(src), R8, G8, B8);
      break;
    case sparkyuv::PIXEL_BGR:LoadInterleaved3(d, reinterpret_cast<const uint8_t *>(src), B8, G8, R8);
      break;
    case sparkyuv::PIXEL_RGBA:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), R8, G8, B8, A8);
      break;
    case sparkyuv::PIXEL_BGRA:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), B8, G8, R8, A8);
      break;
    case sparkyuv::PIXEL_ARGB:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), A8, R8, G8, B8);
      break;
    case sparkyuv::PIXEL_ABGR:LoadInterleaved4(d, reinterpret_cast<const uint8_t *>(src), A8, B8, G8, R8);
      break;
  }
  D16 d16;
  R = PromoteTo(d16, R8);
  G = PromoteTo(d16, G8);
  B = PromoteTo(d16, B8);
  A = PromoteTo(d16, A8);
}

template<sparkyuv::SparkYuvDefaultPixelType PixelType, class D, typename V = Vec<D>>
HWY_API void StoreRGBA(D d, TFromD<D> *store, V R, V G, V B, V A) {
  switch (PixelType) {
    case sparkyuv::PIXEL_RGBA:StoreInterleaved4(R, G, B, A, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::PIXEL_ABGR:StoreInterleaved4(A, B, G, R, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::PIXEL_BGR:StoreInterleaved3(B, G, R, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::PIXEL_RGB:StoreInterleaved3(R, G, B, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::PIXEL_BGRA:StoreInterleaved4(B, G, R, A, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::PIXEL_ARGB:StoreInterleaved4(A, R, G, B, d, reinterpret_cast<TFromD<D> *>(store));
      break;
  }
}

template<sparkyuv::SparkYuvReformatPixelType PixelType, class D, typename V = Vec<D>>
HWY_API void StoreReformatRGBA(D d, TFromD<D> *store, V R, V G, V B, V A) {
  switch (PixelType) {
    case sparkyuv::REFORMAT_RGBA:StoreInterleaved4(R, G, B, A, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::REFORMAT_ABGR:StoreInterleaved4(A, B, G, R, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::REFORMAT_BGR:StoreInterleaved3(B, G, R, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::REFORMAT_RGB:StoreInterleaved3(R, G, B, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::REFORMAT_BGRA:StoreInterleaved4(B, G, R, A, d, reinterpret_cast<TFromD<D> *>(store));
      break;
    case sparkyuv::REFORMAT_ARGB:StoreInterleaved4(A, R, G, B, d, reinterpret_cast<TFromD<D> *>(store));
      break;
  }
}

template<sparkyuv::SparkYuvDefaultPixelType PixelType, class D, typename V = Vec<D>>
HWY_API void LoadRGBA(D d, const TFromD<D> *source, V &R, V &G, V &B, V &A) {
  switch (PixelType) {
    case sparkyuv::PIXEL_RGB:LoadInterleaved3(d, reinterpret_cast<const TFromD<D> *>(source), R, G, B);
      break;
    case sparkyuv::PIXEL_BGR:LoadInterleaved3(d, reinterpret_cast<const TFromD<D> *>(source), B, G, R);
      break;
    case sparkyuv::PIXEL_RGBA:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), R, G, B, A);
      break;
    case sparkyuv::PIXEL_BGRA:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), B, G, R, A);
      break;
    case sparkyuv::PIXEL_ARGB:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), A, R, G, B);
      break;
    case sparkyuv::PIXEL_ABGR:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), A, B, G, R);
      break;
  }
}

template<sparkyuv::SparkYuvReformatPixelType PixelType, class D, typename V = Vec<D>>
HWY_API void LoadReformatRGBA(D d, const TFromD<D> *source, V &R, V &G, V &B, V &A) {
  switch (PixelType) {
    case sparkyuv::REFORMAT_RGB:LoadInterleaved3(d, reinterpret_cast<const TFromD<D> *>(source), R, G, B);
      break;
    case sparkyuv::REFORMAT_BGR:LoadInterleaved3(d, reinterpret_cast<const TFromD<D> *>(source), B, G, R);
      break;
    case sparkyuv::REFORMAT_RGBA:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), R, G, B, A);
      break;
    case sparkyuv::REFORMAT_BGRA:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), B, G, R, A);
      break;
    case sparkyuv::REFORMAT_ARGB:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), A, R, G, B);
      break;
    case sparkyuv::REFORMAT_ABGR:LoadInterleaved4(d, reinterpret_cast<const TFromD<D> *>(source), A, B, G, R);
      break;
    default:A = Zero(d);
      R = Zero(d);
      B = Zero(d);
      G = Zero(d);
      break;
  }
}

template<sparkyuv::SparkYuvDefaultPixelType PixelType, typename D, typename V = Vec<D>>
HWY_API void LoadRGB(D d, const uint8_t *source, V &R, V &G, V &B) {
  const Rebind<uint8_t, decltype(d)> du8;
  using V8 = Vec<decltype(du8)>;
  V8 R8, G8, B8, A8;
  switch (PixelType) {
    case sparkyuv::PIXEL_RGB:LoadInterleaved3(du8, reinterpret_cast<const uint8_t *>(source), R8, G8, B8);
      break;
    case sparkyuv::PIXEL_BGR:LoadInterleaved3(du8, reinterpret_cast<const uint8_t *>(source), B8, G8, R8);
      break;
    case sparkyuv::PIXEL_RGBA:LoadInterleaved4(du8, reinterpret_cast<const uint8_t *>(source), R8, G8, B8, A8);
      break;
    case sparkyuv::PIXEL_BGRA:LoadInterleaved4(du8, reinterpret_cast<const uint8_t *>(source), B8, G8, R8, A8);
      break;
    case sparkyuv::PIXEL_ARGB:LoadInterleaved4(du8, reinterpret_cast<const uint8_t *>(source), A8, R8, G8, B8);
      break;
    case sparkyuv::PIXEL_ABGR:LoadInterleaved4(du8, reinterpret_cast<const uint8_t *>(source), A8, B8, G8, R8);
      break;
  }
  R = PromoteTo(d, R8);
  G = PromoteTo(d, G8);
  B = PromoteTo(d, B8);
}

template<sparkyuv::SparkYuvDefaultPixelType PixelType,
    class D, typename V = Vec<D>, HWY_IF_U16_D(D)>
HWY_API void StoreRGBA(D d, uint8_t *store, V R, V G, V B, V A) {
  const Rebind<uint8_t, decltype(d)> t;
  const auto r = DemoteTo(t, R);
  const auto g = DemoteTo(t, G);
  const auto b = DemoteTo(t, B);
  const auto a = DemoteTo(t, A);
  switch (PixelType) {
    case sparkyuv::PIXEL_RGBA:StoreInterleaved4(r, g, b, a, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_ABGR:StoreInterleaved4(a, b, g, r, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_BGR:StoreInterleaved3(b, g, r, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_RGB:StoreInterleaved3(r, g, b, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_BGRA:StoreInterleaved4(b, g, r, a, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_ARGB:StoreInterleaved4(a, r, g, b, t, reinterpret_cast<uint8_t *>(store));
      break;
  }
}

template<sparkyuv::SparkYuvDefaultPixelType PixelType,
    class D, typename V = Vec<D>, HWY_IF_I16_D(D)>
HWY_API void StoreRGBA(D d, uint8_t *store, V R, V G, V B, V A) {
  const Rebind<uint8_t, decltype(d)> t;
  const auto r = DemoteTo(t, R);
  const auto g = DemoteTo(t, G);
  const auto b = DemoteTo(t, B);
  const auto a = DemoteTo(t, A);
  switch (PixelType) {
    case sparkyuv::PIXEL_RGBA:StoreInterleaved4(r, g, b, a, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_ABGR:StoreInterleaved4(a, b, g, r, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_BGR:StoreInterleaved3(b, g, r, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_RGB:StoreInterleaved3(r, g, b, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_BGRA:StoreInterleaved4(b, g, r, a, t, reinterpret_cast<uint8_t *>(store));
      break;
    case sparkyuv::PIXEL_ARGB:StoreInterleaved4(a, r, g, b, t, reinterpret_cast<uint8_t *>(store));
      break;
  }
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

static void ComputeForwardTransform(const float kr,
                                    const float kb,
                                    const float biasY,
                                    const float biasUV,
                                    const float rangeY,
                                    const float rangeUV,
                                    const float fullRangeRGB,
                                    float &YR, float &YG, float &YB,
                                    float &Cb,
                                    float &Cr) {
  const float kg = 1.0f - kr - kb;
  if (kg == 0.f) {
    throw std::runtime_error("1.0f - kr - kg must not be 0");
  }

  YR = kr * rangeY / fullRangeRGB;
  YG = kg * rangeY / fullRangeRGB;
  YB = kb * rangeY / fullRangeRGB;

  Cb = 0.5f / (1.f - kb) * rangeUV / fullRangeRGB;
  Cr = 0.5f / (1.f - kr) * rangeUV / fullRangeRGB;
}

static void ComputeForwardIntegersTransform(const float kr,
                                            const float kb,
                                            const float biasY,
                                            const float biasUV,
                                            const float rangeY,
                                            const float rangeUV,
                                            const float fullRangeRGB,
                                            const uint16_t precision,
                                            const uint16_t uvPrecision,
                                            uint16_t &uYR, uint16_t &uYG, uint16_t &uYB,
                                            uint16_t &kuCb, uint16_t &kuCr) {
  float YR, YG, YB;
  float kCb, kCr;

  const auto scale = static_cast<float>( 1 << precision );
  const auto scaleUV = static_cast<float>( 1 << uvPrecision );

  ComputeForwardTransform(kr, kb, static_cast<float>(biasY), static_cast<float>(biasUV),
                          static_cast<float>(rangeY), static_cast<float>(rangeUV),
                          fullRangeRGB, YR, YG, YB, kCb, kCr);

  uYR = static_cast<uint16_t>(::roundf(scale * YR));
  uYG = static_cast<uint16_t>(::roundf(scale * YG));
  uYB = static_cast<uint16_t>(::roundf(scale * YB));

  kuCb = static_cast<uint16_t>(::roundf(scaleUV * kCb));
  kuCr = static_cast<uint16_t>(::roundf(scaleUV * kCr));
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

  uYR = static_cast<uint16_t>(::roundf(scale * YR));
  uYG = static_cast<uint16_t>(::roundf(scale * YG));
  uYB = static_cast<uint16_t>(::roundf(scale * YB));

  uCbR = static_cast<uint16_t>(::roundf(scale * CbR));
  uCbG = static_cast<uint16_t>(::roundf(scale * CbG));
  uCbB = static_cast<uint16_t>(::roundf(scale * CbB));

  uCrR = static_cast<uint16_t>(::roundf(scale * CrR));
  uCrG = static_cast<uint16_t>(::roundf(scale * CrG));
  uCrB = static_cast<uint16_t>(::roundf(scale * CrB));
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

template<typename T, typename C, SparkYuvDefaultPixelType PixelType>
SPARKYUV_INLINE static void LoadRGBA(const T *source, C &r, C &g, C &b, C &a) {
  switch (PixelType) {
    case PIXEL_RGB:r = static_cast<C>(source[0]);
      g = static_cast<C>(source[1]);
      b = static_cast<C>(source[2]);
      break;
    case PIXEL_RGBA:r = static_cast<C>(source[0]);
      g = static_cast<C>(source[1]);
      b = static_cast<C>(source[2]);
      a = static_cast<C>(source[3]);
      break;
    case PIXEL_BGRA:b = static_cast<C>(source[0]);
      g = static_cast<C>(source[1]);
      r = static_cast<C>(source[2]);
      a = static_cast<C>(source[3]);
      break;
    case PIXEL_BGR:b = static_cast<C>(source[0]);
      g = static_cast<C>(source[1]);
      r = static_cast<C>(source[2]);
      break;
    case PIXEL_ARGB:a = static_cast<C>(source[0]);
      r = static_cast<C>(source[1]);
      g = static_cast<C>(source[2]);
      b = static_cast<C>(source[3]);
      break;
    case PIXEL_ABGR:a = static_cast<C>(source[0]);
      b = static_cast<C>(source[1]);
      g = static_cast<C>(source[2]);
      r = static_cast<C>(source[3]);
      break;
  }
}

template<typename T, typename C, SparkYuvDefaultPixelType PixelType>
SPARKYUV_INLINE static void LoadRGB(const T *source, C &r, C &g, C &b) {
  switch (PixelType) {
    case PIXEL_RGB:
    case PIXEL_RGBA:r = static_cast<C>(source[0]);
      g = static_cast<C>(source[1]);
      b = static_cast<C>(source[2]);
      break;
    case PIXEL_BGRA:
    case PIXEL_BGR:r = static_cast<C>(source[2]);
      g = static_cast<C>(source[1]);
      b = static_cast<C>(source[0]);
      break;
    case PIXEL_ARGB:r = static_cast<C>(source[1]);
      g = static_cast<C>(source[2]);
      b = static_cast<C>(source[3]);
      break;
    case PIXEL_ABGR:r = static_cast<int>(source[3]);
      g = static_cast<C>(source[2]);
      b = static_cast<C>(source[1]);
      break;
  }
}

template<typename T, typename C, SparkYuvDefaultPixelType PixelType>
SPARKYUV_INLINE static void StoreRGBA(T *store, C r, C g, C b, C a) {
  switch (PixelType) {
    case PIXEL_RGBA:store[3] = a;
    case PIXEL_RGB:store[0] = static_cast<T>(r);
      store[1] = static_cast<T>(g);
      store[2] = static_cast<T>(b);
      break;
    case PIXEL_BGRA:store[3] = a;
    case PIXEL_BGR:store[2] = static_cast<T>(r);
      store[1] = static_cast<T>(g);
      store[0] = static_cast<T>(b);
      break;
    case PIXEL_ABGR:store[0] = a;
      store[3] = static_cast<T>(r);
      store[2] = static_cast<T>(g);
      store[1] = static_cast<T>(b);
      break;
    case PIXEL_ARGB: {
      store[0] = a;
      store[1] = static_cast<T>(r);
      store[2] = static_cast<T>(g);
      store[3] = static_cast<T>(b);
    }
      break;
  }
}

template<typename T, typename C, SparkYuvDefaultPixelType PixelType>
SPARKYUV_INLINE static void SaturatedStoreRGBA(T *store, C r, C g, C b, C a, C max) {
  switch (PixelType) {
    case PIXEL_RGBA:store[3] = static_cast<T>(a);
    case PIXEL_RGB:store[0] = static_cast<T>(std::clamp(r, 0, max));
      store[1] = static_cast<T>(std::clamp(g, 0, max));
      store[2] = static_cast<T>(std::clamp(b, 0, max));
      break;
    case PIXEL_BGRA:store[3] = a;
    case PIXEL_BGR:store[2] = static_cast<T>(std::clamp(r, 0, max));
      store[1] = static_cast<T>(std::clamp(g, 0, max));
      store[0] = static_cast<T>(std::clamp(b, 0, max));
      break;
    case PIXEL_ABGR:store[0] = a;
      store[3] = static_cast<T>(std::clamp(r, 0, max));
      store[2] = static_cast<T>(std::clamp(g, 0, max));
      store[1] = static_cast<T>(std::clamp(b, 0, max));
      break;
    case PIXEL_ARGB: {
      store[0] = a;
      store[1] = static_cast<T>(std::clamp(r, 0, max));
      store[2] = static_cast<T>(std::clamp(g, 0, max));
      store[3] = static_cast<T>(std::clamp(b, 0, max));
    }
      break;
  }
}

}
HWY_AFTER_NAMESPACE();

#endif