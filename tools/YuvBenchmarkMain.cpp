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

static std::string filename = "filirovska.jpeg";

static void SparkyuvYUV444ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYUV444BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uPlane.data(), uvPlaneStride,
                              vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::YUV444BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvYUV444ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYUV444BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
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

static void SparkyuvYUV422ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYUV422BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uPlane.data(), uvPlaneStride,
                              vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::YUV422BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvYUV422ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYUV420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
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

static void SparkyuvYUV420ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYUV420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                              yPlane.data(), yPlaneStride,
                              uPlane.data(), uvPlaneStride,
                              vPlane.data(), uvPlaneStride);
  for (auto _ : state) {
    sparkyuv::YUV420BT601ToRGBA(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvYUV420ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBAToYUV420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
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

static void SparkyuvRGBA8ToYUV420(benchmark::State &state) {
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
    sparkyuv::RGBAToYUV420BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  }
}

static void LibyuvRGBA8ToYUV420(benchmark::State &state) {
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

void SparkyuvRGBA8ToYUV422(benchmark::State &state) {
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
    sparkyuv::RGBAToYUV422BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  }
}

static void LibYuvRGBA8ToYUV422(benchmark::State &state) {
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

void SparkyuvRGBA8ToYUV444(benchmark::State &state) {
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
    sparkyuv::RGBAToYUV444BT601(rgbaData.data(), rgbaStride, inWidth, inHeight,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride);
  }
}

BENCHMARK(SparkyuvYUV444P10ToRGBA10);
BENCHMARK(LibYuvYUV444P10ToRGBA8);
BENCHMARK(SparkyuvYUV422P10ToRGBA10);
BENCHMARK(LibYuvYUV422P10ToRGBA8);
BENCHMARK(SparkyuvYUV420P10ToRGBA10);
BENCHMARK(LibYuvYUV420P10ToRGBA8);
BENCHMARK(SparkyuvRGBAP10ToYUV420P10);
BENCHMARK(SparkyuvRGBA10ToYUV422P10);
BENCHMARK(SparkyuvRGBA10ToYUV444P10);
BENCHMARK(SparkyuvYUV444ToRGBA8);
BENCHMARK(LibYuvYUV444ToRGBA8);
BENCHMARK(SparkyuvYUV422ToRGBA8);
BENCHMARK(LibYuvYUV422ToRGBA8);
BENCHMARK(SparkyuvYUV420ToRGBA8);
BENCHMARK(LibYuvYUV420ToRGBA8);
BENCHMARK(SparkyuvRGBA8ToYUV420);
BENCHMARK(LibyuvRGBA8ToYUV420);
BENCHMARK(SparkyuvRGBA8ToYUV422);
BENCHMARK(LibYuvRGBA8ToYUV422);
BENCHMARK(SparkyuvRGBA8ToYUV444);

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

BENCHMARK(LibyuvPremultiply);
BENCHMARK(SparkyuvPremultiply);

BENCHMARK_MAIN();