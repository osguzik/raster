#pragma once

#include "image.hpp"
#include "raster/vertex.hpp"

#include <vector>

void triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, Image &img, std::vector<float> &zbuffer);

