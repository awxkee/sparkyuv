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

#ifndef YUV_TOOLS_BENCH_YUVBENCHMARKBASE_H_
#define YUV_TOOLS_BENCH_YUVBENCHMARKBASE_H_

#include <benchmark/benchmark.h>

void SparkyuvWide8To10Fixed(benchmark::State &state);
void SparkyuvWide8To10Dynamic(benchmark::State &state);
void SparkyuvSaturate10To8Fixed(benchmark::State &state);
void SparkyuvSaturate10To8Dynamic(benchmark::State &state);
void SparkyuvRGB10BitToF16(benchmark::State &state);
void SparkyuvRotate180(benchmark::State &state);
void SparkyuvFastGuassianRGBA(benchmark::State &state);

#endif //YUV_TOOLS_BENCH_YUVBENCHMARKBASE_H_
