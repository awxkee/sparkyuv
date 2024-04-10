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

#include "YuvBenchmarkNV.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "sparkyuv.h"
#include "../JPEGDecoder.h"
#include <benchmark/benchmark.h>
#include "libyuv.h"

static std::string filename = "filirovska.jpeg";

void SparkyuvRGBA8ToNV21(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBAToNV21BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}

void SparkyuvNV21ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToNV21BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                            yPlane.data(), yPlaneStride,
                            uvPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::NV21BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}

void LibyuvNV21ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToNV21BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                            yPlane.data(), yPlaneStride,
                            uvPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    libyuv::NV12ToABGR(yPlane.data(), yPlaneStride,
                       uvPlane.data(), uvPlaneStride,
                       rgbaData.data(), rgbaStride, inWidth, inHeight);
  }
}

void SparkyuvRGBA8ToNV12(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBAToNV12BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}


void SparkyuvNV12ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToNV12BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                            yPlane.data(), yPlaneStride,
                            uvPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::NV12BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}

void LibyuvNV12ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = (inHeight + 1) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToNV12BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                            yPlane.data(), yPlaneStride,
                            uvPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    libyuv::NV21ToABGR(yPlane.data(), yPlaneStride,
                       uvPlane.data(), uvPlaneStride,
                       rgbaData.data(), rgbaStride, inWidth, inHeight);
  }
}

void SparkyuvRGBA8ToNV16(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBAToNV16BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}

void SparkyuvNV16ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = ((inWidth + 1) / 2) * 2;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToNV16BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                            yPlane.data(), yPlaneStride,
                            uvPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::NV16BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}

void SparkyuvRGBA8ToNV24(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = inWidth * 2;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBAToNV24BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}

void SparkyuvNV24ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uvPlane;
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
  uvPlaneStride = inWidth * 2;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uvPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToNV24BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                            yPlane.data(), yPlaneStride,
                            uvPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::NV24BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uvPlane.data(), uvPlaneStride);
  }
}