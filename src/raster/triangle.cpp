#include "raster/triangle.hpp"

#include "raster/line.hpp"

void triangle(const Vec2 &v0, const Vec2 &v1, const Vec2 &v2, Image &img, const Color &color) {
  line(v0.x, v0.y, v1.x, v1.y, img, color);
  line(v1.x, v1.y, v2.x, v2.y, img, color);
  line(v2.x, v2.y, v0.x, v0.y, img, color);
}

