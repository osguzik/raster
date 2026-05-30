#include <cmath>
#include <utility>
#include "image.hpp"

constexpr Color white = {255, 255, 255};
constexpr Color red = {255, 0, 0};
constexpr Color green = {0, 255, 0};
constexpr Color blue = {0, 0, 255};
constexpr Color yellow = {255, 255, 0};

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
  Image framebuffer(64, 64);

  framebuffer.set(2, 2, white);
  framebuffer.set(3, 2, red);
  framebuffer.set(4, 2, green);
  framebuffer.set(5, 2, blue);
  framebuffer.set(6, 2, yellow);

  line(15, 5, 25, 55, framebuffer, red);
  line(25, 55, 55, 35, framebuffer, green);
  line(55, 35, 15, 5, framebuffer, blue);

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

