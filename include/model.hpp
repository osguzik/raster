#pragma once

#include "math/vec.hpp"

#include <string>
#include <vector>

struct Model {
  std::vector<Vec3> verts;
  std::vector<int> faces;
  std::vector<Vec2> uvs;
  std::vector<int> uvFaces;
  std::vector<Vec3> normals;
  std::vector<int> normalFaces;
  bool uvsPresent = false;
};

Model loadModel(const std::string &filename, bool shouldRecalcNormals = false);
