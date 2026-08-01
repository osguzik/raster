#include "render/vertex_transform.hpp"

#include "math/mat.hpp"
#include "math/vec.hpp"
#include "render/raster/vertex.hpp"

ClipVertex toClip(const Vec3 &modelPos, const Vec3 &modelNormal, const Vec2 &modelUV, const Mat4 &mvp, const Mat3 &normalMat) {
  Vec4 clip = mvp * Vec4{modelPos.x, modelPos.y, modelPos.z, 1.0f};
  Vec3 normal = normalize(normalMat * modelNormal);
  return {clip, modelUV, normal};
}

ScreenVertex toScreen(const ClipVertex &v, const Mat4 &viewport) {
  float invW = 1.0f / v.pos.w;
  Vec3 ndc = {v.pos.x * invW, v.pos.y * invW, v.pos.z * invW};
  Vec4 screen = viewport * Vec4{ndc.x, ndc.y, ndc.z, 1.0f};
  return {{screen.x, screen.y, screen.z}, v.uv, v.normal, invW};
}

