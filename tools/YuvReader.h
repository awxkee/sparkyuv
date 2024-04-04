//
// Created by Radzivon Bartoshyk on 25/03/2024.
//

#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

bool readYUV420File(const std::string &filename, const int width, const int height,
                    std::vector<uint8_t> &yData,
                    std::vector<uint8_t> &uData,
                    std::vector<uint8_t> &vData) {
  namespace fs = std::filesystem;

  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return false;
  }

  std::uintmax_t filesize = fs::file_size(filename);

  // Calculate the size of Y, U, and V planes
  size_t yuvTotal = width * height * 3 / 2;
  size_t ySize = width * height;
  size_t uSize = ((width + 1) / 2) * ((height + 1) / 2);  // YUV420 has 1/4 U and V resolution
  size_t vSize = uSize;

  size_t total = ySize + uSize + vSize;
  if (total != filesize) {
    std::cerr << "Invalid fs " << filename
              << ", local size " + std::to_string(total) + " and perceived " + std::to_string(filesize) << std::endl;
    return false;
  }

  // Resize vectors to hold the Y, U, and V data
  yData.resize(ySize);
  uData.resize(uSize);
  vData.resize(vSize);

  // Read Y plane
  file.read(reinterpret_cast<char *>(yData.data()), ySize);
  if (!file.good()) {
    std::cerr << "Error reading Y plane" << std::endl;
    return false;
  }

  // Read U plane
  file.read(reinterpret_cast<char *>(uData.data()), uSize);
  if (!file.good()) {
    std::cerr << "Error reading U plane" << std::endl;
    return false;
  }

  // Read V plane
  file.read(reinterpret_cast<char *>(vData.data()), vSize);
  if (!file.good()) {
    std::cerr << "Error reading V plane" << std::endl;
    return false;
  }

  file.close();
  return true;
}

bool readYUV422File(const std::string &filename, const int width, const int height,
                    std::vector<uint8_t> &yData,
                    std::vector<uint8_t> &uData,
                    std::vector<uint8_t> &vData) {
  namespace fs = std::filesystem;

  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return false;
  }

  std::uintmax_t filesize = fs::file_size(filename);

  // Calculate the size of Y, U, and V planes
  size_t ySize = width * height;
  size_t uSize = ((width + 1) / 2) * height;  // YUV420 has 1/4 U and V resolution
  size_t vSize = uSize;

  size_t total = ySize + uSize + vSize;
  if (total != filesize) {
    std::cerr << "Invalid fs " << filename
              << ", local size " + std::to_string(total) + " and perceived " + std::to_string(filesize) << std::endl;
    return false;
  }

  // Resize vectors to hold the Y, U, and V data
  yData.resize(ySize);
  uData.resize(uSize);
  vData.resize(vSize);

  // Read Y plane
  file.read(reinterpret_cast<char *>(yData.data()), ySize);
  if (!file.good()) {
    std::cerr << "Error reading Y plane" << std::endl;
    return false;
  }

  // Read U plane
  file.read(reinterpret_cast<char *>(uData.data()), uSize);
  if (!file.good()) {
    std::cerr << "Error reading U plane" << std::endl;
    return false;
  }

  // Read V plane
  file.read(reinterpret_cast<char *>(vData.data()), vSize);
  if (!file.good()) {
    std::cerr << "Error reading V plane" << std::endl;
    return false;
  }

  file.close();
  return true;
}

bool readYUV444File(const std::string &filename, const int width, const int height,
                    std::vector<uint8_t> &yData,
                    std::vector<uint8_t> &uData,
                    std::vector<uint8_t> &vData) {
  namespace fs = std::filesystem;

  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return false;
  }

  std::uintmax_t filesize = fs::file_size(filename);

  // Calculate the size of Y, U, and V planes
  size_t ySize = width * height;
  size_t uSize = width * height;  // YUV420 has 1/4 U and V resolution
  size_t vSize = uSize;

  size_t total = ySize + uSize + vSize;
  if (total != filesize) {
    std::cerr << "Invalid fs " << filename
              << ", local size " + std::to_string(total) + " and perceived " + std::to_string(filesize) << std::endl;
    return false;
  }

  // Resize vectors to hold the Y, U, and V data
  yData.resize(ySize);
  uData.resize(uSize);
  vData.resize(vSize);

  // Read Y plane
  file.read(reinterpret_cast<char *>(yData.data()), ySize);
  if (!file.good()) {
    std::cerr << "Error reading Y plane" << std::endl;
    return false;
  }

  // Read U plane
  file.read(reinterpret_cast<char *>(uData.data()), uSize);
  if (!file.good()) {
    std::cerr << "Error reading U plane" << std::endl;
    return false;
  }

  // Read V plane
  file.read(reinterpret_cast<char *>(vData.data()), vSize);
  if (!file.good()) {
    std::cerr << "Error reading V plane" << std::endl;
    return false;
  }

  file.close();
  return true;
}

