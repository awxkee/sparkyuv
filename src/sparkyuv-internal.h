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

#pragma once

#include <exception>
#include <string>
#include <algorithm>
#include <cmath>
#include <stdexcept>

#define SPARKYUV_RESTRICT __restrict__

namespace sparkyuv {

enum SparkYuvNVLoadOrder {
  YUV_ORDER_UV,
  YUV_ORDER_VU
};

struct SparkYuvTransformMatrix {
  float Y1;
  float Y2;
  float Y3;
  float U1;
  float U2;
  float U3;
  float V1;
  float V2;
  float V3;
};

static SparkYuvTransformMatrix kRGBToYIQMatrix = {.Y1 = 0.299f, .Y2 = 0.587f, .Y3 = 0.114f,
    .U1 = 0.5959f, .U2 = -0.2746f, .U3 = -0.3213f,
    .V1 = 0.2115f, .V2 = -0.5227, .V3 = 0.3112f};

static SparkYuvTransformMatrix kYIQToRGBMatrix = {.Y1 = 1.f, .Y2 = 0.956f, .Y3 = 0.619f,
    .U1 = 1.f, .U2 = -0.272f, .U3 = -0.647f,
    .V1 = 1.f, .V2 = -1.106f, .V3 = 1.703f};

static SparkYuvTransformMatrix kRGBToYDbDrMatrix = {.Y1 = 0.299f, .Y2 = 0.587f, .Y3 = 0.114f,
    .U1 = -0.450f, .U2 = -0.883f, .U3 = 1.333f,
    .V1 = -1.333f, .V2 = 1.116f, .V3 = 0.217f};

static SparkYuvTransformMatrix kYDbDrToRGBMatrix = {.Y1 = 1.f, .Y2 = 0.0000923037161479f, .Y3 = -0.52591263066186528545f,
    .U1 = 1.f, .U2 = -0.1291328988905095f, .U3 = 0.26789932820759879925f,
    .V1 = 1.f, .V2 = 0.6646790599789547528f, .V3 = -0.000079202543533f};

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
    const auto maxColors = static_cast<uint16_t>(::powf(2.f, static_cast<float>(bitDepth)) - 1.f);
    rangeY = maxColors;
    rangeUV = maxColors;
  } else {
    std::string errorMessage = "Yuv Color Range must be valid parameter";
    throw std::runtime_error(errorMessage);
  }
}

}