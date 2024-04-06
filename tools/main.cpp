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
#include "JPEGEncoder.h"
#include "libyuv.h"
#include <chrono>
#include "JPEGDecoder.h"
#include "YuvReader.h"
#include "bench.h"

constexpr const char *ANSI_COLOR_RED = "\033[1;31m";
constexpr const char *ANSI_COLOR_GREEN = "\033[1;32m";
constexpr const char *ANSI_COLOR_YELLOW = "\033[1;33m";
constexpr const char *ANSI_COLOR_RESET = "\033[0m";
constexpr const char *ANSI_COLOR_BLUE = "\033[1;34m";
constexpr const char *ANSI_COLOR_MAGENTA = "\033[0;35m";  // Magenta
constexpr const char *ANSI_COLOR_CYAN = "\033[0;36m";     // Cyan
constexpr const char *ANSI_COLOR_LIGHT_GRAY = "\033[0;37m"; // Light Gray
constexpr const char *ANSI_COLOR_DARK_GRAY = "\033[1;30m"; // Dark Gray
constexpr const char *ANSI_COLOR_LIGHT_RED = "\033[1;31m"; // Light Red
constexpr const char *ANSI_COLOR_LIGHT_GREEN = "\033[1;32m"; // Light Green

int main() {
  std::cout << "Hello, World!" << std::endl;

  std::vector<uint8_t> inSrcData;
  int inWidth, inHeight;
  std::string filename = "filirovska.jpeg";
  if (!sparkyuv::decompressJPEG(filename, inSrcData, inWidth, inHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return -1;
  }

  std::vector<uint8_t> yPlane;
  std::vector<uint8_t> uPlane;
  std::vector<uint8_t> vPlane;
  const int width = 1629;
  const int height = 1080;
  if (!readYUV420File("filirovska_420_pc.yuv", width, height, yPlane, uPlane, vPlane)) {
    std::cout << "Cannot read file (((" << std::endl;
  }

  std::vector<uint8_t> yNVPlane;
  std::vector<uint8_t> uvPlane;

  const int nvWidth = 1629;
  const int nvHeight = 1080;

  if (!readNV16YUvFile("filirovska_nv16.yuv", yNVPlane, uvPlane, nvWidth, nvHeight)) {
    std::cout << "Cannot read file (((" << std::endl;
    return -1;
  }

  const size_t rgbaNVStride = sizeof(uint8_t) * nvWidth * 4;
  std::vector<uint8_t> rgbaNVData(rgbaNVStride * nvHeight);
//  libyuv::NV21ToARGB(yNVPlane.data(), nvWidth, uvPlane.data(), ((nvWidth + 1) / 2) * 2, rgbaData.data(), rgbaStride, nvWidth, nvHeight);

  yPlane.clear();
  uPlane.clear();
  vPlane.clear();
//  int yPlaneStride = width;
//  int uvPlaneStride = (width + 1)/2;
//  yPlane.resize(yPlaneStride * height);
//  uPlane.resize(uvPlaneStride * (height + 1) / 2);
//  vPlane.resize(uvPlaneStride * (height + 1) / 2);

  int yPlaneStride = width;
  int uvPlaneStride = width;
  int uvPlaneHeight = height;
  yPlane.resize(yPlaneStride * height);
  uPlane.resize(uvPlaneStride * uvPlaneHeight);
  vPlane.resize(uvPlaneStride * uvPlaneHeight);

  int uvPlaneStride16 = (width + 1) / 2;
  int uvPlaneHeight16 = height;

  std::vector<uint8_t> yPlane16;
  std::vector<uint8_t> uPlane16;
  std::vector<uint8_t> vPlane16;
  yPlane16.resize(yPlaneStride * height * sizeof(uint16_t));
  uPlane16.resize(uvPlaneStride16 * uvPlaneHeight16 * sizeof(uint16_t));
  vPlane16.resize(uvPlaneStride16 * uvPlaneHeight16 * sizeof(uint16_t));

  bench(1, ANSI_COLOR_YELLOW, "NV21 To RGBA", [&]() {
//    yNVPlane.resize(nvWidth * nvHeight);
    const auto uvPlaneStride = ((width + 1) / 2) * 2; //((width + 1) / 2) * 2;
    uvPlane.resize(uvPlaneStride * nvHeight);
//
//    sparkyuv::RGBToNV24BT601(inSrcData.data(), inWidth * sizeof(uint8_t) * 3, inWidth, inHeight, yNVPlane.data(),
//                             nvWidth, uvPlane.data(), uvPlaneStride);
    sparkyuv::NV16BT601ToRGBA(rgbaNVData.data(),
                              rgbaNVStride,
                              nvWidth,
                              nvHeight,
                              yNVPlane.data(),
                              nvWidth,
                              uvPlane.data(),
                              uvPlaneStride);
  });

//
//  for (int i = 0; i < 1; ++i) {
//    auto start = std::chrono::high_resolution_clock::now();
//    sparkyuv::NV21ToRGBA(rgbaData.data(),
//                         rgbaStride,
//                         nvWidth,
//                         nvHeight,
//                         yNVPlane.data(),
//                         nvWidth,
//                         uvPlane.data(),
//                         ((nvWidth + 1) / 2) * 2);
//
//    auto end = std::chrono::high_resolution_clock::now();
//
//    // Calculate duration
//    std::chrono::duration<double> duration = end - start;
//
//    // Output duration in milliseconds
//    std::cout << ANSI_COLOR_GREEN << "Execution time fast yuv: " << ANSI_COLOR_RESET << duration.count() * 1000
//              << " milliseconds" << std::endl;
//  }

//  const int permuteMap[4] = {2, 1, 0, 3};
//  SwapChannelsRGBA(rgbaData.data(), rgbaStride, rgbaData.data(), rgbaStride, nvWidth, nvHeight, permuteMap);

//  aire::JPEGEncoder encoder(rgbaData.data(), rgbaStride, nvWidth, nvHeight);
//  auto encoded = encoder.encode();
//  saveVectorToFile(encoded, "jpeg.jpeg");
  const size_t rgbaStride = sizeof(uint8_t) * width * 4;
  std::vector<uint8_t> rgbaData(rgbaStride * height);

  sparkyuv::RGBToRGBA(inSrcData.data(), inWidth * sizeof (uint8_t) * 3, rgbaData.data(), rgbaStride, inWidth, inHeight);

  bench(1, ANSI_COLOR_GREEN, "RGBA -> YUV420", [&]() {
    sparkyuv::RGBA8ToYIQ444P8(rgbaData.data(), rgbaStride, width, height,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_PC);
  });


  bench(1, ANSI_COLOR_GREEN, "YUV420 -> RGB", [&]() {
    sparkyuv::YIQ444P8ToRGBA8(rgbaData.data(), rgbaStride, width, height,
                                yPlane.data(), yPlaneStride,
                                uPlane.data(), uvPlaneStride,
                                vPlane.data(), uvPlaneStride, sparkyuv::YUV_RANGE_PC);
  });

//  RGBToRGBA(inSrcData.data(), inWidth * sizeof(uint8_t)* 3, rgbaData.data(), inWidth*4* sizeof(uint8_t), width, height);

//  for (int i = 0; i < 5; ++i) {
//    auto start = std::chrono::high_resolution_clock::now();
//    libyuv::U422ToABGR(yPlane.data(),
//                       width,
//                       uPlane.data(),
//                       (width + 1) / 2,
//                       vPlane.data(),
//                       (width + 1) / 2,
//                       rgbaData.data(),
//                       rgbaStride,
//                       width,
//                       height);
//    auto end = std::chrono::high_resolution_clock::now();
//
//    // Calculate duration
//    std::chrono::duration<double> duration = end - start;
//
//    // Output duration in milliseconds
//    std::cout << "Execution time libyuv: " << duration.count() * 1000 << " milliseconds" << std::endl;
//  }

  std::vector<uint8_t> rgba16Data(sizeof(uint16_t) * inWidth * 4 * inHeight);

//
//  sparkyuv::WideRGBA8To10(rgbaData.data(),
//                          inWidth * 4 * sizeof(uint8_t),
//                          reinterpret_cast<uint16_t *>(rgba16Data.data()),
//                          inWidth * 4 * sizeof(uint16_t),
//                          inWidth,
//                          inHeight);
//  bench(1, ANSI_COLOR_BLUE, "RGBA10 -> YUV444 P10", [&] () {
//    sparkyuv::RGBA10ToYCgCoR422P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
//                                inWidth * 4 * sizeof(uint16_t),
//                                inWidth,
//                                inHeight,
//                                reinterpret_cast<uint16_t *>(yPlane16.data()),
//                                yPlaneStride * sizeof(uint16_t),
//                                reinterpret_cast<uint16_t *>(uPlane16.data()),
//                                uvPlaneStride16 * sizeof(uint16_t),
//                                reinterpret_cast<uint16_t *>(vPlane16.data()),
//                                uvPlaneStride16 * sizeof(uint16_t), sparkyuv::YCGCO_RE);
//  });
//  bench(1, ANSI_COLOR_BLUE, "RGBA10 -> YUV444 P10", [&] () {
////    sparkyuv::YUV420P12ToRGBA12(reinterpret_cast<uint16_t *>(rgba16Data.data()),
////                                     inWidth * 4 * sizeof(uint16_t),
////                                     inWidth,
////                                     inHeight,
////                                     reinterpret_cast<uint16_t *>(yPlane16.data()),
////                                     yPlaneStride * sizeof(uint16_t),
////                                     reinterpret_cast<uint16_t *>(uPlane16.data()),
////                                     uvPlaneStride16 * sizeof(uint16_t),
////                                     reinterpret_cast<uint16_t *>(vPlane16.data()),
////                                     uvPlaneStride16 * sizeof(uint16_t), sparkyuv::YUV_RANGE_TV);
//    sparkyuv::YCgCoR422P10ToRGBA10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
//                                inWidth * 4 * sizeof(uint16_t),
//                                inWidth,
//                                inHeight,
//                                reinterpret_cast<uint16_t *>(yPlane16.data()),
//                                yPlaneStride * sizeof(uint16_t),
//                                reinterpret_cast<uint16_t *>(uPlane16.data()),
//                                uvPlaneStride16 * sizeof(uint16_t),
//                                reinterpret_cast<uint16_t *>(vPlane16.data()),
//                                uvPlaneStride16 * sizeof(uint16_t), sparkyuv::YCGCO_RE);
//  });
//
//  SaveYUV444ToFile(yPlane16.data(), uPlane16.data(), vPlane16.data(), inWidth, inHeight, "filirovska_s_444_p10.yuv",
//                   sizeof(uint16_t));

//  if (!readYUV444P16File("filirovska_444_p10.yuv", width, height, yPlane16, uPlane16, vPlane16)) {
//    std::cout << "Cannot read file (((" << std::endl;
//  }

//  for (int i = 0; i < 5; ++i) {
//    auto start = std::chrono::high_resolution_clock::now();
//    libyuv::I010ToABGR(     reinterpret_cast<uint16_t *>(yPlane16.data()), yPlaneStride * sizeof(uint16_t),
//                            reinterpret_cast<uint16_t *>(uPlane16.data()), uvPlaneStride * sizeof(uint16_t),
//                            reinterpret_cast<uint16_t *>(vPlane16.data()), uvPlaneStride * sizeof(uint16_t),
//                            reinterpret_cast<uint8_t *>(rgba16Data.data()), rgbaStride, width, height);
//
//    auto end = std::chrono::high_resolution_clock::now();
//
//    // Calculate duration
//    std::chrono::duration<double> duration = end - start;
//
//    // Output duration in milliseconds
//    std::cout << ANSI_COLOR_RED << "Execution time LIBYUV P10 -> RGB: " << ANSI_COLOR_RESET << duration.count() * 1000
//              << " milliseconds" << std::endl;
//  }
//
//  bench(1, ANSI_COLOR_YELLOW, "YUV P10 -> RGB", [&]() {
//    sparkyuv::YUV444P10ToRGBA(reinterpret_cast<uint16_t *>(rgba16Data.data()), rgbaStride, width, height,
//                              reinterpret_cast<uint16_t *>(yPlane16.data()), yPlaneStride * sizeof(uint16_t),
//                              reinterpret_cast<uint16_t *>(uPlane16.data()), uvPlaneStride * sizeof(uint16_t),
//                              reinterpret_cast<uint16_t *>(vPlane16.data()), uvPlaneStride * sizeof(uint16_t),
//                              0.299f, 0.114f, sparkyuv::YUV_RANGE_TV);
//  });
//
//  bench(1, ANSI_COLOR_MAGENTA, "RGBA16ToRGBAF16", [&]() {
//    sparkyuv::RGBA16ToRGBAF16(reinterpret_cast<uint16_t *>(rgba16Data.data()), inWidth * 4 * sizeof(uint16_t),
//                              reinterpret_cast<uint16_t *>(rgba16Data.data()), inWidth * 4 * sizeof(uint16_t), inWidth, inHeight, 10);
//
//    sparkyuv::RGBAF16ToRGBA16(reinterpret_cast<uint16_t *>(rgba16Data.data()), inWidth * 4 * sizeof(uint16_t),
//                              reinterpret_cast<uint16_t *>(rgba16Data.data()), inWidth * 4 * sizeof(uint16_t), inWidth, inHeight, 10);
//  });
//

//  std::fill(rgbaData.begin(), rgbaData.end(), 0);

//  sparkyuv::SaturateRGBA10To8(reinterpret_cast<uint16_t *>(rgba16Data.data()), inWidth * 4 * sizeof(uint16_t),
//                              rgbaData.data(), inWidth * 4 * sizeof(uint8_t), inWidth, inHeight);

  aire::JPEGEncoder encoder(rgbaData.data(), rgbaStride, width, height);
  auto encoded = encoder.encode();
  saveVectorToFile(encoded, "jpeg.jpeg");

  aire::JPEGEncoder encoderNV(rgbaNVData.data(), rgbaNVStride, width, height);
  auto encodedNV = encoderNV.encode();
  saveVectorToFile(encodedNV, "jpeg_nv.jpeg");
  return 0;
}
