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

#ifndef YUV_INCLUDE_SPARKYUV_EOTF_H_
#define YUV_INCLUDE_SPARKYUV_EOTF_H_

namespace sparkyuv {
enum SparkYuvTransferFunction {
  TransferBT709 = 1, // BT.709
  TransferBT470M = 2, // BT.470-6 System M
  TransferBT470BG, // BT.470-6 System BG
  TransferBT601, // BT.601", avifToLinear709
  TransferSMPTE240, // SMPTE 240M
  TransferLinear, // Linear
  TransferLog100, // 100:1 Log
  TransferLog100Sqrt10, // 100sqrt(10):1 Log
  TransferIEC61966, // IEC 61966-2-4
  TransferBT1361, // BT.1361
  TransferSRGB, // sRGB
  TransferBT202010Bit, // 10bit BT.2020
  TransferBT202012Bit, // 12bit BT.2020
  TransferPQ, // SMPTE ST 2084 (PQ)
  TransferSMPTE428, // SMPTE ST 428-1
  TransferHLG // ARIB STD-B67 (HLG)
};
}

#endif //YUV_INCLUDE_SPARKYUV_EOTF_H_
