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

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "src/Scale.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"
#include "yuv-inl.h"
#include "sampler/NearestRowSampler-inl.hpp"
#include "sampler/BilinearRowSampler-inl.hpp"
#include "sampler/Window4RowSampler-inl.hpp"
#include "sampler/Window6RowSampler-inl.hpp"
#include "sampler/BoxRowSampler-inl.h"
#include "concurrency.hpp"

HWY_BEFORE_NAMESPACE();
namespace sparkyuv::HWY_NAMESPACE {

template<int Components>
void ScaleRGB16OrChannelHWY(const uint16_t *input, const uint32_t srcStride,
                            uint32_t inputWidth, uint32_t inputHeight,
                            uint16_t *output, const uint32_t dstStride,
                            uint32_t outputWidth, uint32_t outputHeight,
                            const int depth, const SparkYuvSampler option) {

  auto src = reinterpret_cast<const uint16_t *>(input);

  std::unique_ptr<ScaleRowSampler<uint16_t>> sampler;
  switch (option) {
    case box: {
      sampler = std::make_unique<BoxSampler<uint16_t, sparkyuv::BOX_UINT16, Components>>(src,
                                                                                         srcStride,
                                                                                         inputWidth,
                                                                                         inputHeight,
                                                                                         output,
                                                                                         dstStride,
                                                                                         outputWidth,
                                                                                         outputHeight);
    }
      break;
    case hermite: {
      sampler = std::make_unique<WeightedWindow4RowSampler16Bit<WEIGHTED_ROW4_HERMITE, Components>>(src,
                                                                                                    srcStride,
                                                                                                    inputWidth,
                                                                                                    inputHeight,
                                                                                                    output,
                                                                                                    dstStride,
                                                                                                    outputWidth,
                                                                                                    outputHeight,
                                                                                                    depth);
    }
      break;
    case catmullRom: {
      sampler = std::make_unique<WeightedWindow4RowSampler16Bit<WEIGHTED_ROW4_CATMULL_ROM, Components>>(src,
                                                                                                        srcStride,
                                                                                                        inputWidth,
                                                                                                        inputHeight,
                                                                                                        output,
                                                                                                        dstStride,
                                                                                                        outputWidth,
                                                                                                        outputHeight,
                                                                                                        depth);
    }
      break;
    case bSpline: {
      sampler = std::make_unique<WeightedWindow4RowSampler16Bit<WEIGHTED_ROW4_BSPLINE, Components>>(src,
                                                                                                    srcStride,
                                                                                                    inputWidth,
                                                                                                    inputHeight,
                                                                                                    output,
                                                                                                    dstStride,
                                                                                                    outputWidth,
                                                                                                    outputHeight,
                                                                                                    depth);
    }
      break;
    case cubic: {
      sampler = std::make_unique<WeightedWindow4RowSampler16Bit<WEIGHTED_ROW4_CUBIC, Components>>(src,
                                                                                                  srcStride,
                                                                                                  inputWidth,
                                                                                                  inputHeight,
                                                                                                  output,
                                                                                                  dstStride,
                                                                                                  outputWidth,
                                                                                                  outputHeight, depth);
    }
      break;
    case bicubic: {
      sampler = std::make_unique<WeightedWindow4RowSampler16Bit<WEIGHTED_ROW4_BICUBIC, Components>>(src,
                                                                                                    srcStride,
                                                                                                    inputWidth,
                                                                                                    inputHeight,
                                                                                                    output,
                                                                                                    dstStride,
                                                                                                    outputWidth,
                                                                                                    outputHeight,
                                                                                                    depth);
    }
      break;
    case mitchell: {
      sampler = std::make_unique<WeightedWindow4RowSampler16Bit<WEIGHTED_ROW4_MITCHELL, Components>>(src,
                                                                                                     srcStride,
                                                                                                     inputWidth,
                                                                                                     inputHeight,
                                                                                                     output,
                                                                                                     dstStride,
                                                                                                     outputWidth,
                                                                                                     outputHeight,
                                                                                                     depth);
    }
      break;
    case lanczos: {
      sampler = std::make_unique<WeightedWindow6RowSampler16Bit<WEIGHTED_ROW6_LANCZOS_SINC, Components>>(src,
                                                                                                         srcStride,
                                                                                                         inputWidth,
                                                                                                         inputHeight,
                                                                                                         output,
                                                                                                         dstStride,
                                                                                                         outputWidth,
                                                                                                         outputHeight,
                                                                                                         depth);
    }
      break;
    case bilinear: {
      sampler = std::make_unique<BilinearRowSamplerAnyBit<uint16_t, Components>>(src,
                                                                                 srcStride,
                                                                                 inputWidth,
                                                                                 inputHeight,
                                                                                 output,
                                                                                 dstStride,
                                                                                 outputWidth,
                                                                                 outputHeight);
    }
      break;
    default: {
      sampler = std::make_unique<NearestRowSampler16Bit<Components>>(src, srcStride,
                                                                     inputWidth,
                                                                     inputHeight,
                                                                     output,
                                                                     dstStride,
                                                                     outputWidth,
                                                                     outputHeight);
    }
      break;
  }

  const int threadCount = std::clamp(std::min(static_cast<int>(std::thread::hardware_concurrency()),
                                              static_cast<int>(outputHeight * outputWidth / (256 * 256))),
                                     static_cast<int>(1), static_cast<int>(12));

  concurrency::parallel_for(threadCount, outputHeight, [&](int iterationId) {
    sampler->sample(iterationId);
  });
}

template<int Components>
void ScaleRGB8OrChannelHWY(const uint8_t *input,
                           const uint32_t srcStride,
                           uint32_t inputWidth, uint32_t inputHeight,
                           uint8_t *output,
                           const uint32_t dstStride,
                           uint32_t outputWidth, uint32_t outputHeight,
                           const SparkYuvSampler option) {

  auto src8 = reinterpret_cast<const uint8_t *>(input);

  std::unique_ptr<ScaleRowSampler<uint8_t>> sampler;
  switch (option) {
    case box: {
      sampler = std::make_unique<BoxSampler<uint8_t, sparkyuv::BOX_UINT8, Components>>(src8,
                                                                                       srcStride,
                                                                                       inputWidth,
                                                                                       inputHeight,
                                                                                       output,
                                                                                       dstStride,
                                                                                       outputWidth,
                                                                                       outputHeight);
    }
      break;
    case hermite: {
      sampler = std::make_unique<WeightedWindow4RowSampler<WEIGHTED_ROW4_HERMITE, Components>>(src8,
                                                                                               srcStride,
                                                                                               inputWidth,
                                                                                               inputHeight,
                                                                                               output,
                                                                                               dstStride,
                                                                                               outputWidth,
                                                                                               outputHeight);
    }
      break;
    case catmullRom: {
      sampler = std::make_unique<WeightedWindow4RowSampler<WEIGHTED_ROW4_CATMULL_ROM, Components>>(src8,
                                                                                                   srcStride,
                                                                                                   inputWidth,
                                                                                                   inputHeight,
                                                                                                   output,
                                                                                                   dstStride,
                                                                                                   outputWidth,
                                                                                                   outputHeight);
    }
      break;
    case bSpline: {
      sampler = std::make_unique<WeightedWindow4RowSampler<WEIGHTED_ROW4_BSPLINE, Components>>(src8,
                                                                                               srcStride,
                                                                                               inputWidth,
                                                                                               inputHeight,
                                                                                               output,
                                                                                               dstStride,
                                                                                               outputWidth,
                                                                                               outputHeight);
    }
      break;
    case cubic: {
      sampler = std::make_unique<WeightedWindow4RowSampler<WEIGHTED_ROW4_CUBIC, Components>>(src8,
                                                                                             srcStride,
                                                                                             inputWidth,
                                                                                             inputHeight,
                                                                                             output,
                                                                                             dstStride,
                                                                                             outputWidth,
                                                                                             outputHeight);
    }
      break;
    case bicubic: {
      sampler = std::make_unique<WeightedWindow4RowSampler<WEIGHTED_ROW4_BICUBIC, Components>>(src8,
                                                                                               srcStride,
                                                                                               inputWidth,
                                                                                               inputHeight,
                                                                                               output,
                                                                                               dstStride,
                                                                                               outputWidth,
                                                                                               outputHeight);
    }
      break;
    case mitchell: {
      sampler = std::make_unique<WeightedWindow4RowSampler<WEIGHTED_ROW4_MITCHELL, Components>>(src8,
                                                                                                srcStride,
                                                                                                inputWidth,
                                                                                                inputHeight,
                                                                                                output,
                                                                                                dstStride,
                                                                                                outputWidth,
                                                                                                outputHeight);
    }
      break;
    case lanczos: {
      sampler = std::make_unique<WeightedWindow6RowSampler<WEIGHTED_ROW6_LANCZOS_SINC, Components>>(src8,
                                                                                                    srcStride,
                                                                                                    inputWidth,
                                                                                                    inputHeight,
                                                                                                    output,
                                                                                                    dstStride,
                                                                                                    outputWidth,
                                                                                                    outputHeight);
    }
      break;
    case bilinear: {
      if (Components == 4) {
        sampler = std::make_unique<BilinearRowSampler4Chan8Bit<Components>>(src8,
                                                                            srcStride,
                                                                            inputWidth,
                                                                            inputHeight,
                                                                            output,
                                                                            dstStride,
                                                                            outputWidth,
                                                                            outputHeight);
      } else {
        sampler = std::make_unique<BilinearRowSamplerAnyBit<uint8_t, Components>>(src8,
                                                                                  srcStride,
                                                                                  inputWidth,
                                                                                  inputHeight,
                                                                                  output,
                                                                                  dstStride,
                                                                                  outputWidth,
                                                                                  outputHeight);
      }
    }
      break;
    default: {
      sampler = std::make_unique<NearestRowSampler<Components>>(src8, srcStride,
                                                                inputWidth,
                                                                inputHeight,
                                                                output,
                                                                dstStride,
                                                                outputWidth,
                                                                outputHeight);
    }
      break;
  }

  const int threadCount = std::clamp(std::min(static_cast<int>(std::thread::hardware_concurrency()),
                                              static_cast<int>(outputHeight * outputWidth / (256 * 256))),
                                     static_cast<int>(1), static_cast<int>(12));

  concurrency::parallel_for(threadCount, outputHeight, [&](int iterationId) {
    sampler->sample(iterationId);
  });
}

void ScaleRGB1010102OrChannelHWY(const uint8_t *input,
                                 const uint32_t srcStride,
                                 uint32_t inputWidth, uint32_t inputHeight,
                                 uint8_t *output,
                                 const uint32_t dstStride,
                                 uint32_t outputWidth, uint32_t outputHeight,
                                 const SparkYuvSampler option) {

  auto src8 = reinterpret_cast<const uint8_t *>(input);

  std::unique_ptr<ScaleRowSampler<uint32_t>> sampler;
  switch (option) {
    case box: {
      sampler =
          std::make_unique<BoxSampler<uint32_t, sparkyuv::BOX_RGBA1010102, 1>>(reinterpret_cast<const uint32_t *>(src8),
                                                                               srcStride,
                                                                               inputWidth,
                                                                               inputHeight,
                                                                               reinterpret_cast<uint32_t *>(output),
                                                                               dstStride,
                                                                               outputWidth,
                                                                               outputHeight);
    }
      break;
    case hermite: {
      sampler =
          std::make_unique<WeightedWindow4RowSampler10Bit<WEIGHTED_ROW4_HERMITE>>(reinterpret_cast<const uint32_t *>(src8),
                                                                                  srcStride,
                                                                                  inputWidth,
                                                                                  inputHeight,
                                                                                  reinterpret_cast<uint32_t *>(output),
                                                                                  dstStride,
                                                                                  outputWidth,
                                                                                  outputHeight);
    }
      break;
    case catmullRom: {
      sampler =
          std::make_unique<WeightedWindow4RowSampler10Bit<WEIGHTED_ROW4_CATMULL_ROM>>(reinterpret_cast<const uint32_t *>(src8),
                                                                                      srcStride,
                                                                                      inputWidth,
                                                                                      inputHeight,
                                                                                      reinterpret_cast<uint32_t *>(output),
                                                                                      dstStride,
                                                                                      outputWidth,
                                                                                      outputHeight);
    }
      break;
    case bSpline: {
      sampler =
          std::make_unique<WeightedWindow4RowSampler10Bit<WEIGHTED_ROW4_BSPLINE>>(reinterpret_cast<const uint32_t *>(src8),
                                                                                  srcStride,
                                                                                  inputWidth,
                                                                                  inputHeight,
                                                                                  reinterpret_cast<uint32_t *>(output),
                                                                                  dstStride,
                                                                                  outputWidth,
                                                                                  outputHeight);
    }
      break;
    case cubic: {
      sampler =
          std::make_unique<WeightedWindow4RowSampler10Bit<WEIGHTED_ROW4_CUBIC>>(reinterpret_cast<const uint32_t *>(src8),
                                                                                srcStride,
                                                                                inputWidth,
                                                                                inputHeight,
                                                                                reinterpret_cast<uint32_t *>(output),
                                                                                dstStride,
                                                                                outputWidth,
                                                                                outputHeight);
    }
      break;
    case bicubic: {
      sampler =
          std::make_unique<WeightedWindow4RowSampler10Bit<WEIGHTED_ROW4_BICUBIC>>(reinterpret_cast<const uint32_t *>(src8),
                                                                                  srcStride,
                                                                                  inputWidth,
                                                                                  inputHeight,
                                                                                  reinterpret_cast<uint32_t *>(output),
                                                                                  dstStride,
                                                                                  outputWidth,
                                                                                  outputHeight);
    }
      break;
    case mitchell: {
      sampler =
          std::make_unique<WeightedWindow4RowSampler10Bit<WEIGHTED_ROW4_MITCHELL>>(reinterpret_cast<const uint32_t *>(src8),
                                                                                   srcStride,
                                                                                   inputWidth,
                                                                                   inputHeight,
                                                                                   reinterpret_cast<uint32_t *>(output),
                                                                                   dstStride,
                                                                                   outputWidth,
                                                                                   outputHeight);
    }
      break;
    case lanczos: {
      sampler =
          std::make_unique<WeightedWindow6RowSampler10Bit<WEIGHTED_ROW6_LANCZOS_SINC>>(reinterpret_cast<const uint32_t *>(src8),
                                                                                       srcStride,
                                                                                       inputWidth,
                                                                                       inputHeight,
                                                                                       reinterpret_cast<uint32_t *>(output),
                                                                                       dstStride,
                                                                                       outputWidth,
                                                                                       outputHeight);
    }
      break;
    case bilinear: {
      sampler = std::make_unique<BilinearRowSampler10Bit>(reinterpret_cast<const uint32_t *>(src8),
                                                          srcStride,
                                                          inputWidth,
                                                          inputHeight,
                                                          reinterpret_cast<uint32_t *>(output),
                                                          dstStride,
                                                          outputWidth,
                                                          outputHeight);
    }
      break;
    default: {
      sampler = std::make_unique<NearestRowSampler10Bit>(reinterpret_cast<const uint32_t *>(src8),
                                                         srcStride,
                                                         inputWidth,
                                                         inputHeight,
                                                         reinterpret_cast<uint32_t *>(output),
                                                         dstStride,
                                                         outputWidth,
                                                         outputHeight);
    }
      break;
  }

  const int threadCount = std::clamp(std::min(static_cast<int>(std::thread::hardware_concurrency()),
                                              static_cast<int>(outputHeight * outputWidth / (256 * 256))),
                                     static_cast<int>(1), static_cast<int>(12));

  concurrency::parallel_for(threadCount, outputHeight, [&](int iterationId) {
    sampler->sample(iterationId);
  });
}

template<int Components>
void ScaleRGB16FOrChannelHWY(const uint16_t *input,
                             const uint32_t srcStride,
                             uint32_t inputWidth, uint32_t inputHeight,
                             uint16_t *output, const uint32_t dstStride,
                             uint32_t outputWidth, uint32_t outputHeight,
                             const SparkYuvSampler option) {

  auto src8 = reinterpret_cast<const uint16_t *>(input);

  std::unique_ptr<ScaleRowSampler<uint16_t>> sampler;
  switch (option) {
    case box: {
      sampler = std::make_unique<BoxSampler<uint16_t, sparkyuv::BOX_FLOAT16, Components>>(src8,
                                                                                          srcStride,
                                                                                          inputWidth,
                                                                                          inputHeight,
                                                                                          output,
                                                                                          dstStride,
                                                                                          outputWidth,
                                                                                          outputHeight);
    }
      break;
    case hermite: {
      sampler = std::make_unique<WeightedWindow4RowSamplerF16Bit<WEIGHTED_ROW4_HERMITE, Components>>(src8,
                                                                                                     srcStride,
                                                                                                     inputWidth,
                                                                                                     inputHeight,
                                                                                                     output,
                                                                                                     dstStride,
                                                                                                     outputWidth,
                                                                                                     outputHeight);
    }
      break;
    case catmullRom: {
      sampler = std::make_unique<WeightedWindow4RowSamplerF16Bit<WEIGHTED_ROW4_CATMULL_ROM, Components>>(src8,
                                                                                                         srcStride,
                                                                                                         inputWidth,
                                                                                                         inputHeight,
                                                                                                         output,
                                                                                                         dstStride,
                                                                                                         outputWidth,
                                                                                                         outputHeight);
    }
      break;
    case bSpline: {
      sampler = std::make_unique<WeightedWindow4RowSamplerF16Bit<WEIGHTED_ROW4_BSPLINE, Components>>(src8,
                                                                                                     srcStride,
                                                                                                     inputWidth,
                                                                                                     inputHeight,
                                                                                                     output,
                                                                                                     dstStride,
                                                                                                     outputWidth,
                                                                                                     outputHeight);
    }
      break;
    case cubic: {
      sampler = std::make_unique<WeightedWindow4RowSamplerF16Bit<WEIGHTED_ROW4_CUBIC, Components>>(src8,
                                                                                                   srcStride,
                                                                                                   inputWidth,
                                                                                                   inputHeight,
                                                                                                   output,
                                                                                                   dstStride,
                                                                                                   outputWidth,
                                                                                                   outputHeight);
    }
      break;
    case bicubic: {
      sampler = std::make_unique<WeightedWindow4RowSamplerF16Bit<WEIGHTED_ROW4_BICUBIC, Components>>(src8,
                                                                                                     srcStride,
                                                                                                     inputWidth,
                                                                                                     inputHeight,
                                                                                                     output,
                                                                                                     dstStride,
                                                                                                     outputWidth,
                                                                                                     outputHeight);
    }
      break;
    case mitchell: {
      sampler = std::make_unique<WeightedWindow4RowSamplerF16Bit<WEIGHTED_ROW4_MITCHELL, Components>>(src8,
                                                                                                      srcStride,
                                                                                                      inputWidth,
                                                                                                      inputHeight,
                                                                                                      output,
                                                                                                      dstStride,
                                                                                                      outputWidth,
                                                                                                      outputHeight);
    }
      break;
    case lanczos: {
      sampler = std::make_unique<WeightedWindow6RowSamplerF16Bit<WEIGHTED_ROW6_LANCZOS_SINC, Components>>(src8,
                                                                                                          srcStride,
                                                                                                          inputWidth,
                                                                                                          inputHeight,
                                                                                                          output,
                                                                                                          dstStride,
                                                                                                          outputWidth,
                                                                                                          outputHeight);
    }
      break;
    case bilinear: {
      sampler = std::make_unique<BilinearRowSamplerF16Bit<Components>>(src8,
                                                                       srcStride,
                                                                       inputWidth,
                                                                       inputHeight,
                                                                       output,
                                                                       dstStride,
                                                                       outputWidth,
                                                                       outputHeight);
    }
      break;
    default: {
      sampler = std::make_unique<NearestRowSampler16Bit<Components>>(src8, srcStride,
                                                                     inputWidth,
                                                                     inputHeight,
                                                                     output,
                                                                     dstStride,
                                                                     outputWidth,
                                                                     outputHeight);
    }
      break;
  }

  const int threadCount = std::clamp(std::min(static_cast<int>(std::thread::hardware_concurrency()),
                                              static_cast<int>(outputHeight * outputWidth / (256 * 256))),
                                     static_cast<int>(1), static_cast<int>(12));

  concurrency::parallel_for(threadCount, outputHeight, [&](int iterationId) {
    sampler->sample(iterationId);
  });
}

#define SCALE_CHANNEL_16_TYPE(channelName, channelsCount) \
      void Scale##channelName##HWY(const uint16_t *input, const uint32_t srcStride,\
                                   uint32_t inputWidth, uint32_t inputHeight,\
                                   uint16_t *output, const uint32_t dstStride,\
                                   uint32_t outputWidth, uint32_t outputHeight,    \
                                   const int depth, const SparkYuvSampler option) {\
      ScaleRGB16OrChannelHWY<channelsCount>(input, srcStride, inputWidth, inputHeight, output, dstStride, outputWidth, outputHeight, depth, option);\
      }

