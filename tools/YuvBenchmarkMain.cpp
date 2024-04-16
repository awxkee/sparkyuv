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

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "sparkyuv.h"
#include "JPEGDecoder.h"
#include <benchmark/benchmark.h>
#include "libyuv.h"
#include "bench/YuvBenchmark10.h"
#include "bench/YuvBenchmarkYCgCo.h"
#include "bench/YuvBenchmarkYCgCoR.h"
#include "bench/YuvBenchmarkYcCbcCrc.h"
#include "bench/YuvSupport.h"
#include "bench/YuvBenchmarkYDzDx.h"
#include "bench/YuvBenchmarkYIQ.h"
#include "bench/YuvBenchmarkYDbDr.h"
#include "bench/YuvBenchmarkBase.h"
#include "bench/YuvBenchmarkNV.h"

static std::string filename = "filirovska.jpeg";

static void SparkyuvYCbCr444ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYCbCr444BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::YCbCr444BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                  yPlane.data(), yPlaneStride,
                                  uPlane.data(), uvPlaneStride,
                                  vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvYCbCr444ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYCbCr444BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    libyuv::I444ToABGR(yPlane.data(), yPlaneStride,
                       uPlane.data(), uvPlaneStride,
                       vPlane.data(), uvPlaneStride,
                       rgbaData.data(), rgbaStride, inWidth, inHeight);
  }
}

static void SparkyuvYCbCr422ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYCbCr422BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::YCbCr422BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                  yPlane.data(), yPlaneStride,
                                  uPlane.data(), uvPlaneStride,
                                  vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvYCbCr422ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYCbCr420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    libyuv::I422ToABGR(yPlane.data(), yPlaneStride,
                       uPlane.data(), uvPlaneStride,
                       vPlane.data(), uvPlaneStride,
                       rgbaData.data(), rgbaStride, inWidth, inHeight);
  }
}

static void SparkyuvYCbCr420ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYCbCr420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::YCbCr420BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                  yPlane.data(), yPlaneStride,
                                  uPlane.data(), uvPlaneStride,
                                  vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvYCbCr420ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYCbCr420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    libyuv::I420ToABGR(yPlane.data(), yPlaneStride,
                       uPlane.data(), uvPlaneStride,
                       vPlane.data(), uvPlaneStride,
                       rgbaData.data(), rgbaStride, inWidth, inHeight);
  }
}

static void SparkyuvRGBA8ToYCbCr420(benchmark::State &state) {
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
    sparkyuv::RGBAToYCbCr420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                  yPlane.data(), yPlaneStride,
                                  uPlane.data(), uvPlaneStride,
                                  vPlane.data(), uvPlaneStride);
  }
}

static void LibyuvRGBA8ToYCbCr420(benchmark::State &state) {
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
    libyuv::ABGRToI420(rgbaData.data(), rgbaStride, yPlane.data(), yPlaneStride,
                       uPlane.data(), uvPlaneStride,
                       vPlane.data(), uvPlaneStride, inWidth, inHeight);
  }
}

void SparkyuvRGBA8ToYCbCr422(benchmark::State &state) {
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
    sparkyuv::RGBAToYCbCr422BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                  yPlane.data(), yPlaneStride,
                                  uPlane.data(), uvPlaneStride,
                                  vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvRGBA8ToYCbCr422(benchmark::State &state) {
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
    libyuv::ABGRToJ422(rgbaData.data(), rgbaStride, yPlane.data(), yPlaneStride,
                       uPlane.data(), uvPlaneStride,
                       vPlane.data(), uvPlaneStride, inWidth, inHeight);
  }
}

void SparkyuvRGBA8ToYCbCr444(benchmark::State &state) {
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
    sparkyuv::RGBAToYCbCr444BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                  yPlane.data(), yPlaneStride,
                                  uPlane.data(), uvPlaneStride,
                                  vPlane.data(), uvPlaneStride);
  }
}

