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

#include <cstdint>
#include <vector>

namespace aire {
class JPEGEncoder {
 public:
  JPEGEncoder(uint8_t *data, int stride, int width, int height) : data(data), stride(stride),
                                                                  width(width), height(height) {

  }

  std::vector<uint8_t> encode();

  void setQuality(int mQuality) {
    this->quality = mQuality;
  }

  std::vector<uint8_t> encodeRGB();

 private:
  int quality = 81;
  uint8_t *data;
  const int stride;
  const int width;
  const int height;
};
}