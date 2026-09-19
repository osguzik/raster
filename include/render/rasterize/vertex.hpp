#pragma once

#include "math/vec.hpp"

struct ClipVertex {
  Vec4 pos;
  Vec3 worldPos;
  Vec2 uv;
  Vec3 normal;
  Vec3 color;
};

struct ScreenVertex {
  Vec3 pos;
  Vec3 worldPos;
  Vec2 uv;
  Vec3 normal;
  Vec3 color;
  float invW;
};
