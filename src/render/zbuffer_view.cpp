#include "render/zbuffer_view.hpp"

#include "image.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <vector>

Image zbufferView(const std::vector<float> &zbuffer, int w, int h, float near, float far) {
  Image zbufferImg(w, h);

  float minDistance = std::numeric_limits<float>::max();
  float maxDistance = std::numeric_limits<float>::lowest();

  for (float z : zbuffer) {
    if (z == std::numeric_limits<float>::max()) {
      continue;
    }

    float distance = (2.0f * near * far) / (near + far + z * (near - far));

    minDistance = std::min(minDistance, distance);
    maxDistance = std::max(maxDistance, distance);
  }

  // no depth values
  if (minDistance == std::numeric_limits<float>::max()) {
    return zbufferImg;
  }

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      float z = zbuffer[y * w + x];

      unsigned char gray = 255;
      if (z != std::numeric_limits<float>::max()) {
        float distance = (2.0f * near * far) / (near + far + z * (near - far));
        float depth = (maxDistance > minDistance) ? (distance - minDistance) / (maxDistance - minDistance) : 0.0f;
        gray = static_cast<unsigned char>(depth * 255.0f);
      }

      zbufferImg.set(x, y, Color{gray, gray, gray});
    }
  }

  return zbufferImg;
}