SCALE_CHANNEL_16_TYPE(Channel16, 1)
SCALE_CHANNEL_16_TYPE(RGB16, 3)
SCALE_CHANNEL_16_TYPE(RGBA16, 4)

#undef SCALE_CHANNEL_16_TYPE

#define SCALE_CHANNEL_TYPE(channelName, channelsCount) \
      void Scale##channelName##HWY(const uint8_t *input, const uint32_t srcStride,\
                                   uint32_t inputWidth, uint32_t inputHeight,\
                                   uint8_t *output,\
                                   const uint32_t dstStride,\
                                   uint32_t outputWidth, uint32_t outputHeight,\
                                   const SparkYuvSampler option) {\
      ScaleRGB8OrChannelHWY<channelsCount>(input, srcStride, inputWidth, inputHeight, output, dstStride, outputWidth, outputHeight, option);\
      }

SCALE_CHANNEL_TYPE(Channel, 1)
SCALE_CHANNEL_TYPE(RGB, 3)
SCALE_CHANNEL_TYPE(RGBA, 4)

#undef SCALE_CHANNEL_TYPE

void ScaleRGBA1010102HWY(const uint8_t *input, const uint32_t srcStride,
                         uint32_t inputWidth, uint32_t inputHeight,
                         uint8_t *output,
                         const uint32_t dstStride,
                         uint32_t outputWidth, uint32_t outputHeight,
                         const SparkYuvSampler option) {
  ScaleRGB1010102OrChannelHWY(input, srcStride,
                              inputWidth, inputHeight,
                              output, dstStride, outputWidth, outputHeight, option);

}

