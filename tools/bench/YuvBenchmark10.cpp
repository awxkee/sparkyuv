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
#include "libyuv.h"

static std::string filename = "filirovska.jpeg";

void SparkyuvYCbCr444P10ToRGBA10(benchmark::State &state) {
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
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);

  sparkyuv::RGBA10ToYCbCr444P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                rgba16Stride,
                                inWidth,
                                inHeight,
                                reinterpret_cast<uint16_t *>(yPlane.data()),
                                yPlaneStride,
                                reinterpret_cast<uint16_t *>(uPlane.data()),
                                uvPlaneStride,
                                reinterpret_cast<uint16_t *>(vPlane.data()),
                                uvPlaneStride,
                                0.299f,
                                0.114f,
                                sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YCbCr444P10ToRGBA10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                  rgba16Stride,
                                  inWidth,
                                  inHeight,
                                  reinterpret_cast<uint16_t *>(yPlane.data()),
                                  yPlaneStride,
                                  reinterpret_cast<uint16_t *>(uPlane.data()),
                                  uvPlaneStride,
                                  reinterpret_cast<uint16_t *>(vPlane.data()),
                                  uvPlaneStride,
                                  0.299f,
                                  0.114f,
                                  sparkyuv::YUV_RANGE_TV);
  }
}

void LibYuvYCbCr444P10ToRGBA8(benchmark::State &state) {
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
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);

  sparkyuv::RGBA10ToYCbCr444P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                rgba16Stride,
                                inWidth,
                                inHeight,
                                reinterpret_cast<uint16_t *>(yPlane.data()),
                                yPlaneStride,
                                reinterpret_cast<uint16_t *>(uPlane.data()),
                                uvPlaneStride,
                                reinterpret_cast<uint16_t *>(vPlane.data()),
                                uvPlaneStride,
                                0.299f,
                                0.114f,
                                sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    libyuv::I410ToARGBMatrix(reinterpret_cast<uint16_t *>(yPlane.data()),
                             yPlaneStride,
                             reinterpret_cast<uint16_t *>(uPlane.data()),
                             uvPlaneStride,
                             reinterpret_cast<uint16_t *>(vPlane.data()),
                             uvPlaneStride,
                             reinterpret_cast<uint8_t *>(rgbaData.data()),
                             rgbaStride,
                             &libyuv::kYuvI601ConstantsVU,
                             inWidth,
                             inHeight);
  }
}

void SparkyuvYCbCr422P10ToRGBA10(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);

  sparkyuv::RGBA10ToYCbCr422P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                rgba16Stride,
                                inWidth,
                                inHeight,
                                reinterpret_cast<uint16_t *>(yPlane.data()),
                                yPlaneStride,
                                reinterpret_cast<uint16_t *>(uPlane.data()),
                                uvPlaneStride,
                                reinterpret_cast<uint16_t *>(vPlane.data()),
                                uvPlaneStride,
                                0.299f,
                                0.114f,
                                sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YCbCr422P10ToRGBA10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                  rgba16Stride,
                                  inWidth,
                                  inHeight,
                                  reinterpret_cast<uint16_t *>(yPlane.data()),
                                  yPlaneStride,
                                  reinterpret_cast<uint16_t *>(uPlane.data()),
                                  uvPlaneStride,
                                  reinterpret_cast<uint16_t *>(vPlane.data()),
                                  uvPlaneStride,
                                  0.299f,
                                  0.114f,
                                  sparkyuv::YUV_RANGE_TV);
  }
}

void LibYuvYCbCr422P10ToRGBA8(benchmark::State &state) {
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
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);

  sparkyuv::RGBA10ToYCbCr422P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                rgba16Stride,
                                inWidth,
                                inHeight,
                                reinterpret_cast<uint16_t *>(yPlane.data()),
                                yPlaneStride,
                                reinterpret_cast<uint16_t *>(uPlane.data()),
                                uvPlaneStride,
                                reinterpret_cast<uint16_t *>(vPlane.data()),
                                uvPlaneStride,
                                0.299f,
                                0.114f,
                                sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    libyuv::I210ToARGBMatrix(reinterpret_cast<uint16_t *>(yPlane.data()),
                             yPlaneStride,
                             reinterpret_cast<uint16_t *>(uPlane.data()),
                             uvPlaneStride,
                             reinterpret_cast<uint16_t *>(vPlane.data()),
                             uvPlaneStride,
                             reinterpret_cast<uint8_t *>(rgbaData.data()),
                             rgbaStride,
                             &libyuv::kYuvI601ConstantsVU,
                             inWidth,
                             inHeight);
  }
}

