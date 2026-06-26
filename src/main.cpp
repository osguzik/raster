#include <cmath>
#include <utility>
#include "image.hpp"
#include "model.hpp"

constexpr int w = 1024;
constexpr int h = 1024;

constexpr Color green = {0, 255, 0};
/*
constexpr Color white = {255, 255, 255};
constexpr Color red = {255, 0, 0};
constexpr Color blue = {0, 0, 255};
constexpr Color yellow = {255, 255, 0};
*/

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

int main() {
  Image framebuffer(w, h);
  Model model = loadModel("../obj/diablo3_pose.obj");

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    for (int j = 0; j < 3; j++) {
      Vec3 v0 = model.verts[model.faces[i * 3 + j]];
      Vec3 v1 = model.verts[model.faces[i * 3 + (j + 1) % 3]];
      int x0 = (v0.x + 1) * w / 2;
      int y0 = h - ((v0.y + 1) * h / 2);
      int x1 = (v1.x + 1) * w / 2;
      int y1 = h - ((v1.y + 1) * h / 2);
      line(x0, y0, x1, y1, framebuffer, green);
    }
  }

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

