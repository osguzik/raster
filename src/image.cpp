#include <fstream>
#include <iostream>
#include <string>
#include "image.hpp"

Image::Image(int w, int h, Color fill) : w(w), h(h), pixels(w * h * 3) {
  for (int i = 0; i < w * h; i++) {
    pixels[i * 3] = fill.r;
    pixels[i * 3 + 1] = fill.g;
    pixels[i * 3 + 2] = fill.b;
  }
}

Color Image::get(int x, int y) const {
  int i = (x + y * w) * 3;
  return {pixels[i], pixels[i + 1], pixels[i + 2]};
}

void Image::set(int x, int y, const Color &color) {
  uint8_t* row = pixels.data() + y * w * 3;
  int i = x * 3;
  row[i] = color.r;
  row[i + 1] = color.g;
  row[i + 2] = color.b;
}

namespace ppm {
  bool write(const Image &img, const std::string &filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
      std::cerr << "error: failed to open file " << filename << "\n";
      return false;
    }
    out << "P6\n" << img.w << " " << img.h << "\n255\n";
    out.write(reinterpret_cast<const char *>(img.pixels.data()), img.pixels.size());
    if (!out.good()) {
      std::cerr << "error: failed to write to file " << filename << "\n";
      return false;
    }
    return true;
  }
}

