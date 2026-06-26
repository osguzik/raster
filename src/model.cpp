#include <fstream>
#include <iostream>
#include "model.hpp"

Model loadModel(const std::string& filename) {
  Model model;
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "error: failed to read file " << filename << "\n";
    return model;
  }

  return model;
}

