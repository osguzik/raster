#include "raster/triangle.hpp"

#include "raster/line.hpp"

void triangle(Vec3 v0, Vec3 v1, Vec3 v2, Image &img, const Color &color) {
  line(v0.x, v0.y, v1.x, v1.y, img, color);
  line(v1.x, v1.y, v2.x, v2.y, img, color);
  line(v2.x, v2.y, v0.x, v0.y, img, color);
}