#define SCALE_CHANNEL_F16_TYPE(channelName, channelsCount) \
      void Scale##channelName##HWY(const uint16_t *input, const uint32_t srcStride,\
                                   uint32_t inputWidth, uint32_t inputHeight,\
                                   uint16_t *output,\
                                   const uint32_t dstStride,\
                                   uint32_t outputWidth, uint32_t outputHeight,\
                                   const SparkYuvSampler option) {\
      ScaleRGB16FOrChannelHWY<channelsCount>(input, srcStride, inputWidth, inputHeight, output, dstStride, outputWidth, outputHeight, option);\
      }

SCALE_CHANNEL_F16_TYPE(ChannelF16, 1)
SCALE_CHANNEL_F16_TYPE(RGBF16, 3)
SCALE_CHANNEL_F16_TYPE(RGBAF16, 4)

#undef SCALE_CHANNEL_F16_TYPE
}
HWY_AFTER_NAMESPACE();

#if HWY_ONCE
namespace sparkyuv {
#define SCALE_CHANNEL_TYPE_DECLARE_HWY(channelName) HWY_EXPORT(Scale##channelName##HWY);

SCALE_CHANNEL_TYPE_DECLARE_HWY(Channel)
SCALE_CHANNEL_TYPE_DECLARE_HWY(RGB)
SCALE_CHANNEL_TYPE_DECLARE_HWY(RGBA)

SCALE_CHANNEL_TYPE_DECLARE_HWY(Channel16)
SCALE_CHANNEL_TYPE_DECLARE_HWY(RGB16)
SCALE_CHANNEL_TYPE_DECLARE_HWY(RGBA16)

SCALE_CHANNEL_TYPE_DECLARE_HWY(ChannelF16)
SCALE_CHANNEL_TYPE_DECLARE_HWY(RGBF16)
SCALE_CHANNEL_TYPE_DECLARE_HWY(RGBAF16)

#undef SCALE_CHANNEL_TYPE_DECLARE_HWY

#define SCALE_CHANNEL_DECLARE_E(channelName, channelsCount) \
      HWY_DLLEXPORT void Scale##channelName(const uint8_t *input, const uint32_t srcStride,\
                              uint32_t inputWidth, uint32_t inputHeight,\
                              uint8_t *output,\
                              const uint32_t dstStride,\
                              uint32_t outputWidth, uint32_t outputHeight,\
                              const SparkYuvSampler option) {\
        HWY_DYNAMIC_DISPATCH(Scale##channelName##HWY)(input, srcStride, inputWidth, inputHeight, \
                              output, dstStride, outputWidth, outputHeight, option);\
      }