void SparkyuvRGBA8ToYCbCr400(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
  std::vector<uint8_t> rgbaData;
  int yPlaneStride;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  yPlaneStride = inWidth;

  yPlane.resize(yPlaneStride * inHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBAToYCbCr400(rgbaData.data(), rgbaStride, inWidth, inHeight,
                             yPlane.data(), yPlaneStride,
                             0.299f, 0.114f, sparkyuv::YUV_RANGE_PC);
  }
}

static void SparkyuvYCbCr400ToRGBA8(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> rgbaData;
  int yPlaneStride;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  yPlaneStride = inWidth;

  yPlane.resize(yPlaneStride * inHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToYCbCr400(rgbaData.data(), rgbaStride, inWidth, inHeight,
                           yPlane.data(), yPlaneStride,
                           0.299f, 0.114f, sparkyuv::YUV_RANGE_PC);
  for (auto _ : state) {
    sparkyuv::YCbCr400ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                             yPlane.data(), yPlaneStride,
                             0.299f, 0.114f, sparkyuv::YUV_RANGE_PC);
  }
}

void SparkyuvRGBA8ToYCbCr411(benchmark::State &state) {
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
  uvPlaneStride = (inWidth + 5) / 4;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBAToYCbCr411(rgbaData.data(), rgbaStride, inWidth, inHeight,
                             yPlane.data(), yPlaneStride,
                             uPlane.data(), uvPlaneStride,
                             vPlane.data(), uvPlaneStride, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV);
  }
}

