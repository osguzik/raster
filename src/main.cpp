#include <utility>
#include "image.hpp"

constexpr Color white = {255, 255, 255};

constexpr Color red = {255, 0, 0};
constexpr Color green = {0, 255, 0};
constexpr Color blue = {0, 0, 255};

constexpr Color yellow = {255, 255, 0};

void line(int x0, int y0, int x1, int y1, Image &img, const Color &color) {
  if(x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = y1 - y0;
  int err = 2*dy - dx;

  for(int x = x0, y = y0; x <= x1; x++) {
    img.set(x, y, color);
    err += 2*dy;

    if(err >= 0) {
      y++;
      err -= 2*dx;
    }
  }
}

int main() {
  Image framebuffer(64, 64);

  framebuffer.set(2, 2, white);
  framebuffer.set(4, 2, red);
  framebuffer.set(5, 2, green);
  framebuffer.set(6, 2, blue);
  framebuffer.set(8, 2, yellow);

  line(50, 30, 10, 10, framebuffer, white);

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

