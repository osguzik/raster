#pragma once

#include "image.hpp"
#include "render/material.hpp"
#include "render/raster/vertex.hpp"
#include "render/shading.hpp"

#include <vector>

void triangle(const ScreenVertex &v0, const ScreenVertex &v1, const ScreenVertex &v2, Image &img, std::vector<float> &zbuffer,
              const Lighting &lighting, const Material &material);

