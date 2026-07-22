#pragma once

#include "math/mat.hpp"
#include "math/vec.hpp"
#include "render/raster/vertex.hpp"

ClipVertex toClip(const Vec3 &modelPos, const Vec3 &modelNormal, const Mat4 &mvp, const Mat3 &normalMat);

ScreenVertex toScreen(const ClipVertex &v, const Mat4 &viewport);

