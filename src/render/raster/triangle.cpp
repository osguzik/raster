#include "render/raster/triangle.hpp"

#include "image.hpp"
#include "math/vec.hpp"
#include "render/raster/vertex.hpp"
#include "render/shading.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

float edge(const Vec3 &p, const Vec3 &a, const Vec3 &dir) {
  return (p.x - a.x) * dir.y - (p.y - a.y) * dir.x;
}

void triangle(const ScreenVertex &v0, const ScreenVertex &v1, const ScreenVertex &v2, Image &img, std::vector<float> &zbuffer,
              const Vec3 &normal, const Lighting &lighting, const Color &baseColor) {
  const Vec3 &p0 = v0.pos;
  const Vec3 &p1 = v1.pos;
  const Vec3 &p2 = v2.pos;

  float signedArea = (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
  if (signedArea >= 0.0f) { // back-facing
    return;
  }
  float totalArea = std::abs(signedArea);

  Vec3 dir0 = {p1.x - p0.x, p1.y - p0.y};
  Vec3 dir1 = {p2.x - p1.x, p2.y - p1.y};
  Vec3 dir2 = {p0.x - p2.x, p0.y - p2.y};

  int minX = static_cast<int>(std::floor(std::min({p0.x, p1.x, p2.x})));
  int maxX = static_cast<int>(std::ceil(std::max({p0.x, p1.x, p2.x})));
  int minY = static_cast<int>(std::floor(std::min({p0.y, p1.y, p2.y})));
  int maxY = static_cast<int>(std::ceil(std::max({p0.y, p1.y, p2.y})));

  minX = std::max(minX, 0);
  maxX = std::min(maxX, img.w - 1);
  minY = std::max(minY, 0);
  maxY = std::min(maxY, img.h - 1);

  Vec3 start = {minX + 0.5f, minY + 0.5f};
  float rowE0 = edge(start, p0, dir0);
  float rowE1 = edge(start, p1, dir1);
  float rowE2 = edge(start, p2, dir2);

  Color shadeColor = shade(normal, lighting, baseColor);

  for(int y = minY; y <= maxY; y++) {
    float e0 = rowE0, e1 = rowE1, e2 = rowE2;
    for(int x = minX; x <= maxX; x++) {
      if(e0 >= 0 && e1 >= 0 && e2 >= 0) {
        float w0 = e1 / totalArea;
        float w1 = e2 / totalArea;
        float w2 = e0 / totalArea;

        float z = w0 * p0.z + w1 * p1.z + w2 * p2.z;
        int idx = y * img.w + x;
        if (z < zbuffer[idx]) {
          zbuffer[idx] = z;
          img.set(x, y, shadeColor);
        }
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

