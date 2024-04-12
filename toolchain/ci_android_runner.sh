#!/bin/bash
#
# Copyright (C) 2024 Radzivon Bartoshyk
#
# This file belongs to sparkyuv project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

ABI_LIST="armeabi-v7a arm64-v8a x86 x86_64"

for abi in ${ABI_LIST}; do
  rm -rf "build-${abi}"
  mkdir "build-${abi}"
  cd "build-${abi}"

  cmake .. \
    -G "Ninja" \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake -DANDROID_ABI=${abi} -DCMAKE_ANDROID_ARCH_ABI=${abi} \
    -DANDROID_NDK=${ANDROID_NDK_HOME} \
    -DANDROID_PLATFORM=android-21 \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED=OFF \
    -DCMAKE_SYSTEM_NAME=Generic \
    -DCMAKE_ANDROID_STL_TYPE=c++_shared \
    -DCMAKE_SYSTEM_NAME=Android \
    -DANDROID=TRUE
  ninja -j 8
  cd ..
done