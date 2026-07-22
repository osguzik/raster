#include "model.hpp"

#include "math/vec.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Model loadModel(const std::string& filename) {
  Model model;
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "error: failed to read file " << filename << "\n";
    return model;
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
    } else if (keyword == "f") {
      std::string s;
      for (int i = 0; i < 3; i++) {
        iss >> s;
        std::istringstream fss(s);
        std::string field;
        int f, n;
        int fieldIdx = 0;
        while (std::getline(fss, field, '/')) {
          int val = field.empty() ? 0 : std::stoi(field);
          if (fieldIdx == 0) { f = val; }
          else if (fieldIdx == 2) { n = val; }
          fieldIdx++;
        }
        model.faces.push_back(f - 1);
        model.faceNormals.push_back(n - 1);
      }
    }
  }

  return model;
}

