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

#ifndef YUV_TOOLS_BENCH_YUVBENCHMARK10_H_
#define YUV_TOOLS_BENCH_YUVBENCHMARK10_H_

#include <benchmark/benchmark.h>

void SparkyuvYCbCr444P10ToRGBA10(benchmark::State &state);
void LibYuvYCbCr444P10ToRGBA8(benchmark::State &state);
void SparkyuvYCbCr422P10ToRGBA10(benchmark::State &state);
void LibYuvYCbCr422P10ToRGBA8(benchmark::State &state);
void SparkyuvYCbCr420P10ToRGBA10(benchmark::State &state);
void LibYuvYCbCr420P10ToRGBA8(benchmark::State &state);
void SparkyuvRGBAP10ToYCbCr420P10(benchmark::State &state);
void SparkyuvRGBA10ToYCbCr422P10(benchmark::State &state);
void SparkyuvRGBA10ToYCbCr444P10(benchmark::State &state);

#endif //YUV_TOOLS_BENCH_YUVBENCHMARK10_H_
