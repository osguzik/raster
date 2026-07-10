#pragma once

#include "image.hpp"

#include <vector>

Image zbufferView(const std::vector<float> &zbuffer, int w, int h);

