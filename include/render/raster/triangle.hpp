#pragma once

#include "image.hpp"
#include "math/vec.hpp"
#include "render/raster/vertex.hpp"

#include <vector>

void triangle(const ScreenVertex &v0, const ScreenVertex &v1, const ScreenVertex &v2, Image &img, std::vector<float> &zbuffer,
              const Vec3 &normal, const Vec3 &lightDir, const Vec3 &viewDir, const Color &baseColor, float ambient, float shininess);

