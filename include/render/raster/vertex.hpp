#pragma once

#include "image.hpp"
#include "math/vec.hpp"

struct ClipVertex {
  Vec4 pos;
  Color color;
};

struct ScreenVertex {
  Vec3 pos;
  Color color;
};

