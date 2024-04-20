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

namespace sparkyuv {
void WideRGBA8To10(uint8_t *src,
                   uint32_t srcStride,
                   uint16_t *dst,
                   uint32_t dstStride,
                   uint32_t width,
                   uint32_t height);
void WideRGBATo10(uint8_t *src, uint32_t srcStride, uint16_t *dst, uint32_t dstStride, uint32_t width, uint32_t height);

#if SPARKYUV_FULL_CHANNELS
void WideABGR8To10(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideARGBTo10(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideBGRA8To10(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideBGRTo10(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
#endif

void WideRGBA8To12(uint8_t *src,
                   uint32_t srcStride,
                   uint16_t *dst,
                   uint32_t dstStride,
                   uint32_t width,
                   uint32_t height);
void WideRGBATo12(uint8_t *src, uint32_t srcStride, uint16_t *dst, uint32_t dstStride, uint32_t width, uint32_t height);

#if SPARKYUV_FULL_CHANNELS
void WideABGR8To12(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideARGBTo12(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideBGRA8To12(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideBGRTo12(uint8_t* src, uint32_t srcStride,uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
#endif

void WideRGBA8To16(uint8_t *src,
                   uint32_t srcStride,
                   uint16_t *dst,
                   uint32_t dstStride,
                   uint32_t width,
                   uint32_t height);
void WideRGBATo16(uint8_t *src, uint32_t srcStride, uint16_t *dst, uint32_t dstStride, uint32_t width, uint32_t height);

#if SPARKYUV_FULL_CHANNELS
void WideABGR8To16(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideARGBTo16(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideBGRA8To16(uint8_t* src, uint32_t srcStride, uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
void WideBGRTo16(uint8_t* src, uint32_t srcStride,uint16_t* dst, uint32_t dstStride, uint32_t width, uint32_t height);
#endif

/**
 * Saturate 10 bit image to 8 bit, faster that dynamic version where bit depth comes into a parameter
 */
void SaturateRGBA10To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void SaturateRGB10To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
#if SPARKYUV_FULL_CHANNELS
void SaturateBGRA10To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
void SaturateARGB10To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
void SaturateABGR10To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
void SaturateBGR10To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
#endif

/**
 * Saturate 12 bit image to 8 bit, faster that dynamic version where bit depth comes into a parameter
 */
void SaturateRGBA12To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void SaturateRGB12To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
#if SPARKYUV_FULL_CHANNELS
void SaturateBGRA12To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
void SaturateARGB12To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
void SaturateABGR12To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
void SaturateBGR12To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
#endif

/**
 * Saturate 16 bit image to 8 bit, faster that dynamic version where bit depth comes into a parameter
 */
void SaturateRGBA16To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);

void SaturateRGB16To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
#if SPARKYUV_FULL_CHANNELS
void SaturateBGRA16To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
void SaturateARGB16To8(const uint16_t *src, uint32_t srcStride,
                      uint8_t *dst, uint32_t dstStride,
                      uint32_t width, uint32_t height);
void SaturateABGR16To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
void SaturateBGR16To8(const uint16_t *src, uint32_t srcStride,
                       uint8_t *dst, uint32_t dstStride,
                       uint32_t width, uint32_t height);
#endif

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

#if SPARKYUV_FULL_CHANNELS
void WideBGRA8(uint8_t *src, uint32_t srcStride,
              uint16_t *dst, uint32_t dstStride,
              uint32_t width, uint32_t height, int bitDepth);
void WideABGR8(uint8_t *src, uint32_t srcStride,
               uint16_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height, int bitDepth);
void WideARGB8(uint8_t *src, uint32_t srcStride,
               uint16_t *dst, uint32_t dstStride,
               uint32_t width, uint32_t height, int bitDepth);
void WideBGR8(uint8_t *src, uint32_t srcStride,
              uint16_t *dst, uint32_t dstStride,
              uint32_t width, uint32_t height, int bitDepth);
#endif

/**
 * Saturate uint16_t storage image with bit depth from param to 8
 */

void SaturateRGBATo8(const uint16_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, int bitDepth);

void SaturateRGBTo8(const uint16_t *src, uint32_t srcStride,
                    uint8_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);

#if SPARKYUV_FULL_CHANNELS
void SaturateBGRTo8(const uint16_t *src, uint32_t srcStride,
                    uint8_t *dst, uint32_t dstStride,
                    uint32_t width, uint32_t height, int bitDepth);

void SaturateBGRATo8(const uint16_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, int bitDepth);

void SaturateABGRTo8(const uint16_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, int bitDepth);

void SaturateARGBTo8(const uint16_t *src, uint32_t srcStride,
                     uint8_t *dst, uint32_t dstStride,
                     uint32_t width, uint32_t height, int bitDepth);
#endif

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
 * Gaussian Blur.
 * Not approximation just a gaussian blur, use when antialias or clear gaussian methods is needed.
 * In-place use allowed
 * For this method A channel position doesn't matter. You may pass any 4-channel image and expect valid results
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
}

#endif //YUV_INCLUDE_SPARKYUV_BASIC_H_
