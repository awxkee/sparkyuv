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

#ifndef YUV_INCLUDE_SPARKYUV_BASIC_H_
#define YUV_INCLUDE_SPARKYUV_BASIC_H_

#include <cstdint>
#include "sparkyuv-def.h"

namespace sparkyuv {
void WideRGBA8To10(uint8_t *src,
                   uint32_t srcStride,
                   uint16_t *dst,
                   uint32_t dstStride,
                   uint32_t width,
                   uint32_t height);
void WideRGBATo10(uint8_t *src, uint32_t srcStride, uint16_t *dst, uint32_t dstStride, uint32_t width, uint32_t height);

void WideRGBA8To12(uint8_t *src,
                   uint32_t srcStride,
                   uint16_t *dst,
                   uint32_t dstStride,
                   uint32_t width,
                   uint32_t height);
void WideRGBATo12(uint8_t *src, uint32_t srcStride, uint16_t *dst, uint32_t dstStride, uint32_t width, uint32_t height);

void WideRGBA8To16(uint8_t *src,
                   uint32_t srcStride,
                   uint16_t *dst,
                   uint32_t dstStride,
                   uint32_t width,
                   uint32_t height);
void WideRGBATo16(uint8_t *src, uint32_t srcStride, uint16_t *dst, uint32_t dstStride, uint32_t width, uint32_t height);


/**
 * Saturate 10 bit image to 8 bit, faster that dynamic version where bit depth comes into a parameter
 */
void SaturateRGBA10To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void SaturateRGB10To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);

/**
 * Saturate 12 bit image to 8 bit, faster that dynamic version where bit depth comes into a parameter
 */
void SaturateRGBA12To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void SaturateRGB12To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);

/**
 * Saturate 16 bit image to 8 bit, faster that dynamic version where bit depth comes into a parameter
 */
void SaturateRGBA16To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void SaturateRGB16To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);

/**
 * Wide 8 bit image to provided bit depth
 * @param bitDepth - bit depth of the target image
 */
void WideRGBA8(uint8_t *src, uint32_t srcStride,
               uint16_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height, int bitDepth);

/**
 * Wide 8 bit image to provided bit depth
 * @param bitDepth - bit depth of the target image
 */
void WideRGB8(uint8_t *src, uint32_t srcStride,
              uint16_t *dst, uint32_t dstStride,
              uint32_t width, uint32_t height, int bitDepth);

/**
 * Saturate uint16_t storage image with bit depth from param to 8
 */

void SaturateRGBATo8(const uint16_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, int bitDepth);

void SaturateRGBTo8(const uint16_t *src, uint32_t srcStride,
                    uint8_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);

/**
 * Alpha multiplication
 */

/**
* @brief Pre multiplies alpha for rendering
*/
void RGBAPremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                          uint8_t *dst, uint32_t dstStride,
                          uint32_t width, uint32_t height);

/**
* @brief Pre multiplies alpha for rendering
*/
void ABGRPremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                          uint8_t *dst, uint32_t dstStride,
                          uint32_t width, uint32_t height);

/**
* @brief Pre multiplies alpha for rendering
*/
void ARGBPremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                          uint8_t *dst, uint32_t dstStride,
                          uint32_t width, uint32_t height);

/**
* @brief Pre multiplies alpha for rendering
*/
void BGRAPremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                          uint8_t *dst, uint32_t dstStride,
                          uint32_t width, uint32_t height);

/**
 * Alpha un pre multiplication
 */

/**
 * @brief Un premultiply alpha
 */
void RGBAUnpremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                            uint8_t *dst, uint32_t dstStride,
                            uint32_t width, uint32_t height);

/**
 * @brief Un premultiply alpha
 */
void ABGRUnpremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                            uint8_t *dst, uint32_t dstStride,
                            uint32_t width, uint32_t height);

/**
 * @brief Un premultiply alpha
 */
void ARGBUnpremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                            uint8_t *dst, uint32_t dstStride,
                            uint32_t width, uint32_t height);

/**
 * @brief Un premultiply alpha
 */
void BGRAUnpremultiplyAlpha(const uint8_t *src, uint32_t srcStride,
                            uint8_t *dst, uint32_t dstStride,
                            uint32_t width, uint32_t height);

/**
 * Flip Vertical
 */

void FlipVerticalRGBA(const uint8_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);

