#pragma once

#include "math/vec.hpp"

#include <string>
#include <vector>

struct Model {
  std::vector<Vec3> verts;
  std::vector<int> faces;
  std::vector<Vec3> normals;
  std::vector<int> faceNormals;
};

Model loadModel(const std::string& filename);

