#include "image.hpp"
#include "render/zbuffer_view.hpp"
#include "scene.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <dragon>\n";
    return 1;
  }

  Scene scene;
  int w, h;
  std::string name = argv[1];

  if (name == "dragon") {
    w = 1024, h = 1024;
    scene = buildDragonScene(w, h);
  } else {
    std::cerr << "error: unknown scene " << name << "\n";
    return 1;
  }

  Image framebuffer(w, h);
  std::vector<float> zbuffer(w * h, std::numeric_limits<float>::max());

  renderScene(scene, framebuffer, zbuffer, w, h);
  Image zbufferImg = zbufferView(zbuffer, w, h);

  ppm::write(framebuffer, "framebuffer.ppm");
  ppm::write(zbufferImg, "zbuffer.ppm");
  return 0;
}

