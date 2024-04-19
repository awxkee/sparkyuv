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


#ifndef YUV_SRC_MATH_GAUSSIAN_H_
#define YUV_SRC_MATH_GAUSSIAN_H_

#include <vector>
#include <cmath>
#include <algorithm>

namespace {
using namespace std;
vector<float> Get1DGaussianKernel(int width, float sigma) {
  vector<float> kernel(ceil(width));
  int mean = width / 2;
  float sum = 0.f;
  const float scale = 1.f / (::sqrtf(2 * M_PI) * sigma);
  for (int x = 0; x < width; x++) {
    kernel[x] = ::expf(-0.5f * ::powf(static_cast<float>(x - mean) / sigma, 2.0f)) * scale;
    sum += kernel[x];
  }
  if (sum != 0.f) {
    for (int x = 0; x < width; x++)
      kernel[x] /= sum;
  }
  return std::move(kernel);
}
}

#endif //YUV_SRC_MATH_GAUSSIAN_H_
