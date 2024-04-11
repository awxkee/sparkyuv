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

namespace sparkyuv {

typedef float (*ScaleWeightSampler)(float);

template<typename T>
class ScaleRowSampler {
 public:
  ScaleRowSampler(const T *mSource,
                  const int srcStride,
                  const int inputWidth,
                  const int inputHeight,
                  T *mDestination,
                  const int dstStride,
                  const int outputWidth,
                  const int outputHeight) : mSource(mSource),
                                            srcStride(srcStride),
                                            inputWidth(inputWidth),
                                            inputHeight(inputHeight),
                                            mDestination(mDestination),
                                            dstStride(dstStride),
                                            outputWidth(outputWidth),
                                            outputHeight(outputHeight) {
    xScale = static_cast<float>(inputWidth) / static_cast<float>(outputWidth);
    yScale = static_cast<float>(inputHeight) / static_cast<float>(outputHeight);
  }

  virtual void sample(int row) = 0;

  virtual ~ScaleRowSampler() = default;

 public:
  const T *mSource;
  const int srcStride;
  const int inputWidth;
  const int inputHeight;
  T *mDestination;
  const int dstStride;
  const int outputWidth;
  const int outputHeight;

  float xScale;
  float yScale;
};
}