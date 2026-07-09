#pragma once

#include "mat.hpp"
#include "vec.hpp"

#include <cmath>

inline Vec3 rotateY(const Vec3 &v, float angle) {
  Mat3 Ry = {{
  {std::cos(angle), 0, std::sin(angle)},
  {0, 1, 0},
  {-std::sin(angle), 0, std::cos(angle)}
  }};
  return Ry * v;
}

