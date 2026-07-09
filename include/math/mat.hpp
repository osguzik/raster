#pragma once

#include "math/vec.hpp"

struct Mat3 {
  Vec3 rows[3];

  Vec3 operator*(const Vec3 &v) const {
    return {
      rows[0].x * v.x + rows[0].y * v.y + rows[0].z * v.z,
      rows[1].x * v.x + rows[1].y * v.y + rows[1].z * v.z,
      rows[2].x * v.x + rows[2].y * v.y + rows[2].z * v.z
    };
  }
};

