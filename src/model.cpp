#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "model.hpp"

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
    } else if (keyword == "f") {
      std::string s;
      for (int i = 0; i < 3; i++) {
        iss >> s;
        model.faces.push_back(std::stoi(s) - 1);
      }
    }
  }

  return model;
}