static void SparkyuvYCbCr411ToRGBA8(benchmark::State &state) {
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
  uvPlaneStride = (inWidth + 5) / 2;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToYCbCr411(rgbaData.data(), rgbaStride, inWidth, inHeight,
                           yPlane.data(), yPlaneStride,
                           uPlane.data(), uvPlaneStride,
                           vPlane.data(), uvPlaneStride, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YCbCr411ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                             yPlane.data(), yPlaneStride,
                             uPlane.data(), uvPlaneStride,
                             vPlane.data(), uvPlaneStride, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvRGBA8ToYCbCr410(benchmark::State &state) {
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
  uvPlaneStride = (inWidth + 5) / 4;
  uvPlaneHeight = inHeight;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  for (auto _ : state) {
    sparkyuv::RGBAToYCbCr410(rgbaData.data(), rgbaStride, inWidth, inHeight,
                             yPlane.data(), yPlaneStride,
                             uPlane.data(), uvPlaneStride,
                             vPlane.data(), uvPlaneStride, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV);
  }
}

static void SparkyuvYCbCr410ToRGBA8(benchmark::State &state) {
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
  uvPlaneStride = (inWidth + 5) / 2;
  uvPlaneHeight = (inHeight + 5) / 2;

  yPlane.resize(yPlaneStride * inHeight);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  sparkyuv::RGBAToYCbCr410(rgbaData.data(), rgbaStride, inWidth, inHeight,
                           yPlane.data(), yPlaneStride,
                           uPlane.data(), uvPlaneStride,
                           vPlane.data(), uvPlaneStride, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YCbCr410ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                             yPlane.data(), yPlaneStride,
                             uPlane.data(), uvPlaneStride,
                             vPlane.data(), uvPlaneStride, 0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV);
  }
}

BENCHMARK(SparkyuvYCbCr444P10ToRGBA10);
BENCHMARK(LibYuvYCbCr444P10ToRGBA8);
BENCHMARK(SparkyuvYCbCr422P10ToRGBA10);
BENCHMARK(LibYuvYCbCr422P10ToRGBA8);
BENCHMARK(SparkyuvYCbCr420P10ToRGBA10);
BENCHMARK(LibYuvYCbCr420P10ToRGBA8);
BENCHMARK(SparkyuvRGBAP10ToYCbCr420P10);
BENCHMARK(SparkyuvRGBA10ToYCbCr422P10);
BENCHMARK(SparkyuvRGBA10ToYCbCr444P10);
BENCHMARK(SparkyuvYCbCr444ToRGBA8);
BENCHMARK(LibYuvYCbCr444ToRGBA8);
BENCHMARK(SparkyuvYCbCr422ToRGBA8);
BENCHMARK(LibYuvYCbCr422ToRGBA8);
BENCHMARK(SparkyuvYCbCr420ToRGBA8);
BENCHMARK(LibYuvYCbCr420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYCbCr420);
BENCHMARK(LibyuvRGBA8ToYCbCr420);
BENCHMARK(LibYuvRGBA8ToYCbCr422);
BENCHMARK(SparkyuvRGBA8ToYCbCr444);
BENCHMARK(SparkyuvRGBA8ToYCbCr422);

BENCHMARK(SparkyuvRGBA8ToYCbCr411);
BENCHMARK(SparkyuvYCbCr411ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYCbCr410);
BENCHMARK(SparkyuvYCbCr410ToRGBA8);

BENCHMARK(SparkyuvRGBA8ToYCbCr400);
BENCHMARK(SparkyuvYCbCr400ToRGBA8);

BENCHMARK(SparkyuvRGBA8ToNV21);
BENCHMARK(SparkyuvNV21ToRGBA8);
BENCHMARK(LibyuvNV21ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToNV12);
BENCHMARK(SparkyuvNV12ToRGBA8);
BENCHMARK(LibyuvNV12ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToNV16);
BENCHMARK(SparkyuvNV16ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToNV24);
BENCHMARK(SparkyuvNV24ToRGBA8);

BENCHMARK(SparkyuvYCgCoR444ToRGBA8);
BENCHMARK(SparkyuvYCgCoR422ToRGBA8);
BENCHMARK(SparkyuvYCgCoR420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYCgCoR420);
BENCHMARK(SparkyuvRGBA8ToYCgCoR422);
BENCHMARK(SparkyuvRGBA8ToYCgCoR444);

BENCHMARK(SparkyuvYCgCo444ToRGBA8);
BENCHMARK(SparkyuvYCgCo422ToRGBA8);
BENCHMARK(SparkyuvYCgCo420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYCgCo420);
BENCHMARK(SparkyuvRGBA8ToYCgCo422);
BENCHMARK(SparkyuvRGBA8ToYCgCo444);

BENCHMARK(SparkyuvYcCbcCrc444ToRGBA8);
BENCHMARK(SparkyuvYcCbcCrc422ToRGBA8);
BENCHMARK(SparkyuvYcCbcCrc420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYcCbcCrc420);
BENCHMARK(SparkyuvRGBA8ToYcCbcCrc422);
BENCHMARK(SparkyuvRGBA8ToYcCbcCrc444);

BENCHMARK(SparkyuvYIQ444ToRGBA8);
BENCHMARK(SparkyuvYIQ422ToRGBA8);
BENCHMARK(SparkyuvYIQ420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYIQ420);
BENCHMARK(SparkyuvRGBA8ToYIQ422);
BENCHMARK(SparkyuvRGBA8ToYIQ444);

BENCHMARK(SparkyuvYDzDx444ToRGBA8);
BENCHMARK(SparkyuvYDzDx422ToRGBA8);
BENCHMARK(SparkyuvYDzDx420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYDzDx420);
BENCHMARK(SparkyuvRGBA8ToYDzDx422);
BENCHMARK(SparkyuvRGBA8ToYDzDx444);

BENCHMARK(SparkyuvYDbDr444ToRGBA8);
BENCHMARK(SparkyuvYDbDr422ToRGBA8);
BENCHMARK(SparkyuvYDbDr420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYDbDr420);
BENCHMARK(SparkyuvRGBA8ToYDbDr422);
BENCHMARK(SparkyuvRGBA8ToYDbDr444);

BENCHMARK(SparkyuvRGB10BitToF16);
BENCHMARK(LibyuvPremultiply);
BENCHMARK(SparkyuvPremultiply);
BENCHMARK(SparkyuvUnpremultiply);
BENCHMARK(LibyuvUnpremultiply);
BENCHMARK(SparkyuvWide8To10Fixed);
BENCHMARK(SparkyuvWide8To10Dynamic);
BENCHMARK(SparkyuvSaturate10To8Fixed);
BENCHMARK(SparkyuvSaturate10To8Dynamic);

BENCHMARK_MAIN();