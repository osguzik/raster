#include "image.hpp"

Image::Image(int w, int h, Color fill) : w(w), h(h), pixels(w * h * 3) {
  for(int i = 0; i < w * h; i++) {
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
  int i = (x + y * w) * 3;
  pixels[i] = color.r;
  pixels[i + 1] = color.g;
  pixels[i + 2] = color.b;
}