void FlipVerticalRGB(const uint8_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void FlipVerticalChannel(const uint8_t *src, uint32_t srcStride,
                         uint8_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height);

void FlipVerticalRGBA1010102(const uint8_t *src, uint32_t srcStride,
                             uint8_t *dst, uint32_t dstStride,
                             uint32_t width, uint32_t height);

void FlipVerticalRGBA16(const uint16_t *src, uint32_t srcStride,
                        uint16_t *dst, uint32_t dstStride,
                        uint32_t width, uint32_t height);

void FlipVerticalRGB16(const uint16_t *src, uint32_t srcStride,
                       uint16_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void FlipVerticalChannel16(const uint16_t *src, uint32_t srcStride,
                           uint16_t *dst, uint32_t dstStride,
                           uint32_t width, uint32_t height);

/**
 * Flip Horizontal
 */

void FlipHorizontalRGBA(const uint8_t *src, uint32_t srcStride,
                        uint8_t *dst, uint32_t dstStride,
                        uint32_t width, uint32_t height);
void FlipHorizontalRGB(const uint8_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void FlipHorizontalChannel(const uint8_t *src, uint32_t srcStride,
                           uint8_t *dst, uint32_t dstStride,
                           uint32_t width, uint32_t height);

void FlipHorizontalRGBA1010102(const uint8_t *src, uint32_t srcStride,
                               uint8_t *dst, uint32_t dstStride,
                               uint32_t width, uint32_t height);

void FlipHorizontalRGBA16(const uint16_t *src, uint32_t srcStride,
                          uint16_t *dst, uint32_t dstStride,
                          uint32_t width, uint32_t height);

void FlipHorizontalRGB16(const uint16_t *src, uint32_t srcStride,
                         uint16_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height);

void FlipHorizontalChannel16(const uint16_t *src, uint32_t srcStride,
                             uint16_t *dst, uint32_t dstStride,
                             uint32_t width, uint32_t height);

/**
 * Transpose clockwise
 */

void TransposeClockwiseRGBA(const uint8_t *src, uint32_t srcStride,
                            uint8_t *dst, uint32_t dstStride,
                            uint32_t width, uint32_t height);

void TransposeClockwiseRGB(const uint8_t *src, uint32_t srcStride,
                           uint8_t *dst, uint32_t dstStride,
                           uint32_t width, uint32_t height);

void TransposeClockwiseChannel(const uint8_t *src, uint32_t srcStride,
                               uint8_t *dst, uint32_t dstStride,
                               uint32_t width, uint32_t height);

void TransposeClockwiseRGBA1010102(const uint8_t *src, uint32_t srcStride,
                                   uint8_t *dst, uint32_t dstStride,
                                   uint32_t width, uint32_t height);

void TransposeClockwiseRGBA16(const uint16_t *src, uint32_t srcStride,
                              uint16_t *dst, uint32_t dstStride,
                              uint32_t width, uint32_t height);

void TransposeClockwiseRGB16(const uint16_t *src, uint32_t srcStride,
                             uint16_t *dst, uint32_t dstStride,
                             uint32_t width, uint32_t height);

void TransposeClockwiseChannel16(const uint16_t *src, uint32_t srcStride,
                                 uint16_t *dst, uint32_t dstStride,
                                 uint32_t width, uint32_t height);

/**
 * Transpose counter clockwise
 */

void TransposeCounterClockwiseRGBA(const uint8_t *src, uint32_t srcStride,
                                   uint8_t *dst, uint32_t dstStride,
                                   uint32_t width, uint32_t height);

void TransposeCounterClockwiseRGB(const uint8_t *src, uint32_t srcStride,
                                  uint8_t *dst, uint32_t dstStride,
                                  uint32_t width, uint32_t height);

void TransposeCounterClockwiseChannel(const uint8_t *src, uint32_t srcStride,
                                      uint8_t *dst, uint32_t dstStride,
                                      uint32_t width, uint32_t height);

void TransposeCounterClockwiseRGBA1010102(const uint8_t *src, uint32_t srcStride,
                                          uint8_t *dst, uint32_t dstStride,
                                          uint32_t width, uint32_t height);

void TransposeCounterClockwiseRGBA16(const uint16_t *src, uint32_t srcStride,
                                     uint16_t *dst, uint32_t dstStride,
                                     uint32_t width, uint32_t height);

void TransposeCounterClockwiseRGB16(const uint16_t *src, uint32_t srcStride,
                                    uint16_t *dst, uint32_t dstStride,
                                    uint32_t width, uint32_t height);

void TransposeCounterClockwiseChannel16(const uint16_t *src, uint32_t srcStride,
                                        uint16_t *dst, uint32_t dstStride,
                                        uint32_t width, uint32_t height);

/**
 * Copy
 */

void CopyRGBA(const uint8_t *src, uint32_t srcStride,
              uint8_t *dst, uint32_t dstStride,
              uint32_t width, uint32_t height);

void CopyRGB(const uint8_t *src, uint32_t srcStride,
             uint8_t *dst, uint32_t dstStride,
             uint32_t width, uint32_t height);

void CopyChannel(const uint8_t *src, uint32_t srcStride,
                 uint8_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height);

void CopyRGBA1010102(const uint8_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void CopyRGBA16(const uint16_t *src, uint32_t srcStride,
                uint16_t *dst, uint32_t dstStride,
                uint32_t width, uint32_t height);

void CopyRGB16(const uint16_t *src, uint32_t srcStride,
               uint16_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);

void CopyChannel16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height);

/**
 * Rotate
 */

void RotateRGBA(const uint8_t *src, uint32_t srcStride,
                uint8_t *dst, uint32_t dstStride,
                uint32_t width, uint32_t height, SparkYuvRotation rotation);

void RotateRGB(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height, SparkYuvRotation rotation);

void RotateRGBA1010102(const uint8_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height, SparkYuvRotation rotation);

void RotateChannel(const uint8_t *src, uint32_t srcStride,
                   uint8_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, SparkYuvRotation rotation);

void RotateRGBA16(const uint16_t *src, uint32_t srcStride,
                  uint16_t *dst, uint32_t dstStride,
                  uint32_t width, uint32_t height, SparkYuvRotation rotation);

void RotateRGB16(const uint16_t *src, uint32_t srcStride,
                 uint16_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height, SparkYuvRotation rotation);

void RotateChannel16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, SparkYuvRotation rotation);

/**
 * Fast gaussian approximation. Close to gaussian however some non gaussian may be detected
 * if FFT or any advanced analysis will be involved
 */

/**
 * @param radius do not really expected to be larger 512 for uint8_t and ~200 for uint16_t
 */
void FastGaussianBlurRGBA(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurRGB(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);

#if SPARKYUV_FULL_CHANNELS
void FastGaussianBlurARGB(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurABGR(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurBGRA(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurBGR(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
#endif

void FastGaussianBlurRGBA16(uint16_t *data, uint32_t stride,
                            uint32_t width, uint32_t height,
                            int radius);
void FastGaussianBlurRGB16(uint16_t *data, uint32_t stride,
                           uint32_t width, uint32_t height,
                           int radius);

#if SPARKYUV_FULL_CHANNELS
void FastGaussianBlurARGB16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurABGR16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurBGRA16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurBGR16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
#endif

void FastGaussianBlurRGBAF16(uint16_t *data, uint32_t stride,
                             uint32_t width, uint32_t height,
                             int radius);
void FastGaussianBlurRGBF16(uint16_t *data, uint32_t stride,
                            uint32_t width, uint32_t height,
                            int radius);

#if SPARKYUV_FULL_CHANNELS
void FastGaussianBlurARGBF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurABGRF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurBGRAF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurBGRF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
#endif

void FastGaussianBlurChannel(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurChannel16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);

/**
 * Fast Gaussian Next Blur.
 * Approximation.
 * In-place use allowed
 * Slower that Fast Gaussian however better results, still much more faster than naive blur
 * Close to gaussian however some non gaussian may be detected if FFT or any advanced analysis will be involved
 */

void FastGaussianNextBlurRGBA(uint8_t *src, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBlurRGB(uint8_t *src, uint32_t stride, uint32_t width, uint32_t height, int radius);
#if SPARKYUV_FULL_CHANNELS
void FastGaussianNextARGB(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextABGR(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBGRA(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBGR(uint8_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
#endif

void FastGaussianBlurNextRGBA16(uint16_t *src, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianBlurNextRGB16(uint16_t *src, uint32_t stride, uint32_t width, uint32_t height, int radius);
#if SPARKYUV_FULL_CHANNELS
void FastGaussianNextBlurRARGB16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBlurRABGR16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBlurRBGRA16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBlurRBGR16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
#endif

void FastGaussianNextBlurRGBAF16(uint16_t *data, uint32_t stride,
                                 uint32_t width, uint32_t height,
                                 int radius);
void FastGaussianNextBlurRGBF16(uint16_t *data, uint32_t stride,
                                uint32_t width, uint32_t height,
                                int radius);

#if SPARKYUV_FULL_CHANNELS
void FastGaussianNextBlurARGBF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBlurABGRF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBlurBGRAF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
void FastGaussianNextBlurBGRF16(uint16_t *data, uint32_t stride, uint32_t width, uint32_t height, int radius);
#endif

/**
 * Gaussian Blur.
 * Not approximation just a gaussian blur, use when antialias or clear gaussian methods is needed.
 * In-place use allowed
 */

void GaussianBlurRGBA(const uint8_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height,
                      int kernelSize, float sigma);
void GaussianBlurRGB(const uint8_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height,
                     int kernelSize, float sigma);
void GaussianBlurChannel(const uint8_t *src, uint32_t srcStride,
                         uint8_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height,
                         int kernelSize, float sigma);

void GaussianBlurRGBA16(const uint16_t *src, uint32_t srcStride,
                        uint16_t *dst, uint32_t dstStride,
                        uint32_t width, uint32_t height,
                        int kernelSize, float sigma);
void GaussianBlurRGB16(const uint16_t *src, uint32_t srcStride,
                       uint16_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height,
                       int kernelSize, float sigma);
void GaussianBlurChannel16(const uint16_t *src, uint32_t srcStride,
                           uint16_t *dst, uint32_t dstStride,
                           uint32_t width, uint32_t height,
                           int kernelSize, float sigma);

void GaussianBlurRGBAF16(const uint16_t *src, uint32_t srcStride,
                         uint16_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height,
                         int kernelSize, float sigma);
void GaussianBlurRGBF16(const uint16_t *src, uint32_t srcStride,
                        uint16_t *dst, uint32_t dstStride,
                        uint32_t width, uint32_t height,
                        int kernelSize, float sigma);
void GaussianBlurChannelF16(const uint16_t *src, uint32_t srcStride,
                            uint16_t *dst, uint32_t dstStride,
                            uint32_t width, uint32_t height,
                            int kernelSize, float sigma);

void GaussianBlurRGBAF32(const float *src, uint32_t srcStride,
                         float *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height,
                         int kernelSize, float sigma);
void GaussianBlurRGBF32(const float *src, uint32_t srcStride,
                        float *dst, uint32_t dstStride,
                        uint32_t width, uint32_t height,
                        int kernelSize, float sigma);
void GaussianBlurChannelF32(const float *src, uint32_t srcStride,
                            float *dst, uint32_t dstStride,
                            uint32_t width, uint32_t height,
                            int kernelSize, float sigma);

/**
 * Scaling functions
 */

// Mark scale U8

void ScaleRGB(const uint8_t *input, uint32_t srcStride,
              uint32_t inputWidth, uint32_t inputHeight,
              uint8_t *output, uint32_t dstStride,
              uint32_t outputWidth, uint32_t outputHeight,
              SparkYuvSampler option);
void ScaleRGBA(const uint8_t *input, uint32_t srcStride,
               uint32_t inputWidth, uint32_t inputHeight,
               uint8_t *output, uint32_t dstStride,
               uint32_t outputWidth, uint32_t outputHeight,
               SparkYuvSampler option);
void ScaleChannel(const uint8_t *input, uint32_t srcStride,
                  uint32_t inputWidth, uint32_t inputHeight,
                  uint8_t *output, uint32_t dstStride,
                  uint32_t outputWidth, uint32_t outputHeight,
                  SparkYuvSampler option);
// Mark scale F16


void ScaleRGBF16(const uint16_t *input, uint32_t srcStride,
                 uint32_t inputWidth, uint32_t inputHeight,
                 uint16_t *output, uint32_t dstStride,
                 uint32_t outputWidth, uint32_t outputHeight,
                 SparkYuvSampler option);
void ScaleRGBAF16(const uint16_t *input, uint32_t srcStride,
                  uint32_t inputWidth, uint32_t inputHeight,
                  uint16_t *output, uint32_t dstStride,
                  uint32_t outputWidth, uint32_t outputHeight,
                  SparkYuvSampler option);

void ScaleChannelF16(const uint16_t *input, uint32_t srcStride,
                     uint32_t inputWidth, uint32_t inputHeight,
                     uint16_t *output, uint32_t dstStride,
                     uint32_t outputWidth, uint32_t outputHeight,
                     SparkYuvSampler option);

// Mark: Scale RGBA1010102

void ScaleRGBA1010102(const uint8_t *input, uint32_t srcStride,
                      uint32_t inputWidth, uint32_t inputHeight,
                      uint8_t *output, uint32_t dstStride,
                      uint32_t outputWidth, uint32_t outputHeight,
                      SparkYuvSampler option);

void ScaleRGBA16(const uint16_t *input, uint32_t srcStride,
                 uint32_t inputWidth, uint32_t inputHeight,
                 uint16_t *output, uint32_t dstStride,
                 uint32_t outputWidth, uint32_t outputHeight,
                 int depth, SparkYuvSampler option);
void ScaleRGB16(const uint16_t *input, uint32_t srcStride,
                uint32_t inputWidth, uint32_t inputHeight,
                uint16_t *output, uint32_t dstStride,
                uint32_t outputWidth, uint32_t outputHeight,
                int depth, SparkYuvSampler option);
void ScaleChannel16(const uint16_t *input, uint32_t srcStride,
                    uint32_t inputWidth, uint32_t inputHeight,
                    uint16_t *output, uint32_t dstStride,
                    uint32_t outputWidth, uint32_t outputHeight,
                    int depth, SparkYuvSampler option);

/**
 * Convert from U8 to F16
 */

/**
* @brief Converts an image to F16 type, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBAToRGBAF16(const uint8_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height);

/**
* @brief Converts an image to F16 type, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBToRGBF16(const uint8_t *src, uint32_t srcStride,
                 uint16_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height);

/**
* @brief Converts an image to F16 type, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBA1010102ToRGBAF16(const uint8_t *src, uint32_t srcStride,
                          uint16_t *dst, uint32_t dstStride,
                          uint32_t width, uint32_t height);

/**
* @brief Converts an image to F16 type, channel order is preserved
* @param depth Bit depth of the image
*/
void ChannelToChannelF16(const uint8_t *src, uint32_t srcStride,
                         uint16_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height);

/**
 * Convert from U16 to F16
 */

/**
* @brief Converts an image to F16 (float16) type, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBA16ToRGBAF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, int depth);

/**
* @brief Converts an image to F16 (float16) type, channel order is preserved
* @param depth Bit depth of the image
*/
void RGB16ToRGBF16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);

/**
* @brief Converts an image to F16 (float16) type, channel order is preserved
* @param depth Bit depth of the image
*/
void Channel16ToChannelF16(const uint16_t *src, uint32_t srcStride,
                           uint16_t *dst, uint32_t dstStride,
                           uint32_t width, uint32_t height, int depth);

/**
 * Convert from F16 to U8
 */

/**
* @brief Converts an image from F16 (float16) type to uint8_t 8 bit, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBAF16ToRGBA(const uint16_t *src, uint32_t srcStride,
                   uint8_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height);

/**
* @brief Converts an image from F16 (float16) type to uint8_t 8 bit, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBF16ToRGB(const uint16_t *src, uint32_t srcStride,
                 uint8_t *dst, uint32_t dstStride,
                 uint32_t width, uint32_t height);

/**
* @brief Converts an image from F16 (float16) type to uint8_t 8 bit, channel order is preserved
* @param depth Bit depth of the image
*/
void ChannelF16ToChannel(const uint16_t *src, uint32_t srcStride,
                         uint8_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height);

/**
* @brief Converts an image from F16 (float16) type to uint8_t 8 bit, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBAF16ToRGBA1010102(const uint16_t *src, uint32_t srcStride,
                          uint8_t *dst, uint32_t dstStride,
                          uint32_t width, uint32_t height);

/**
 * Convert from F16 to U16
 */

/**
* @brief Converts an image from F16 (float16) type to uint16_t with provided bit depth, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBAF16ToRGBA16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, int depth);

/**
* @brief Converts an image from F16 (float16) type to uint16_t with provided bit depth, channel order is preserved
* @param depth Bit depth of the image
*/
void RGBF16ToRGB16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);

/**
* @brief Converts an image from F16 (float16) type to uint16_t with provided bit depth, channel order is preserved
* @param depth Bit depth of the image
*/
void ChannelF16ToChannel16(const uint16_t *src, uint32_t srcStride,
                           uint16_t *dst, uint32_t dstStride,
                           uint32_t width, uint32_t height, int depth);

/**
 * Reformat F16 to other F16
 */

void RGBF16ToRGBAF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void BGRF16ToBGRAF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void BGRF16ToABGRF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void RGBF16ToARGBF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void RGBAF16ToRGBF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void BGRAF16ToBGRF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void ABGRF16ToBGRF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void ARGBF16ToRGBF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void BGRAF16ToRGBF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void RGBAF16ToBGRF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void ABGRF16ToRGBF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);
void ARGBF16ToBGRF16(const uint16_t *src, uint32_t srcStride,
                     uint16_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height);

void BGRAF16ToRGBAF16(const uint16_t *src, uint32_t srcStride,
                      uint16_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);

void BGRAF16ToARGBF16(const uint16_t *src, uint32_t srcStride,
                      uint16_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
void RGBAF16ToABGRF16(const uint16_t *src, uint32_t srcStride,
                      uint16_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
void ABGRF16ToRGBAF16(const uint16_t *src, uint32_t srcStride,
                      uint16_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
void ARGBF16ToBGRAF16(const uint16_t *src, uint32_t srcStride,
                      uint16_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);

// MARK: Channels reformat 16 bit with dynamic bit depth

/**
 * Reformat bitmap to AR30(1010102) from RGBA of given depth
 * @param depth Bit depth of source image
 */
void RGBA16ToRGBA1010102(const uint16_t *src, uint32_t srcStride,
                         uint16_t *dst, uint32_t dstStride,
                         uint32_t width, uint32_t height, int depth);

/**
* @brief Reformat from source pixel format to another using uint16_t pixel storage type
*
* This function takes two integers as input and returns their sum.
*
* @param depth Bit depth of the image, will be used to set alpha channel at peak of the current bit depth if alpha
* channel will be added
*/
void RGB16ToRGBA16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void BGR16ToBGRA16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void BGR16ToABGR16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void RGB16ToARGB16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);

void RGBA16ToRGB16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void BGRA16ToBGR16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void ABGR16ToBGR16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void ARGB16ToRGB16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);

void BGRA16ToRGB16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void RGBA16ToBGR16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void ABGR16ToRGB16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);
void ARGB16ToBGR16(const uint16_t *src, uint32_t srcStride,
                   uint16_t *dst, uint32_t dstStride,
                   uint32_t width, uint32_t height, int depth);

void BGRA16ToRGBA16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int depth);
void BGRA16ToARGB16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int depth);
void RGBA16ToABGR16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int depth);
void ABGR16ToRGBA16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int depth);
void ARGB16ToBGRA16(const uint16_t *src, uint32_t srcStride,
                    uint16_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int depth);

/**
* @brief Reformat from source pixel format to another using uint8_t pixel storage type
* If alpha channel will be added it will be set to peak value of 8 bit.
*
*/
void RGBAToRGBA1010102(const uint8_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
void RGBA1010102ToRGBA(const uint8_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void RGBToRGBA(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void BGRToBGRA(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void BGRToABGR(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void RGBToARGB(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);

void RGBAToRGB(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void BGRAToBGR(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void ABGRToBGR(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void ARGBToRGB(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);

void BGRAToRGB(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void RGBAToBGR(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void ABGRToRGB(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);
void ARGBToBGR(const uint8_t *src, uint32_t srcStride,
               uint8_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height);

void BGRAToRGBA(const uint8_t *src, uint32_t srcStride,
                uint8_t *dst, uint32_t dstStride,
                uint32_t width, uint32_t height);
void BGRAToARGB(const uint8_t *src, uint32_t srcStride,
                uint8_t *dst, uint32_t dstStride,
                uint32_t width, uint32_t height);
void RGBAToABGR(const uint8_t *src, uint32_t srcStride,
                uint8_t *dst, uint32_t dstStride,
                uint32_t width, uint32_t height);
void ABGRToRGBA(const uint8_t *src, uint32_t srcStride,
                uint8_t *dst, uint32_t dstStride,
                uint32_t width, uint32_t height);
void ARGBToBGRA(const uint8_t *src, uint32_t srcStride,
                uint8_t *dst, uint32_t dstStride,
                uint32_t width, uint32_t height);

}

#endif //YUV_INCLUDE_SPARKYUV_BASIC_H_
