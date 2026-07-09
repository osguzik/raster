#include "image.hpp"
#include "math/transform.hpp"
#include "math/vec.hpp"
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

Vec3 toScreen(const Vec3 &modelPos, float angle) {
  Vec3 rotated = rotateY(modelPos, angle);
  return {(rotated.x + 1) * w / 2, h - ((rotated.y + 1) * h / 2), rotated.z};
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " /path/to/model.obj\n";
    return 1;
  }

  Image framebuffer(w, h);
  Model model = loadModel(argv[1]);

  std::vector<float> zbuffer(w * h, std::numeric_limits<float>::lowest());

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 screenPos0 = toScreen(model.verts[model.faces[i * 3]], angle);
    Vec3 screenPos1 = toScreen(model.verts[model.faces[i * 3 + 1]], angle);
    Vec3 screenPos2 = toScreen(model.verts[model.faces[i * 3 + 2]], angle);

    Color c0{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c1{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c2{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};

    Vertex v0{screenPos0, c0};
    Vertex v1{screenPos1, c1};
    Vertex v2{screenPos2, c2};

    triangle(v0, v1, v2, framebuffer, zbuffer);
  }

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