bool readYUV444P16File(const std::string &filename, const int width, const int height,
                       std::vector<uint8_t> &yData,
                       std::vector<uint8_t> &uData,
                       std::vector<uint8_t> &vData) {
  namespace fs = std::filesystem;

  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return false;
  }

  std::uintmax_t filesize = fs::file_size(filename);

  // Calculate the size of Y, U, and V planes
  size_t ySize = width * height * sizeof(uint16_t);
  size_t uSize = width * height * sizeof(uint16_t);;
  size_t vSize = uSize;;

  size_t total = ySize + uSize + vSize;
  if (total != filesize) {
    std::cerr << "Invalid fs " << filename
              << ", local size " + std::to_string(total) + " and perceived " + std::to_string(filesize) << std::endl;
    return false;
  }

  // Resize vectors to hold the Y, U, and V data
  yData.resize(ySize);
  uData.resize(uSize);
  vData.resize(vSize);

  // Read Y plane
  file.read(reinterpret_cast<char *>(yData.data()), ySize);
  if (!file.good()) {
    std::cerr << "Error reading Y plane" << std::endl;
    return false;
  }

  // Read U plane
  file.read(reinterpret_cast<char *>(uData.data()), uSize);
  if (!file.good()) {
    std::cerr << "Error reading U plane" << std::endl;
    return false;
  }

  // Read V plane
  file.read(reinterpret_cast<char *>(vData.data()), vSize);
  if (!file.good()) {
    std::cerr << "Error reading V plane" << std::endl;
    return false;
  }

  file.close();
  return true;
}

bool readNV21YUvFile(const std::string &filename,
                     std::vector<uint8_t> &yPlane,
                     std::vector<uint8_t> &uvPlanes,
                     int width,
                     int height) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return false;
  }
  namespace fs = std::filesystem;
  std::uintmax_t filesize = fs::file_size(filename);

  // Calculate the size of the YUV data (Y component + UV component)
  int ySize = width * height;
  int uvSize = ((width + 1) / 2) * ((height + 1) / 2) * 2;

  size_t total = ySize + uvSize;
  if (total != filesize) {
    std::cerr << "Invalid fs " << filename
              << ", local size " + std::to_string(total) + " and perceived " + std::to_string(filesize) << std::endl;
    return false;
  }

  yPlane.resize(ySize);
  uvPlanes.resize(uvSize);

  file.read(reinterpret_cast<char *>(yPlane.data()), ySize);
  if (!file.good()) {
    std::cerr << "Failed to read YUV data from file." << std::endl;
    return false;
  }

  file.read(reinterpret_cast<char *>(uvPlanes.data()), uvSize);
  if (!file.good()) {
    std::cerr << "Failed to read YUV UV-plane data from file." << std::endl;
    return false;
  }

  return true;
}

bool readNV16YUvFile(const std::string &filename,
                     std::vector<uint8_t> &yPlane,
                     std::vector<uint8_t> &uvPlanes,
                     int width,
                     int height) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return false;
  }
  namespace fs = std::filesystem;
  std::uintmax_t filesize = fs::file_size(filename);

  // Calculate the size of the YUV data (Y component + UV component)
  int ySize = width * height;
  int uvSize = ((width + 1) / 2) * height * 2;

  size_t total = ySize + uvSize;
  if (total != filesize) {
    std::cerr << "Invalid fs " << filename
              << ", local size " + std::to_string(total) + " and perceived " + std::to_string(filesize) << std::endl;
    return false;
  }

  yPlane.resize(ySize);
  uvPlanes.resize(uvSize);

  file.read(reinterpret_cast<char *>(yPlane.data()), ySize);
  if (!file.good()) {
    std::cerr << "Failed to read YUV data from file." << std::endl;
    return false;
  }

  file.read(reinterpret_cast<char *>(uvPlanes.data()), uvSize);
  if (!file.good()) {
    std::cerr << "Failed to read YUV UV-plane data from file." << std::endl;
    return false;
  }

  return true;
}

void saveVectorToFile(const std::vector<uint8_t> &data, const std::string &filename) {
  // Open a file for writing
  namespace fs = std::filesystem;
  if (fs::exists(filename)) {
    // Attempt to delete the file
    if (fs::remove(filename)) {
      std::cout << "File '" << filename << "' deleted successfully." << std::endl;
    } else {
      std::cerr << "Failed to delete file '" << filename << "'." << std::endl;
    }
  } else {
    std::cout << "File '" << filename << "' does not exist." << std::endl;
  }

  std::ofstream outFile(filename, std::ios::binary);

  // Check if the file is open
  if (!outFile.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  // Write the vector data to the file
  outFile.write(reinterpret_cast<const char *>(data.data()), data.size());

  // Check for write errors
  if (!outFile) {
    std::cerr << "Error writing to file: " << filename << std::endl;
  } else {
    std::cout << "Vector data successfully saved to file: " << filename << std::endl;
  }

  // Close the file
  outFile.close();
}

void SaveYUV444ToFile(const uint8_t* y, const uint8_t* u, const uint8_t* v, int width, int height, const std::string& filename, int pixelSize) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file for writing: " << filename << std::endl;
    return;
  }

  // Write YUV444 data to the file
  file.write(reinterpret_cast<const char*>(y), width * height * pixelSize); // 3 bytes per pixel
  file.write(reinterpret_cast<const char*>(u), width * height * pixelSize); // 3 bytes per pixel
  file.write(reinterpret_cast<const char*>(v), width * height * pixelSize); // 3 bytes per pixel

  // Close the file
  file.close();
}
