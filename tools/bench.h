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

#include <string>
#include <functional>

static void bench(int iterations, const char *color, const char *mark, const std::function<void()> &func) {
  double totalTime = 0;
  for (int i = 0; i < iterations; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = end - start;
    totalTime += duration.count();
    // Output duration in milliseconds
    std::cout << color << "Execution time for: " << mark << " - " << "\033[0m" << "\033[1;32m"
              << duration.count() * 1000
              << color << " milliseconds" << "\033[0m" << std::endl;
  }
  std::cout << "\033[1;31m" << "Average time for: " << mark << " - \033[0m" << "\033[1;32m"
            << (totalTime / iterations) * 1000
            << "\033[1;31m" << " milliseconds" << "\033[0m" << std::endl;
}

static void benchRoundTrip(int iterations,
                           const char *color,
                           const char *mark,
                           const std::function<void()> &forward,
                           const std::function<void()> &rollback) {
  double totalTime = 0;
  for (int i = 0; i < iterations; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    forward();
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = end - start;
    totalTime += duration.count();
    // Output duration in milliseconds
    std::cout << color << "Execution time for: " << mark << " - " << "\033[0m" << "\033[1;32m"
              << duration.count() * 1000
              << color << " milliseconds" << "\033[0m" << std::endl;
    rollback();
  }
  std::cout << "\033[1;31m" << "Average time for: " << mark << " - \033[0m" << "\033[1;32m"
            << (totalTime / iterations) * 1000
            << "\033[1;31m" << " milliseconds" << "\033[0m" << std::endl;
}