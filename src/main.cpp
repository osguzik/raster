#include "image.hpp"

constexpr Color white = {255, 255, 255};

constexpr Color red = {255, 0, 0};
constexpr Color green = {0, 255, 0};
constexpr Color blue = {0, 0, 255};

constexpr Color yellow = {255, 255, 0};

int main() {
  Image framebuffer(64, 64);

  framebuffer.set(2, 2, white);
  framebuffer.set(4, 2, red);
  framebuffer.set(5, 2, green);
  framebuffer.set(6, 2, blue);
  framebuffer.set(8, 2, yellow);

  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}

