#include "raster/triangle.hpp"

#include <algorithm>
#include <cmath>

float edge(const Vec2 &p, const Vec2 &a, const Vec2 &dir) {
  return (p.x - a.x) * dir.y - (p.y - a.y) * dir.x;
}

void triangle(const Vec2 &v0, const Vec2 &v1, const Vec2 &v2, Image &img, const Color &color) {
  Vec2 dir0 = {v1.x - v0.x, v1.y - v0.y};
  Vec2 dir1 = {v2.x - v1.x, v2.y - v1.y};
  Vec2 dir2 = {v0.x - v2.x, v0.y - v2.y};

  int minX = static_cast<int>(std::floor(std::min({v0.x, v1.x, v2.x})));
  int maxX = static_cast<int>(std::ceil(std::max({v0.x, v1.x, v2.x})));
  int minY = static_cast<int>(std::floor(std::min({v0.y, v1.y, v2.y})));
  int maxY = static_cast<int>(std::ceil(std::max({v0.y, v1.y, v2.y})));

  Vec2 start = {minX + 0.5f, minY + 0.5f};
  float rowE0 = edge(start, v0, dir0);
  float rowE1 = edge(start, v1, dir1);
  float rowE2 = edge(start, v2, dir2);

  for(int y = minY; y <= maxY; y++) {
    float e0 = rowE0, e1 = rowE1, e2 = rowE2;
    for(int x = minX; x <= maxX; x++) {
      if(e0 >= 0 && e1 >= 0 && e2 >= 0) {
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

