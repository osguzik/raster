#pragma once

#include "math/mat.hpp"
#include "math/vec.hpp"

struct Camera {
  Vec3 eye;
  Vec3 at;
  Vec3 up;
  float fovY;
  float ar;
  float near;
  float far;
};

Mat4 viewMat(const Camera &camera);

Mat4 projectionMat(const Camera &camera);

