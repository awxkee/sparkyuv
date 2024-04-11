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

#include <functional>
#include <queue>
#include <mutex>
#if THREADS_SUPPORTED
#include <thread>
#include <vector>
#endif
#include <type_traits>

namespace concurrency {

template<typename Function>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R(Args...)> {
  using result_type = R;
};

template<typename Function, typename... Args>
void parallel_for(const int numThreads, const size_t numIterations, Function &&func, Args &&... args) {
  static_assert(std::is_invocable_v<Function, int, Args...>, "func must take an int parameter for iteration id");

#if NDEBUG && THREADS_SUPPORTED
  std::vector<std::thread> threads;

  size_t segmentHeight = numIterations / numThreads;

  auto parallelWorker = [&](int start, int end) {
    for (int y = start; y < end; ++y) {
      {
        std::invoke(func, y, std::forward<Args>(args)...);
      }
    }
  };

  if (numThreads > 1) {
    // Launch N-1 worker threads
    for (size_t i = 1; i < numThreads; ++i) {
      size_t start = i * segmentHeight;
      size_t end = (i + 1) * segmentHeight;
      if (i == numThreads - 1) {
        end = numIterations;
      }
      threads.emplace_back(parallelWorker, start, end);
    }
  }

  size_t start = 0;
  size_t end = segmentHeight;
  if (numThreads == 1) {
    end = numIterations;
  }
  parallelWorker(start, end);

  // Join all threads
  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
#else
  for (int i = 0; i < numIterations; ++i) {
    func(i);
  }
#endif
}

template<typename Function, typename... Args>
void parallel_for_with_thread_id(const int numThreads, const int numIterations, Function &&func, Args &&... args) {
  static_assert(std::is_invocable_v<Function, int, int, Args...>,
                "func must take an int parameter for threadId, and iteration Id");
#if THREADS_SUPPORTED
  std::vector<std::thread> threads;

  int segmentHeight = numIterations / numThreads;

  auto parallel_worker = [&](int threadId, int start, int end) {
    for (int y = start; y < end; ++y) {
      {
        std::invoke(func, threadId, y, std::forward<Args>(args)...);
      }
    }
  };

  if (numThreads > 1) {
    // Launch N-1 worker threads
    for (int i = 1; i < numThreads; ++i) {
      int start = i * segmentHeight;
      int end = (i + 1) * segmentHeight;
      if (i == numThreads - 1) {
        end = numIterations;
      }
      threads.emplace_back(parallel_worker, i, start, end);
    }
  }

  int start = 0;
  int end = segmentHeight;
  if (numThreads == 1) {
    end = numIterations;
  }
  parallel_worker(0, start, end);

  // Join all threads
  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
#else
  for (int i = 0; i < numIterations; ++i) {
    func(0, i);
  }
#endif
}
}
