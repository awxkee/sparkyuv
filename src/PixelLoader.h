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

#ifndef YUV_SRC_PIXELLOADER_H_
#define YUV_SRC_PIXELLOADER_H_

#include "sparkyuv-internal.h"
#include "base.h"
#include "TypeSupport.h"

#if defined(__GNUC__) || defined(__clang__)
#define PIXEL_INLINE __attribute__((flatten)) inline
#else
#define PIXEL_INLINE inline
#endif

namespace sparkyuv {
template<typename T, typename C, SparkYuvDefaultPixelType PixelType>
PIXEL_INLINE static void LoadRGBA(const T *source, C &r, C &g, C &b, C &a) {
  switch (PixelType) {
    case PIXEL_RGB:r = LoadPixel<C, T>(&source[0]);
      g = LoadPixel<C, T>(&source[1]);
      b = LoadPixel<C, T>(&source[2]);
      break;
    case PIXEL_RGBA:r = LoadPixel<C, T>(&source[0]);
      g = LoadPixel<C, T>(&source[1]);
      b = LoadPixel<C, T>(&source[2]);
      a = LoadPixel<C, T>(&source[3]);
      break;
    case PIXEL_BGRA:b = LoadPixel<C, T>(&source[0]);
      g = LoadPixel<C, T>(&source[1]);
      r = LoadPixel<C, T>(&source[2]);
      a = LoadPixel<C, T>(&source[3]);
      break;
    case PIXEL_BGR:b = LoadPixel<C, T>(&source[0]);
      g = LoadPixel<C, T>(&source[1]);
      r = LoadPixel<C, T>(&source[2]);
      break;
    case PIXEL_ARGB:a = LoadPixel<C, T>(&source[0]);
      r = LoadPixel<C, T>(&source[1]);
      g = LoadPixel<C, T>(&source[2]);
      b = LoadPixel<C, T>(&source[3]);
      break;
    case PIXEL_ABGR:a = LoadPixel<C, T>(&source[0]);
      b = LoadPixel<C, T>(&source[1]);
      g = LoadPixel<C, T>(&source[2]);
      r = LoadPixel<C, T>(&source[3]);
      break;
  }
}

template<typename T, typename C, SparkYuvDefaultPixelType PixelType>
PIXEL_INLINE static void LoadRGB(const T *source, C &r, C &g, C &b) {
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
PIXEL_INLINE static void StoreRGBA(T *store, C r, C g, C b, C a) {
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

template<typename T, typename C, SparkYuvDefaultPixelType PixelType,
    typename std::enable_if<std::is_same<T, hwy::float16_t>::value, int>::type = 0,
    typename std::enable_if<std::is_same<C, hwy::float16_t>::value, int>::type = 0>
PIXEL_INLINE static void StoreRGB(T *store, C r, C g, C b) {
  auto uStore = reinterpret_cast<uint16_t *>(store);
  switch (PixelType) {
    case PIXEL_RGBA:
    case PIXEL_RGB:uStore[0] = r.bits;
      uStore[1] = g.bits;
      uStore[2] = b.bits;
      break;
    case PIXEL_BGRA:
    case PIXEL_BGR:uStore[2] = r.bits;
      uStore[1] = g.bits;
      uStore[0] = b.bits;
      break;
    case PIXEL_ABGR:uStore[3] = r.bits;
      uStore[2] = g.bits;
      uStore[1] = b.bits;
      break;
    case PIXEL_ARGB: {
      uStore[1] = r.bits;
      uStore[2] = g.bits;
      uStore[3] = b.bits;
    }
      break;
  }
}

template<typename T, typename C, SparkYuvDefaultPixelType PixelType,
    typename std::enable_if<!std::is_same<T, hwy::float16_t>::value, int>::type = 0,
    typename std::enable_if<!std::is_same<C, hwy::float16_t>::value, int>::type = 0>
PIXEL_INLINE static void StoreRGB(T *store, C r, C g, C b) {
  switch (PixelType) {
    case PIXEL_RGBA:
    case PIXEL_RGB:store[0] = static_cast<T>(r);
      store[1] = static_cast<T>(g);
      store[2] = static_cast<T>(b);
      break;
    case PIXEL_BGRA:
    case PIXEL_BGR:store[2] = static_cast<T>(r);
      store[1] = static_cast<T>(g);
      store[0] = static_cast<T>(b);
      break;
    case PIXEL_ABGR:store[3] = static_cast<T>(r);
      store[2] = static_cast<T>(g);
      store[1] = static_cast<T>(b);
      break;
    case PIXEL_ARGB: {
      store[1] = static_cast<T>(r);
      store[2] = static_cast<T>(g);
      store[3] = static_cast<T>(b);
    }
      break;
  }
}

template<typename T, typename C, SparkYuvDefaultPixelType PixelType>
PIXEL_INLINE static void SaturatedStoreRGBA(T *store, C r, C g, C b, C a, C max) {
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

#endif //YUV_SRC_PIXELLOADER_H_
