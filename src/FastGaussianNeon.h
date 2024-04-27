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

#ifndef YUV_SRC_FASTGAUSSIANNEON_H_
#define YUV_SRC_FASTGAUSSIANNEON_H_

#if __aarch64__

#include <cstdint>

namespace sparkyuv {
void VerticalGaussianPassRGBANeon(uint8_t *data,
                                  uint32_t stride,
                                  uint32_t width,
                                  uint32_t height,
                                  int radius,
                                  uint32_t start,
                                  uint32_t end);

void HorizontalGaussianPassRGBANeon(uint8_t *data,
                                    uint32_t stride,
                                    uint32_t width,
                                    uint32_t height,
                                    int radius,
                                    uint32_t start,
                                    uint32_t end);
}

#endif

#endif //YUV_SRC_FASTGAUSSIANNEON_H_
