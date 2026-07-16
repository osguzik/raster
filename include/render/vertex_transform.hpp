#pragma once

#include "image.hpp"
#include "math/mat.hpp"
#include "math/vec.hpp"
#include "render/raster/vertex.hpp"

ClipVertex toClip(const Vec3 &modelPos, const Color &color, const Mat4 &mvp);

ScreenVertex toScreen(const ClipVertex &v, const Mat4 &viewport);

