#pragma once

#include "math/vec.hpp"

struct ClipVertex {
  Vec4 pos;
  Vec2 uv;
  Vec3 normal;
};

struct ScreenVertex {
  Vec3 pos;
  Vec2 uv;
  Vec3 normal;
};

