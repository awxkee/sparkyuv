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

#include "YuvBenchmarkYIQ.h"
#include <iostream>
#include <vector>
#include "sparkyuv.h"
#include "../JPEGDecoder.h"
#include <benchmark/benchmark.h>

static std::string filename = "filirovska.jpeg";

void SparkyuvYIQ444ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
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
  sparkyuv::RGBA8ToYIQ444P8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uPlane.data(), uvPlaneStride,
                              vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YIQ444P8ToRGBA8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvYIQ422ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
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
  uvPlaneStride = (inWidth + 1) / 2;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBA8ToYIQ422P8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uPlane.data(), uvPlaneStride,
                              vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YIQ422P8ToRGBA8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvYIQ420ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
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
  uvPlaneStride = (inWidth + 1) / 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBA8ToYIQ420P8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uPlane.data(), uvPlaneStride,
                              vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YIQ420P8ToRGBA8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvRGBA8ToYIQ420(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
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
  uvPlaneStride = (inWidth + 1) / 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBA8ToYIQ420P8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvRGBA8ToYIQ422(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
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
  uvPlaneStride = (inWidth + 1) / 2;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBA8ToYIQ422P8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvRGBA8ToYIQ444(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
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
  for (auto _ : state) {
    sparkyuv::RGBA8ToYIQ444P8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_TV);
  }
}