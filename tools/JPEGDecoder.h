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

#include <jpeglib.h>
#include <setjmp.h>
#include <string>

namespace sparkyuv {
static bool decompressJPEG(std::string &filename, std::vector<uint8_t> &imageBuffer, int &width, int &height) {
  FILE *file = fopen(filename.c_str(), "rb");
  if (!file) {
    fprintf(stderr, "Error opening JPEG file: %s\n", filename.c_str());
    return false;
  }

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);
  cinfo.out_color_space = JCS_RGB;
  jpeg_start_decompress(&cinfo);

  width = cinfo.output_width;
  height = cinfo.output_height;
  int numChannels = cinfo.output_components;

  // Allocate memory for the image buffer
  imageBuffer.resize(width * height * numChannels);

  // Decompress the image scanlines
  JSAMPROW rowPointer[1];
  while (cinfo.output_scanline < cinfo.output_height) {
    rowPointer[0] = &imageBuffer[(cinfo.output_scanline) * width * numChannels];
    jpeg_read_scanlines(&cinfo, rowPointer, 1);
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(file);

  return true;
}
}