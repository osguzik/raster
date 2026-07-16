#pragma once

#include "image.hpp"
#include "render/raster/vertex.hpp"

#include <vector>

void triangle(const ScreenVertex &v0, const ScreenVertex &v1, const ScreenVertex &v2, Image &img, std::vector<float> &zbuffer);

