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

struct Mat4 {
  Vec4 rows[4];

  Vec4 operator*(const Vec4 &v) const {
    return {
      rows[0].x * v.x + rows[0].y * v.y + rows[0].z * v.z + rows[0].w * v.w,
      rows[1].x * v.x + rows[1].y * v.y + rows[1].z * v.z + rows[1].w * v.w,
      rows[2].x * v.x + rows[2].y * v.y + rows[2].z * v.z + rows[2].w * v.w,
      rows[3].x * v.x + rows[3].y * v.y + rows[3].z * v.z + rows[3].w * v.w
    };
  }
};

