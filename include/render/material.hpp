#pragma once

#include "image.hpp"

struct Material {
  Color baseColor;
  const Image *texture = nullptr;
  float specularStrength;
  float shininess;
};

inline Image checkerTexture(int size, int sqSize) {
  Image img(size, size);
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      bool dark = ((x / sqSize) + (y / sqSize)) % 2 == 0;
      img.set(x, y, dark ? Color{64, 64, 64} : Color{128, 128, 128});
    }
  }
  return img;
}

