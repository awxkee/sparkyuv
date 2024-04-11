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

#include "YuvSupport.h"

#include <iostream>
#include <vector>
#include "sparkyuv.h"
#include "../JPEGDecoder.h"
#include <benchmark/benchmark.h>
#include "libyuv.h"

static std::string filename = "filirovska.jpeg";

void SparkyuvPremultiply(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint16_t> yPlane;
  std::vector<uint16_t> uPlane;
  std::vector<uint16_t> vPlane;
  std::vector<uint8_t> rgbaData;
  int yPlaneStride;
  int uvPlaneStride;
  int uvPlaneHeight;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  yPlaneStride = inWidth;
  uvPlaneStride = inWidth;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  std::vector<uint8_t > store(rgbaData.size());
  for (auto _ : state) {
    sparkyuv::RGBAPremultiplyAlpha(rgbaData.data(), rgbaStride, store.data(), rgbaStride, inWidth, inHeight);
  }
}

void LibyuvPremultiply(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint16_t> yPlane;
  std::vector<uint16_t> uPlane;
  std::vector<uint16_t> vPlane;
  std::vector<uint8_t> rgbaData;
  int yPlaneStride;
  int uvPlaneStride;
  int uvPlaneHeight;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  yPlaneStride = inWidth;
  uvPlaneStride = inWidth;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  std::vector<uint8_t > store(rgbaData.size());
  for (auto _ : state) {
    libyuv::ARGBAttenuate(rgbaData.data(), rgbaStride, store.data(), rgbaStride, inWidth, inHeight);
  }
}

void SparkyuvUnpremultiply(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint16_t> yPlane;
  std::vector<uint16_t> uPlane;
  std::vector<uint16_t> vPlane;
  std::vector<uint8_t> rgbaData;
  int yPlaneStride;
  int uvPlaneStride;
  int uvPlaneHeight;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  yPlaneStride = inWidth;
  uvPlaneStride = inWidth;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  std::vector<uint8_t > store(rgbaData.size());
  sparkyuv::RGBAPremultiplyAlpha(rgbaData.data(), rgbaStride, store.data(), rgbaStride, inWidth, inHeight);
  std::vector<uint8_t > dst(rgbaData.size());

  for (auto _ : state) {
    sparkyuv::RGBAUnpremultiplyAlpha(rgbaData.data(), rgbaStride, dst.data(), rgbaStride, inWidth, inHeight);
  }
}

void LibyuvUnpremultiply(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint16_t> yPlane;
  std::vector<uint16_t> uPlane;
  std::vector<uint16_t> vPlane;
  std::vector<uint8_t> rgbaData;
  int yPlaneStride;
  int uvPlaneStride;
  int uvPlaneHeight;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  yPlaneStride = inWidth;
  uvPlaneStride = inWidth;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  std::vector<uint8_t > store(rgbaData.size());
  sparkyuv::RGBAPremultiplyAlpha(rgbaData.data(), rgbaStride, store.data(), rgbaStride, inWidth, inHeight);
  std::vector<uint8_t > dst(rgbaData.size());

  for (auto _ : state) {
    libyuv::ARGBUnattenuate(rgbaData.data(), rgbaStride, dst.data(), rgbaStride, inWidth, inHeight);
  }
}