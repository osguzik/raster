#include "image.hpp"
#include "math/transform.hpp"
#include "model.hpp"
#include "raster/triangle.hpp"
#include "raster/vertex.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <numbers>
#include <vector>

constexpr int w = 1024;
constexpr int h = 1024;

constexpr float angle = std::numbers::pi_v<float> / 3.0f;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " /path/to/model.obj\n";
    return 1;
  }

  Image framebuffer(w, h);
  Model model = loadModel(argv[1]);

  std::vector<float> zbuffer(w * h, std::numeric_limits<float>::lowest());

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 modelP0 = rotateY(model.verts[model.faces[i * 3]], angle);
    Vec3 modelP1 = rotateY(model.verts[model.faces[i * 3 + 1]], angle);
    Vec3 modelP2 = rotateY(model.verts[model.faces[i * 3 + 2]], angle);
    
    Color c0{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c1{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c2{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};

    Vec3 screenP0 = {(modelP0.x + 1) * w / 2, h - ((modelP0.y + 1) * h / 2), modelP0.z};
    Vec3 screenP1 = {(modelP1.x + 1) * w / 2, h - ((modelP1.y + 1) * h / 2), modelP1.z};
    Vec3 screenP2 = {(modelP2.x + 1) * w / 2, h - ((modelP2.y + 1) * h / 2), modelP2.z};

    Vertex v0{screenP0, c0};
    Vertex v1{screenP1, c1};
    Vertex v2{screenP2, c2};

    triangle(v0, v1, v2, framebuffer, zbuffer);
  }

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

