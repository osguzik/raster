#pragma once

#include "image.hpp"
#include "math/vec.hpp"

Color shade(const Vec3 &normal, const Vec3 &light, const Color &baseColor, float ambient);