void SparkyuvYCbCr420P10ToRGBA10(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);

  sparkyuv::RGBA10ToYCbCr420P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                rgba16Stride,
                                inWidth,
                                inHeight,
                                reinterpret_cast<uint16_t *>(yPlane.data()),
                                yPlaneStride,
                                reinterpret_cast<uint16_t *>(uPlane.data()),
                                uvPlaneStride,
                                reinterpret_cast<uint16_t *>(vPlane.data()),
                                uvPlaneStride,
                                0.299f,
                                0.114f,
                                sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    sparkyuv::YCbCr420P10ToRGBA10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                  rgba16Stride,
                                  inWidth,
                                  inHeight,
                                  reinterpret_cast<uint16_t *>(yPlane.data()),
                                  yPlaneStride,
                                  reinterpret_cast<uint16_t *>(uPlane.data()),
                                  uvPlaneStride,
                                  reinterpret_cast<uint16_t *>(vPlane.data()),
                                  uvPlaneStride,
                                  0.299f,
                                  0.114f,
                                  sparkyuv::YUV_RANGE_TV);
  }
}

void LibYuvYCbCr420P10ToRGBA8(benchmark::State &state) {
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
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);

  sparkyuv::RGBA10ToYCbCr420P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                rgba16Stride,
                                inWidth,
                                inHeight,
                                reinterpret_cast<uint16_t *>(yPlane.data()),
                                yPlaneStride,
                                reinterpret_cast<uint16_t *>(uPlane.data()),
                                uvPlaneStride,
                                reinterpret_cast<uint16_t *>(vPlane.data()),
                                uvPlaneStride,
                                0.299f,
                                0.114f,
                                sparkyuv::YUV_RANGE_TV);
  for (auto _ : state) {
    libyuv::I010ToARGBMatrix(reinterpret_cast<uint16_t *>(yPlane.data()),
                             yPlaneStride,
                             reinterpret_cast<uint16_t *>(uPlane.data()),
                             uvPlaneStride,
                             reinterpret_cast<uint16_t *>(vPlane.data()),
                             uvPlaneStride,
                             reinterpret_cast<uint8_t *>(rgbaData.data()),
                             rgbaStride,
                             &libyuv::kYuvI601ConstantsVU,
                             inWidth,
                             inHeight);
  }
}

void SparkyuvRGBAP10ToYCbCr420P10(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);
  for (auto _ : state) {
    sparkyuv::RGBA10ToYCbCr420P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                  rgba16Stride,
                                  inWidth,
                                  inHeight,
                                  reinterpret_cast<uint16_t *>(yPlane.data()),
                                  yPlaneStride,
                                  reinterpret_cast<uint16_t *>(uPlane.data()),
                                  uvPlaneStride,
                                  reinterpret_cast<uint16_t *>(vPlane.data()),
                                  uvPlaneStride,
                                  0.299f,
                                  0.114f,
                                  sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvRGBA10ToYCbCr422P10(benchmark::State &state) {
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

  yPlane.resize(yPlaneStride * inHeight * sizeof(uint16_t));
  uPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  vPlane.resize(uvPlaneStride * uvPlaneHeight * sizeof(uint16_t));
  rgbaStride = sizeof(uint8_t) * inWidth * 4;
  rgbaData.resize(rgbaStride * inHeight);
  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);
  for (auto _ : state) {
    sparkyuv::RGBA10ToYCbCr422P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                  rgba16Stride,
                                  inWidth,
                                  inHeight,
                                  reinterpret_cast<uint16_t *>(yPlane.data()),
                                  yPlaneStride,
                                  reinterpret_cast<uint16_t *>(uPlane.data()),
                                  uvPlaneStride,
                                  reinterpret_cast<uint16_t *>(vPlane.data()),
                                  uvPlaneStride,
                                  0.299f,
                                  0.114f,
                                  sparkyuv::YUV_RANGE_TV);
  }
}

void SparkyuvRGBA10ToYCbCr444P10(benchmark::State &state) {
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
  int rgba16Stride = sizeof(uint8_t) * inWidth * 4;
  std::vector<uint8_t> rgba16Data(rgba16Stride * inHeight);
  sparkyuv::WideRGBA8To10(rgbaData.data(),
                          rgbaStride,
                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
                          rgba16Stride,
                          inWidth,
                          inHeight);
  for (auto _ : state) {
    sparkyuv::RGBA10ToYCbCr444P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                                  rgba16Stride,
                                  inWidth,
                                  inHeight,
                                  reinterpret_cast<uint16_t *>(yPlane.data()),
                                  yPlaneStride,
                                  reinterpret_cast<uint16_t *>(uPlane.data()),
                                  uvPlaneStride,
                                  reinterpret_cast<uint16_t *>(vPlane.data()),
                                  uvPlaneStride,
                                  0.299f,
                                  0.114f,
                                  sparkyuv::YUV_RANGE_TV);
  }
}