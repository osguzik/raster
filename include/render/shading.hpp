#pragma once

#include "image.hpp"
#include "math/vec.hpp"

struct Lighting {
  Vec3 lightDir;
  Vec3 viewDir;
  float ambient;
  float shininess;
};

Color blinnPhong(const Vec3 &normal, const Lighting &lighting, const Color &baseColor);

