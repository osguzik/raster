#pragma once

#include "mat.hpp"

#include <cmath>

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

