#pragma once

#include "math/mat.hpp"
#include "math/vec.hpp"
#include "render/material.hpp"
#include "render/shading/lighting.hpp"

#include <vector>

struct ShadingContext {
  Lighting lighting;
  const Material &material;
  Vec3 eye;
  Mat4 lightSpaceMat;
  const std::vector<float> &shadowMap;
};
