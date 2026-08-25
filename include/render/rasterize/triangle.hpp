#pragma once

#include "image.hpp"
#include "math/mat.hpp"
#include "math/vec.hpp"
#include "render/material.hpp"
#include "render/rasterize/vertex.hpp"
#include "render/shading/lighting.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

inline float edge(const Vec3 &p, const Vec3 &a, const Vec3 &dir) {
  return (p.x - a.x) * dir.y - (p.y - a.y) * dir.x;
}

template <typename FragmentShader>
void rasterize(const ScreenVertex &v0, const ScreenVertex &v1, const ScreenVertex &v2,
               int w, int h, FragmentShader fragmentShader) {
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

  int minX = std::max(static_cast<int>(std::floor(std::min({p0.x, p1.x, p2.x}))), 0);
  int maxX = std::min(static_cast<int>(std::ceil(std::max({p0.x, p1.x, p2.x}))), w - 1);
  int minY = std::max(static_cast<int>(std::floor(std::min({p0.y, p1.y, p2.y}))), 0);
  int maxY = std::min(static_cast<int>(std::ceil(std::max({p0.y, p1.y, p2.y}))), h - 1);
  if (minX > maxX || minY > maxY) {
    return;
  }

  Vec3 start = {minX + 0.5f, minY + 0.5f};
  float rowE0 = edge(start, p0, dir0);
  float rowE1 = edge(start, p1, dir1);
  float rowE2 = edge(start, p2, dir2);

  for (int y = minY; y <= maxY; y++) {
    float e0 = rowE0, e1 = rowE1, e2 = rowE2;
    for (int x = minX; x <= maxX; x++) {
      if (e0 >= 0.0f && e1 >= 0.0f && e2 >= 0.0f) {
        float w0 = e1 / totalArea;
        float w1 = e2 / totalArea;
        float w2 = e0 / totalArea;
        fragmentShader(x, y, w0, w1, w2);
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

void triangle(const ScreenVertex &v0, const ScreenVertex &v1, const ScreenVertex &v2,
              Image &img, std::vector<float> &zbuffer,
              Lighting lighting, const Material &material, const Vec3 &eye,
              const Mat4 &lightSpaceMat, const std::vector<float> &shadowMap);

