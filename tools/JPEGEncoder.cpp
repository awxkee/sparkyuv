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

#include "JPEGEncoder.h"
#include <jpeglib.h>
#include <csetjmp>
#include <string>

#include "hwy/foreach_target.h"  // IWYU pragma: keep
#include "hwy/highway.h"
#include "hwy/base.h"

namespace aire {

struct aire_jpeg_error_mng {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

METHODDEF(void)
handleJpegError(j_common_ptr cinfo) {
  aire_jpeg_error_mng *myerr = (aire_jpeg_error_mng *) cinfo->err;
//        (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

using namespace hwy;
using namespace hwy::HWY_NAMESPACE;

template<class T>
void
RgbaToRGB(const T *__restrict__ src, const uint32_t srcStride,
          T *__restrict__ dst, const uint32_t newStride,
          const uint32_t width, const uint32_t height,
          const int *__restrict__ permuteMap) {

  const int idx1 = permuteMap[0];
  const int idx2 = permuteMap[1];
  const int idx3 = permuteMap[2];

  for (int y = 0; y < height; ++y) {

    auto srcPixels = reinterpret_cast<const T *>(reinterpret_cast<const uint8_t *>(src) + y * srcStride);
    auto dstPixels = reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(dst) + y * newStride);

    int x = 0;

    for (; x < width; ++x) {
      T vec[3] = {srcPixels[0], srcPixels[1], srcPixels[2]};
      dstPixels[0] = vec[idx1];
      dstPixels[1] = vec[idx2];
      dstPixels[2] = vec[idx3];

      srcPixels += 4;
      dstPixels += 3;
    }
  }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err52-cpp"
std::vector<uint8_t> JPEGEncoder::encodeRGB() {
  int newStride = sizeof(uint8_t) * width * 3;

  struct jpeg_compress_struct cinfo;
  struct aire_jpeg_error_mng jerr;

  cinfo.err = jpeg_std_error(reinterpret_cast<jpeg_error_mgr *>(&jerr));
  jerr.pub.error_exit = handleJpegError;

  jpeg_create_compress(&cinfo);

  unsigned char *outputBuffer = nullptr;
  unsigned long outputSize = 0;

  jpeg_mem_dest(&cinfo, &outputBuffer, &outputSize);

  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_compress(&cinfo);
    if (outputBuffer) {
      free(outputBuffer);
    }
    std::string msg("JPEG compression has failed");
    throw std::runtime_error(msg);
  }

  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_simple_progression(&cinfo);

  jpeg_start_compress(&cinfo, TRUE);

  JSAMPROW row_pointer;

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = reinterpret_cast<uint8_t *>(data) + cinfo.next_scanline * newStride;
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  std::vector<uint8_t> output(outputSize);
  std::copy(outputBuffer, outputBuffer + outputSize, output.begin());

  if (outputBuffer) {
    free(outputBuffer);
  }

  return output;
}
#pragma clang diagnostic pop

std::vector<uint8_t> JPEGEncoder::encode() {
  int newStride = sizeof(uint8_t) * width * 3;
  std::vector<uint8_t> rgbData(newStride * height);
  int permuteMap[3] = {0, 1, 2};
  RgbaToRGB(data, stride, rgbData.data(), newStride, width, height, permuteMap);

  struct jpeg_compress_struct cinfo;
  struct aire_jpeg_error_mng jerr;

  cinfo.err = jpeg_std_error(reinterpret_cast<jpeg_error_mgr *>(&jerr));
  jerr.pub.error_exit = handleJpegError;

  jpeg_create_compress(&cinfo);

  unsigned char *outputBuffer = nullptr;
  unsigned long outputSize = 0;

  jpeg_mem_dest(&cinfo, &outputBuffer, &outputSize);

  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_compress(&cinfo);
    if (outputBuffer) {
      free(outputBuffer);
    }
    std::string msg("JPEG compression has failed");
    throw std::runtime_error(msg);
  }

  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_simple_progression(&cinfo);

  jpeg_start_compress(&cinfo, TRUE);

  JSAMPROW row_pointer;

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = reinterpret_cast<uint8_t *>(rgbData.data()) + cinfo.next_scanline * newStride;
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  std::vector<uint8_t> output(outputSize);
  std::copy(outputBuffer, outputBuffer + outputSize, output.begin());

  if (outputBuffer) {
    free(outputBuffer);
  }

  return output;
}
}