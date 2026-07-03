#include "image.hpp"
#include "model.hpp"
#include "raster/triangle.hpp"

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
    
    Color rnd;
    rnd.r = std::rand() % 255;
    rnd.g = std::rand() % 255;
    rnd.b = std::rand() % 255;

    Vec2 p0 = {(v0.x + 1) * w / 2, h - ((v0.y + 1) * h / 2)};
    Vec2 p1 = {(v1.x + 1) * w / 2, h - ((v1.y + 1) * h / 2)};
    Vec2 p2 = {(v2.x + 1) * w / 2, h - ((v2.y + 1) * h / 2)};

    triangle(p0, p1, p2, framebuffer, rnd);
  }

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

