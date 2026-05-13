#include "image.hpp"

int main() {
  Image framebuffer(64, 64);
  framebuffer.set(2, 2, {255, 255, 255});
  ppm::write(framebuffer, "framebuffer.ppm");
  return 0;
}
