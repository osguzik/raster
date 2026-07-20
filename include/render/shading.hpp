#pragma once

#include "image.hpp"
#include "math/vec.hpp"

Color shade(const Vec3 &normal, const Vec3 &lightDir, const Vec3 &viewDir, const Color &baseColor, float ambient, float shine);

