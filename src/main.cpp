#include "image.hpp"
#include "model.hpp"
#include "raster/triangle.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>

constexpr int w = 1024;
constexpr int h = 1024;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " /path/to/model.obj\n";
    return 1;
  }

  Image framebuffer(w, h);
  Model model = loadModel(argv[1]);

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 v0 = model.verts[model.faces[i * 3]];
    Vec3 v1 = model.verts[model.faces[i * 3 + 1]];
    Vec3 v2 = model.verts[model.faces[i * 3 + 2]];
    
    Color c0{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c1{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c2{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};

    Vec3 p0 = {(v0.x + 1) * w / 2, h - ((v0.y + 1) * h / 2), v0.z};
    Vec3 p1 = {(v1.x + 1) * w / 2, h - ((v1.y + 1) * h / 2), v1.z};
    Vec3 p2 = {(v2.x + 1) * w / 2, h - ((v2.y + 1) * h / 2), v2.z};

    triangle(p0, p1, p2, c0, c1, c2, framebuffer);
  }

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

