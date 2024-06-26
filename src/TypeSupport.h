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

#ifndef YUV_SRC_TYPESUPPORT_H_
#define YUV_SRC_TYPESUPPORT_H_

#include "hwy/highway.h"
#include <cstdint>
#include <algorithm>

#if defined(__GNUC__) || defined(__clang__)
#define TYPE_INLINE static __attribute__((flatten)) inline
#else
#define TYPE_INLINE static inline
#endif

#define ENABLE_TYPE_IS_F16(T) typename std::enable_if<(std::is_same<T, hwy::float16_t>::value), int>::type = 0
#define ENABLE_TYPE_IS_NOT_F16(T) typename std::enable_if<(!std::is_same<T, hwy::float16_t>::value), int>::type = 0

template<typename T, ENABLE_TYPE_IS_F16(T)>
TYPE_INLINE float LoadFloat(const T *src) {
#if HWY_HAVE_FLOAT16 || HWY_HAVE_SCALAR_F16_OPERATORS
  return static_cast<float>(src[0]);
#else
  auto uSource = reinterpret_cast<const uint16_t *>(src);
  return hwy::F32FromF16(hwy::float16_t::FromBits(uSource[0]));
#endif
}

template<typename T, ENABLE_TYPE_IS_NOT_F16(T)>
TYPE_INLINE float LoadFloat(const T *src) {
  return static_cast<float>(src[0]);
}

template<typename V, typename T, ENABLE_TYPE_IS_F16(T)>
TYPE_INLINE V LoadPixel(const T *src) {
#if HWY_HAVE_FLOAT16 || HWY_HAVE_SCALAR_F16_OPERATORS
  return static_cast<V>(src[0]);
#else
  auto uSource = reinterpret_cast<const uint16_t *>(src);
  return static_cast<V>(hwy::F32FromF16(hwy::float16_t::FromBits(uSource[0])));
#endif
}

template<typename V, typename T, ENABLE_TYPE_IS_NOT_F16(T)>
TYPE_INLINE V LoadPixel(const T *src) {
  return static_cast<V>(src[0]);
}

template<typename V, typename T, ENABLE_TYPE_IS_NOT_F16(V)>
TYPE_INLINE V TransformCast(T t) {
  return static_cast<V>(t);
}

template<typename V, typename T, ENABLE_TYPE_IS_F16(V)>
TYPE_INLINE V TransformCast(T t) {
#if HWY_HAVE_FLOAT16 || HWY_HAVE_SCALAR_F16_OPERATORS
  return static_cast<hwy::float16_t>(t);
#else
  return hwy::F16FromF32(t);
#endif
}

template<typename T, typename V, ENABLE_TYPE_IS_NOT_F16(V)>
TYPE_INLINE void StoreRoundedFloat(V *v, T t) {
  v[0] = static_cast<V>(::roundf(t));
}

template<typename T, typename V, ENABLE_TYPE_IS_F16(V)>
TYPE_INLINE void StoreRoundedFloat(V *v, T t) {
#if HWY_HAVE_FLOAT16 || HWY_HAVE_SCALAR_F16_OPERATORS
  v[0] = static_cast<V>(::roundf(t));
#else
  reinterpret_cast<uint16_t *>(v)[0] = hwy::F16FromF32(::roundf(t)).bits;
#endif
}

template<typename T, typename V, ENABLE_TYPE_IS_NOT_F16(V)>
TYPE_INLINE void StoreFloat(V *v, T t) {
  v[0] = static_cast<V>(t);
}

template<typename T, typename V, ENABLE_TYPE_IS_F16(V)>
TYPE_INLINE void StoreFloat(V *v, T t) {
#if HWY_HAVE_FLOAT16 || HWY_HAVE_SCALAR_F16_OPERATORS
  v[0] = static_cast<V>(t);
#else
  reinterpret_cast<uint16_t *>(v)[0] = hwy::F16FromF32(t).bits;
#endif
}

#endif //YUV_SRC_TYPESUPPORT_H_
