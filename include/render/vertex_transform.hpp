#pragma once

#include "math/mat.hpp"
#include "math/vec.hpp"
#include "render/rasterize/vertex.hpp"

ClipVertex toClip(const Vec3 &modelPos, const Vec3 &worldPos,
                  const Vec3 &modelNormal, const Vec2 &modelUV, const Mat4 &mvp,
                  const Mat3 &normalMat);

ScreenVertex toScreen(const ClipVertex &v, const Mat4 &viewport);
