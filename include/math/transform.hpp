#pragma once

#include "mat.hpp"

#include <cmath>

inline Mat4 rotateY(float angle) {
  return Mat4{{
    {std::cos(angle), 0, std::sin(angle), 0},
    {0, 1, 0, 0},
    {-std::sin(angle), 0, std::cos(angle), 0},
    {0, 0, 0, 1}
  }};
}

