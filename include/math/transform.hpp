#pragma once

#include "mat.hpp"

#include <cmath>

inline Mat4 rotateX(float angle) {
  return Mat4{{
  {1, 0, 0, 0},
  {0, std::cos(angle), -std::sin(angle), 0},
  {0, std::sin(angle), std::cos(angle), 0},
  {0, 0, 0, 1}
  }};
}

inline Mat4 rotateY(float angle) {
  return Mat4{{
    {std::cos(angle), 0, std::sin(angle), 0},
    {0, 1, 0, 0},
    {-std::sin(angle), 0, std::cos(angle), 0},
    {0, 0, 0, 1}
  }};
}

inline Mat4 rotateZ(float angle) {
  return Mat4{{
  {std::cos(angle), -sin(angle), 0, 0},
  {std::sin(angle), std::cos(angle), 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
  }};
}

