#include "zbuffer_view.hpp"

#include "image.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <vector>

Image zbufferView(const std::vector<float> &zbuffer, int w, int h) {
  Image zbufferImg(w, h);

  float zMin = std::numeric_limits<float>::max();
  float zMax = std::numeric_limits<float>::lowest();

  for (float z : zbuffer) {
    if (z == std::numeric_limits<float>::max()) {
      continue;
    }

    zMin = std::min(zMin, z);
    zMax = std::max(zMax, z);
  }

  // no depth values
  if (zMin == std::numeric_limits<float>::max()) {
    return zbufferImg;
  }

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      float z = zbuffer[y * w + x];
      float depth = 1.0f - (z - zMin) / (zMax - zMin);
      unsigned char gray = static_cast<unsigned char>(depth * 255.0f);
      zbufferImg.set(x, y, Color{gray, gray, gray});
    }
  }

  return zbufferImg;
}