SCALE_CHANNEL_DECLARE_E(Channel, 1)
SCALE_CHANNEL_DECLARE_E(RGB, 3)
SCALE_CHANNEL_DECLARE_E(RGBA, 4)

#undef SCALE_CHANNEL_DECLARE_E

#define SCALE_CHANNEL_F16_DECLARE_E(channelName, channelsCount) \
      HWY_DLLEXPORT void Scale##channelName(const uint16_t *input, const uint32_t srcStride,\
                              uint32_t inputWidth, uint32_t inputHeight,\
                              uint16_t *output, const uint32_t dstStride,\
                              uint32_t outputWidth, uint32_t outputHeight,\
                              const SparkYuvSampler option) {\
        HWY_DYNAMIC_DISPATCH(Scale##channelName##HWY)(input, srcStride, inputWidth, inputHeight, \
                              output, dstStride, outputWidth, outputHeight, option);\
      }

SCALE_CHANNEL_F16_DECLARE_E(ChannelF16, 1)
SCALE_CHANNEL_F16_DECLARE_E(RGBF16, 3)
SCALE_CHANNEL_F16_DECLARE_E(RGBAF16, 4)

#undef SCALE_CHANNEL_F16_DECLARE_E

HWY_EXPORT(ScaleRGBA1010102HWY);

