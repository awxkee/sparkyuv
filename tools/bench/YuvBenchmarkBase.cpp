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
#include "../JPEGDecoder.h"
#include <benchmark/benchmark.h>

static std::string filename = "filirovska.jpeg";

void SparkyuvWide8To10Fixed(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint16_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  for (auto _ : state) {
    sparkyuv::WideRGBA8To10(rgbaData.data(),
                            rgbaStride,
                            reinterpret_cast<uint16_t *>(rgba16Data.data()),
                            rgba16Stride,
                            inWidth,
                            inHeight);
  }
}

void SparkyuvWide8To10Dynamic(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint16_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  for (auto _ : state) {
    sparkyuv::WideRGBA8(rgbaData.data(),
                        rgbaStride,
                        reinterpret_cast<uint16_t *>(rgba16Data.data()),
                        rgba16Stride,
                        inWidth,
                        inHeight, 10);
  }
}

void SparkyuvSaturate10To8Fixed(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint16_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);
  for (auto _ : state) {
    sparkyuv::SaturateRGBA10To8(reinterpret_cast<uint16_t *>(rgba16Data.data()), rgba16Stride,
                                rgbaData.data(), rgbaStride, inWidth, inHeight);
  }
}

void SparkyuvSaturate10To8Dynamic(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint16_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8(rgbaData.data(),
                      rgbaStride,
                      reinterpret_cast<uint16_t *>(rgba16Data.data()),
                      rgba16Stride,
                      inWidth,
                      inHeight, 10);
  for (auto _ : state) {
    sparkyuv::SaturateRGBATo8(reinterpret_cast<uint16_t *>(rgba16Data.data()), rgba16Stride,
                              rgbaData.data(), rgbaStride, inWidth, inHeight, 10);
  }
}

void SparkyuvRGB10BitToF16(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint16_t) * inWidth * 4;
  std::vector<uint8_t> rgbaF16Data(rgba16Stride * inHeight);
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8(rgbaData.data(),
                      rgbaStride,
                      reinterpret_cast<uint16_t *>(rgba16Data.data()),
                      rgba16Stride,
                      inWidth,
                      inHeight, 10);
  for (auto _ : state) {
    sparkyuv::RGBA16ToRGBAF16(reinterpret_cast<uint16_t *>(rgba16Data.data()), rgba16Stride,
                              reinterpret_cast<uint16_t *>(rgbaF16Data.data()), rgba16Stride, inWidth, inHeight, 10);
  }
}

void SparkyuvRotate180(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> rgbaData;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rotatedStride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rotated(rotatedStride * inHeight);

  for (auto _ : state) {
    sparkyuv::RotateRGBA(reinterpret_cast<uint8_t *>(rgbaData.data()), rgbaStride,
                         rotated.data(), rotatedStride, inWidth, inHeight, sparkyuv::sRotate180);
  }
}

void SparkyuvFastGuassianRGBA(benchmark::State &state) {
  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;

  std::vector<uint8_t> rgbaData;
  int rgbaStride;
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return;
  }

  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);

  for (auto _ : state) {
    sparkyuv::FastGaussianBlurRGBA(reinterpret_cast<uint8_t *>(rgbaData.data()), rgbaStride, inWidth, inHeight, 15);
  }
}
