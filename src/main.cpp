#include <cmath>
#include <iostream>
#include <utility>
#include "image.hpp"
#include "model.hpp"

constexpr int w = 1024;
constexpr int h = 1024;

constexpr Color green = {0, 255, 0};

void line(int x0, int y0, int x1, int y1, Image &img, const Color &color) {
  bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
  if(steep) {
    std::swap(x0, y0);
    std::swap(x1, y1);
  }

  if(x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = std::abs(y1 - y0);

  int err = 2*dy - dx;
  int ystep = (y0 < y1) ? 1 : -1;
  int y = y0;

  for(int x = x0; x <= x1; x++) {
    if(steep) {
      img.set(y, x, color);
    } else {
      img.set(x, y, color);
    }

    if(err >= 0) {
      y += ystep;
      err -= 2*dx;
    }

    err += 2*dy;
  }
}

void triangle(Vec3 v0, Vec3 v1, Vec3 v2, Image &img, const Color &color) {
  line(v0.x, v0.y, v1.x, v1.y, img, color);
  line(v1.x, v1.y, v2.x, v2.y, img, color);
  line(v2.x, v2.y, v0.x, v0.y, img, color);
}

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

    Vec3 p0 = {(v0.x + 1) * w / 2, h - ((v0.y + 1) * h / 2), v0.z};
    Vec3 p1 = {(v1.x + 1) * w / 2, h - ((v1.y + 1) * h / 2), v1.z};
    Vec3 p2 = {(v2.x + 1) * w / 2, h - ((v2.y + 1) * h / 2), v2.z};

    triangle(p0, p1, p2, framebuffer, green);
  }

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

