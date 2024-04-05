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

#if defined(SPARKYUV__EOTF_INL_H) == defined(HWY_TARGET_TOGGLE)
#ifdef SPARKYUV__EOTF_INL_H
#undef SPARKYUV__EOTF_INL_H
#else
#define SPARKYUV__EOTF_INL_H
#endif

#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sparkyuv-internal.h"
#include "ITUR/eotf-inl.h"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {
EOTF_INLINE float Oetf(float x, const SparkYuvTransferFunction transferFunction) {
  switch (transferFunction) {
    case TransferBT709:return Rec709Oetf(x);
    case TransferBT470M:return Curve470MOetf(x);
    case TransferBT470BG:return Curve470BGOetf(x);
    case TransferBT601:return Rec709Oetf(x);
    case TransferSMPTE240:return Smpte240Oetf(x);
    case TransferLinear:return x;
    case TransferLog100:return Log100Oetf(x);
    case TransferLog100Sqrt10:return Log100Sqrt10Oetf(x);
    case TransferIEC61966:return Iec61966Oetf(x);
    case TransferBT1361:return Bt1361Oetf(x);
    case TransferSRGB:
    case TransferBT202010Bit:
    case TransferBT202012Bit:return SRGBOetf(x);
    case TransferPQ:return PQOetf(x);
    case TransferSMPTE428:return Smpte240Oetf(x);
    case TransferHLG:return HLGOetf(x);
  }
  return x;
}

template<typename D, typename V = Vec<D>>
EOTF_INLINE V Oetf(D d, V x, const SparkYuvTransferFunction transferFunction) {
  switch (transferFunction) {
    case TransferBT709:return Rec709Oetf(d, x);
    case TransferBT470M:return Curve470MOetf(d, x);
    case TransferBT470BG:return Curve470BGOetf(d, x);
    case TransferBT601:return Rec709Oetf(d, x);
    case TransferSMPTE240:return Smpte240Oetf(d, x);
    case TransferLinear:return x;
    case TransferLog100:return Log100Oetf(d, x);
    case TransferLog100Sqrt10:return Log100Sqrt10Oetf(d, x);
    case TransferIEC61966:return Iec61966Oetf(d, x);
    case TransferBT1361:return Bt1361Oetf(d, x);
    case TransferSRGB:
    case TransferBT202010Bit:
    case TransferBT202012Bit:return SRGBOetf(d, x);
    case TransferPQ:return PQOetf(d, x);
    case TransferSMPTE428:return Smpte240Oetf(d, x);
    case TransferHLG:return HLGOetf(d, x);
  }
  return x;
}

template<typename D, typename V = Vec<D>>
EOTF_INLINE V Eotf(D d, V x, const SparkYuvTransferFunction transferFunction) {
  switch (transferFunction) {
    case TransferBT709:return Rec709Eotf(d, x);
    case TransferBT470M:return Curve470MEotf(d, x);
    case TransferBT470BG:return Curve470BGEotf(d, x);
    case TransferBT601:return Rec601Eotf(d, x);
    case TransferSMPTE240:return Smpte240Eotf(d, x);
    case TransferLinear:return x;
    case TransferLog100:return Log100Eotf(d, x);
    case TransferLog100Sqrt10:return Log100Sqrt10Eotf(d, x);
    case TransferIEC61966:return Iec61966Eotf(d, x);
    case TransferBT1361:return Bt1361Eotf(d, x);
    case TransferSRGB:
    case TransferBT202010Bit:
    case TransferBT202012Bit:return SRGBEotf(d, x);
    case TransferPQ:return PQEotf(d, x, 203.f);
    case TransferSMPTE428:return Smpte240Eotf(d, x);
    case TransferHLG:return HLGEotf(d, x);
  }
  return x;
}

EOTF_INLINE float Eotf(float x, const SparkYuvTransferFunction transferFunction) {
  switch (transferFunction) {
    case TransferBT709:return Rec709Eotf(x);
    case TransferBT470M:return Curve470MEotf(x);
    case TransferBT470BG:return Curve470BGEotf(x);
    case TransferBT601:return Rec601Eotf(x);
    case TransferSMPTE240:return Smpte240Eotf(x);
    case TransferLinear:return x;
    case TransferLog100:return Log100Eotf(x);
    case TransferLog100Sqrt10:return Log100Sqrt10Eotf(x);
    case TransferIEC61966:return Iec61966Eotf(x);
    case TransferBT1361:return Bt1361Eotf(x);
    case TransferSRGB:
    case TransferBT202010Bit:
    case TransferBT202012Bit:return SRGBEotf(x);
    case TransferPQ:return PQEotf(x, 203.f);
    case TransferSMPTE428:return Smpte240Eotf(x);
    case TransferHLG:return HLGEotf(x);
  }
  return x;
}

}
HWY_AFTER_NAMESPACE();
#endif