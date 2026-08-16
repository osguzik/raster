#pragma once

#include "image.hpp"
#include "render/material.hpp"
#include "math/vec.hpp"

struct Lighting {
  Vec3 lightDir;
  Vec3 viewDir;
  float ambient;
  Color lightColor;
};

Color blinnPhong(const Vec3 &normal, const Vec2 &uv, const Lighting &lighting, const Material &material, float shadow);

