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

#ifndef YUV_TOOLS_BENCH_YUVBENCHMARKNV_H_
#define YUV_TOOLS_BENCH_YUVBENCHMARKNV_H_

#include <benchmark/benchmark.h>

void SparkyuvRGBA8ToNV21(benchmark::State &state);
void SparkyuvNV21ToRGBA8(benchmark::State &state);
void LibyuvNV21ToRGBA8(benchmark::State &state);
void SparkyuvRGBA8ToNV12(benchmark::State &state);
void SparkyuvNV12ToRGBA8(benchmark::State &state);
void LibyuvNV12ToRGBA8(benchmark::State &state);
void SparkyuvRGBA8ToNV16(benchmark::State &state);
void SparkyuvNV16ToRGBA8(benchmark::State &state);
void SparkyuvRGBA8ToNV24(benchmark::State &state);
void SparkyuvNV24ToRGBA8(benchmark::State &state);

#endif //YUV_TOOLS_BENCH_YUVBENCHMARKNV_H_
