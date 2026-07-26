#include "model.hpp"

#include "math/vec.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void recalcNormals(Model &model) {
  size_t faceCount = model.faces.size() / 3;
  std::vector<Vec3> vertexNormals(model.verts.size(), Vec3{0.0f, 0.0f, 0.0f});

  for (size_t i = 0; i < faceCount; i++) {
    int v0 = model.faces[i * 3], v1 = model.faces[i * 3 + 1], v2 = model.faces[i * 3 + 2];
    Vec3 edge1 = model.verts[v1] - model.verts[v0];
    Vec3 edge2 = model.verts[v2] - model.verts[v0];
    Vec3 faceNormal = cross(edge1, edge2);
    vertexNormals[v0] = vertexNormals[v0] + faceNormal;
    vertexNormals[v1] = vertexNormals[v1] + faceNormal;
    vertexNormals[v2] = vertexNormals[v2] + faceNormal;
  }

  for (Vec3 &n : vertexNormals) {
    n = normalize(n);
  }

  model.normals = vertexNormals;
  model.normalFaces = model.faces;
}

Model loadModel(const std::string& filename, bool shouldRecalcNormals) {
  Model model;
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "error: failed to read file " << filename << "\n";
    exit(1);
  }

  std::string line;
  while (std::getline(in, line)) {
    std::istringstream iss(line);
    std::string keyword;

    iss >> keyword;
    if (keyword == "v") {
      Vec3 v;
      iss >> v.x >> v.y >> v.z;
      model.verts.push_back(v);
    } else if (keyword == "vn") {
      Vec3 normal;
      iss >> normal.x >> normal.y >> normal.z;
      model.normals.push_back(normalize(normal));
    } else if (keyword == "vt") {
      Vec2 uv;
      iss >> uv.x >> uv.y;
      model.uvs.push_back(uv);
    } else if (keyword == "f") {
      std::string s;
      for (int i = 0; i < 3; i++) {
        iss >> s;
        std::istringstream fss(s);
        std::string field;
        int f, t, n;
        int fieldIdx = 0;
        while (std::getline(fss, field, '/')) {
          int val = field.empty() ? 0 : std::stoi(field);
          if (fieldIdx == 0) { f = val; }
          else if (fieldIdx == 1) { t = val; }
          else if (fieldIdx == 2) { n = val; }
          fieldIdx++;
        }
        model.faces.push_back(f - 1);
        model.uvFaces.push_back(t - 1);
        model.normalFaces.push_back(n - 1);
      }
    }
  }

  // uvs are optional
  model.uvsPresent = !model.uvs.empty();
  if (model.uvs.empty()) {
    model.uvs.push_back(Vec2{0.0f, 0.0f});
    model.uvFaces.assign(model.faces.size(), 0);
  }

  // normals are required
  if (model.normals.empty() && !shouldRecalcNormals) {
    std::cerr << "error: file " << filename << " has no vertex normals (vn)\n";
    exit(1);
  }

  if (shouldRecalcNormals) {
    recalcNormals(model);
  }

  return model;
}

