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
    } else if (keyword == "f") {}
  }

  return model;
}

