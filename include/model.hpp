#pragma once

#include <string>
#include <vector>
#include "math/vec.hpp"

struct Model {
  std::vector<Vec3> verts;
  std::vector<int> faces;
};

Model loadModel(const std::string& filename);

