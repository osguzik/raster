#include "vertex_transform.hpp"

#include "image.hpp"
#include "math/mat.hpp"
#include "math/vec.hpp"
#include "raster/vertex.hpp"

ClipVertex toClip(const Vec3 &modelPos, const Color &color, const Mat4 &mvp) {
  Vec4 clip = mvp * Vec4{modelPos.x, modelPos.y, modelPos.z, 1.0f};
  return {clip, color};
}

ScreenVertex toScreen(const ClipVertex &v, const Mat4 &viewport) {
  Vec3 ndc = {v.pos.x / v.pos.w, v.pos.y / v.pos.w, v.pos.z / v.pos.w};
  Vec4 screen = viewport * Vec4{ndc.x, ndc.y, ndc.z, 1.0f};
  return {{screen.x, screen.y, screen.z}, v.color};
}

