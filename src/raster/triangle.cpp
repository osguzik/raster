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

  for(int y = minY; y <= maxY; y++) {
    for(int x = minX; x <= maxX; x++) {
      Vec2 p = {x + 0.5f, y + 0.5f};

      float e0 = edge(p, v0, dir0);
      float e1 = edge(p, v1, dir1);
      float e2 = edge(p, v2, dir2);

      if(e0 >= 0 && e1 >= 0 && e2 >= 0) {
        img.set(x, y, color);
      }
    }
  }
}