HWY_DLLEXPORT void ScaleRGBA1010102(const uint8_t *input, const uint32_t srcStride,
                                    uint32_t inputWidth, uint32_t inputHeight,
                                    uint8_t *output, const uint32_t dstStride,
                                    uint32_t outputWidth, uint32_t outputHeight,
                                    const SparkYuvSampler option) {
  HWY_DYNAMIC_DISPATCH(ScaleRGBA1010102HWY)(input, srcStride,
                                            inputWidth, inputHeight,
                                            output, dstStride, outputWidth, outputHeight, option);

}

#define SCALE_CHANNEL_16_TYPE_E(channelName) \
      void Scale##channelName(const uint16_t *input, const uint32_t srcStride,\
                              uint32_t inputWidth, uint32_t inputHeight,\
                              uint16_t *output, const uint32_t dstStride,\
                              uint32_t outputWidth, uint32_t outputHeight,    \
                              const int depth, const SparkYuvSampler option) {\
        HWY_DYNAMIC_DISPATCH(Scale##channelName##HWY)(input, srcStride, inputWidth, inputHeight, output, dstStride, \
                outputWidth, outputHeight, depth, option);\
      }

SCALE_CHANNEL_16_TYPE_E(Channel16)
SCALE_CHANNEL_16_TYPE_E(RGB16)
SCALE_CHANNEL_16_TYPE_E(RGBA16)

#undef SCALE_CHANNEL_16_TYPE

}
#endif