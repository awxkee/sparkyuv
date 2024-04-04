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

//
// Created by Radzivon Bartoshyk on 24/03/2024.
//

#pragma once

#include <exception>
#include <string>

#define SPARKYUV_RESTRICT __restrict__

namespace sparkyuv {

enum SparkYuvNVLoadOrder {
  YUV_ORDER_UV,
  YUV_ORDER_VU
};

enum SparkYuvDefaultPixelType {
  PIXEL_RGBA,
  PIXEL_RGB,
  PIXEL_BGR,
  PIXEL_BGRA,
  PIXEL_ARGB,
  PIXEL_ABGR
};

enum SparkYuvChromaSubsample {
  YUV_SAMPLE_444,
  YUV_SAMPLE_422,
  YUV_SAMPLE_420
};

enum SparkYuvReformatPixelType {
  REFORMAT_RGBA,
  REFORMAT_RGB,
  REFORMAT_BGR,
  REFORMAT_BGRA,
  REFORMAT_ARGB,
  REFORMAT_ABGR,
  REFORMAT_RGBA1010102
};

enum SparkYuvSurfaceChannels {
  SURFACE_CHANNEL,
  SURFACE_CHANNELS_3,
  SURFACE_CHANNELS_4,
  SURFACE_RGBA1010102
};

enum BoxSamplerPixType {
  BOX_UINT8,
  BOX_UINT16,
  BOX_FLOAT16,
  BOX_RGBA1010102
};

static void GetYUVRange(const SparkYuvColorRange yuvColorRange, const int bitDepth,
                        uint16_t &biasY, uint16_t &biasUV,
                        uint16_t &rangeY, uint16_t &rangeUV) {
  if (yuvColorRange == YUV_RANGE_TV) {
    biasY = static_cast<uint16_t>(16 << (static_cast<uint16_t>(bitDepth) - 8));
    biasUV = static_cast<uint16_t>(1 << (static_cast<uint16_t>(bitDepth) - 1));
    rangeY = static_cast<uint16_t>((219 << (static_cast<uint16_t>(bitDepth) - 8)));
    rangeUV = static_cast<uint16_t>((224 << (static_cast<uint16_t>(bitDepth) - 8)));
  } else if (yuvColorRange == YUV_RANGE_PC) {
    biasY = 0;
    biasUV = static_cast<uint16_t>(1 << (static_cast<uint16_t>(bitDepth) - 1));
    const auto maxColors = static_cast<uint16_t>(std::powf(2.f, static_cast<float>(bitDepth)) - 1.f);
    rangeY = maxColors;
    rangeUV = maxColors;
  } else {
    std::string errorMessage = "Yuv Color Range must be valid parameter";
    throw std::runtime_error(errorMessage);
  }
}

}