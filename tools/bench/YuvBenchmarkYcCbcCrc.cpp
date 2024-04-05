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

#include "YuvBenchmarkYcCbcCrc.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "sparkyuv.h"
#include "../JPEGDecoder.h"
#include <benchmark/benchmark.h>
#include "libyuv.h"

static std::string filename = "filirovska.jpeg";

void SparkyuvYcCbcCrc444ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBA8ToYcCbcCrc444P8(rgbaData.data(),
                                 rgbaStride,
                                 inWidth,
                                 inHeight,
                                 yPlane.data(),
                                 yPlaneStride,
                                 uPlane.data(),
                                 uvPlaneStride,
                                 vPlane.data(),
                                 uvPlaneStride,
                                 0.299f,
                                 0.114f,
                                 sparkyuv::YUV_RANGE_TV,
                                 sparkyuv::TransferSRGB);
  for (auto _ : state) {
    sparkyuv::YcCbcCrc444P8ToRGBA8(rgbaData.data(),
                                   rgbaStride,
                                   inWidth,
                                   inHeight,
                                   yPlane.data(),
                                   yPlaneStride,
                                   uPlane.data(),
                                   uvPlaneStride,
                                   vPlane.data(),
                                   uvPlaneStride,
                                   0.299f,
                                   0.114f,
                                   sparkyuv::YUV_RANGE_TV,
                                   sparkyuv::TransferSRGB);
  }
}

void SparkyuvYcCbcCrc422ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBA8ToYcCbcCrc422P8(rgbaData.data(),
                                 rgbaStride,
                                 inWidth,
                                 inHeight,
                                 yPlane.data(),
                                 yPlaneStride,
                                 uPlane.data(),
                                 uvPlaneStride,
                                 vPlane.data(),
                                 uvPlaneStride,
                                 0.299f,
                                 0.114f,
                                 sparkyuv::YUV_RANGE_TV,
                                 sparkyuv::TransferSRGB);
  for (auto _ : state) {
    sparkyuv::YcCbcCrc422P8ToRGBA8(rgbaData.data(),
                                   rgbaStride,
                                   inWidth,
                                   inHeight,
                                   yPlane.data(),
                                   yPlaneStride,
                                   uPlane.data(),
                                   uvPlaneStride,
                                   vPlane.data(),
                                   uvPlaneStride,
                                   0.299f,
                                   0.114f,
                                   sparkyuv::YUV_RANGE_TV,
                                   sparkyuv::TransferSRGB);
  }
}

void SparkyuvYcCbcCrc420ToRGBA8(benchmark::State &state) {
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
  sparkyuv::RGBA8ToYcCbcCrc420P8(rgbaData.data(),
                                 rgbaStride,
                                 inWidth,
                                 inHeight,
                                 yPlane.data(),
                                 yPlaneStride,
                                 uPlane.data(),
                                 uvPlaneStride,
                                 vPlane.data(),
                                 uvPlaneStride,
                                 0.299f,
                                 0.114f,
                                 sparkyuv::YUV_RANGE_TV,
                                 sparkyuv::TransferSRGB);
  for (auto _ : state) {
    sparkyuv::YcCbcCrc420P8ToRGBA8(rgbaData.data(),
                                   rgbaStride,
                                   inWidth,
                                   inHeight,
                                   yPlane.data(),
                                   yPlaneStride,
                                   uPlane.data(),
                                   uvPlaneStride,
                                   vPlane.data(),
                                   uvPlaneStride,
                                   0.299f,
                                   0.114f,
                                   sparkyuv::YUV_RANGE_TV,
                                   sparkyuv::TransferSRGB);
  }
}

void SparkyuvRGBA8ToYcCbcCrc420(benchmark::State &state) {
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
    sparkyuv::RGBA8ToYcCbcCrc420P8(rgbaData.data(),
                                   rgbaStride,
                                   inWidth,
                                   inHeight,
                                   yPlane.data(),
                                   yPlaneStride,
                                   uPlane.data(),
                                   uvPlaneStride,
                                   vPlane.data(),
                                   uvPlaneStride,
                                   0.299f,
                                   0.114f,
                                   sparkyuv::YUV_RANGE_TV,
                                   sparkyuv::TransferSRGB);
  }
}

void SparkyuvRGBA8ToYcCbcCrc422(benchmark::State &state) {
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
    sparkyuv::RGBA8ToYcCbcCrc422P8(rgbaData.data(),
                                   rgbaStride,
                                   inWidth,
                                   inHeight,
                                   yPlane.data(),
                                   yPlaneStride,
                                   uPlane.data(),
                                   uvPlaneStride,
                                   vPlane.data(),
                                   uvPlaneStride,
                                   0.299f,
                                   0.114f,
                                   sparkyuv::YUV_RANGE_TV,
                                   sparkyuv::TransferSRGB);
  }
}

void SparkyuvRGBA8ToYcCbcCrc444(benchmark::State &state) {
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
    sparkyuv::RGBA8ToYcCbcCrc444P8(rgbaData.data(),
                                   rgbaStride,
                                   inWidth,
                                   inHeight,
                                   yPlane.data(),
                                   yPlaneStride,
                                   uPlane.data(),
                                   uvPlaneStride,
                                   vPlane.data(),
                                   uvPlaneStride,
                                   0.299f,
                                   0.114f,
                                   sparkyuv::YUV_RANGE_TV,
                                   sparkyuv::TransferSRGB);
  }
}