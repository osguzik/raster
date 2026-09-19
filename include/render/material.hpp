#pragma once

#include "image.hpp"

struct Material {
  Color baseColor;
  const Image *texture = nullptr;
  float specularStrength;
  float shininess;
  bool useVertexColor = false;
};
