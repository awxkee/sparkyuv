//
// Created by Radzivon Bartoshyk on 25/03/2024.
//

#pragma once

#include <jpeglib.h>
#include <setjmp.h>
#include <string>

namespace sparkyuv {
bool decompressJPEG(const char *filename, std::vector<uint8_t> &imageBuffer, int &width, int &height) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Error opening JPEG file: %s\n", filename);
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