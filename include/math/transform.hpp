#pragma once

#include "mat.hpp"
#include "vec.hpp"

#include <cmath>

inline Mat4 translate(Vec3 t) {
  return Mat4{{
    {1, 0, 0, t.x},
    {0, 1, 0, t.y},
    {0, 0, 1, t.z},
    {0, 0, 0, 1}
  }};
}

inline Mat4 scale(Vec3 s) {
  return Mat4{{
    {s.x, 0, 0, 0},
    {0, s.y, 0, 0},
    {0, 0, s.z, 0},
    {0, 0, 0, 1}
  }};
}

inline Mat4 rotateX(float rad) {
  return Mat4{{
  {1, 0, 0, 0},
  {0, std::cos(rad), -std::sin(rad), 0},
  {0, std::sin(rad), std::cos(rad), 0},
  {0, 0, 0, 1}
  }};
}

inline Mat4 rotateY(float rad) {
  return Mat4{{
    {std::cos(rad), 0, std::sin(rad), 0},
    {0, 1, 0, 0},
    {-std::sin(rad), 0, std::cos(rad), 0},
    {0, 0, 0, 1}
  }};
}

inline Mat4 rotateZ(float rad) {
  return Mat4{{
  {std::cos(rad), -sin(rad), 0, 0},
  {std::sin(rad), std::cos(rad), 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
  }};
}

