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
enum SparkYuvColorRange {
  YUV_RANGE_TV = 1,
  YUV_RANGE_PC = 2
};

enum SparkYuvSampler {
  bilinear = 1,
  nearest = 2,
  cubic = 3,
  mitchell = 4,
  lanczos = 5,
  catmullRom = 6,
  hermite = 7,
  bSpline = 8,
  bicubic = 9,
  box = 10
};
}