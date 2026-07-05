#include "raster/triangle.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>

float edge(const Vec3 &p, const Vec3 &a, const Vec3 &dir) {
  return (p.x - a.x) * dir.y - (p.y - a.y) * dir.x;
}

void triangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Color &c0, const Color &c1, const Color &c2, Image &img) {
  float signedArea = (v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x);
  if (signedArea >= 0.0f) { // back-facing
    return;
  }
  float totalArea = std::abs(signedArea);

  Vec3 dir0 = {v1.x - v0.x, v1.y - v0.y};
  Vec3 dir1 = {v2.x - v1.x, v2.y - v1.y};
  Vec3 dir2 = {v0.x - v2.x, v0.y - v2.y};

  int minX = static_cast<int>(std::floor(std::min({v0.x, v1.x, v2.x})));
  int maxX = static_cast<int>(std::ceil(std::max({v0.x, v1.x, v2.x})));
  int minY = static_cast<int>(std::floor(std::min({v0.y, v1.y, v2.y})));
  int maxY = static_cast<int>(std::ceil(std::max({v0.y, v1.y, v2.y})));

  Vec3 start = {minX + 0.5f, minY + 0.5f};
  float rowE0 = edge(start, v0, dir0);
  float rowE1 = edge(start, v1, dir1);
  float rowE2 = edge(start, v2, dir2);

  for(int y = minY; y <= maxY; y++) {
    float e0 = rowE0, e1 = rowE1, e2 = rowE2;
    for(int x = minX; x <= maxX; x++) {
      if(e0 >= 0 && e1 >= 0 && e2 >= 0) {
        float w0 = e1 / totalArea;
        float w1 = e2 / totalArea;
        float w2 = e0 / totalArea;

        float z = w0 * v0.z + w1 * v1.z + w2 * v2.z;

        Color color;
        color.r = static_cast<std::uint8_t>(w0 * c0.r + w1 * c1.r + w2 * c2.r);
        color.g = static_cast<std::uint8_t>(w0 * c0.g + w1 * c1.g + w2 * c2.g);
        color.b = static_cast<std::uint8_t>(w0 * c0.b + w1 * c1.b + w2 * c2.b);

        img.set(x, y, color);
      }

      e0 += dir0.y;
      e1 += dir1.y;
      e2 += dir2.y;
    }

    rowE0 -= dir0.x;
    rowE1 -= dir1.x;
    rowE2 -= dir2.x;
  }
}

