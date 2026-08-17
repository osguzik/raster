#include "image.hpp"
#include "render/shading/shadow.hpp"
#include "render/zbuffer_view.hpp"
#include "scene.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <dragons>\n";
    return 1;
  }

  Scene scene;
  int w, h;
  std::string name = argv[1];

  if (name == "dragons") {
    w = 2048; h = 1024;
    scene = buildDragonsScene(w, h);
  } else {
    std::cerr << "error: unknown scene " << name << "\n";
    return 1;
  }

  Image framebuffer(w, h);
  std::vector<float> zbuffer(w * h, std::numeric_limits<float>::max());
  std::vector<float> shadowMap(w * h);

  renderShadowMap(scene, scene.lightSpaceMat, /*near=*/0.1f, shadowMap, w, h);
  renderScene(scene, framebuffer, zbuffer, w, h, shadowMap);
  Image zbufferImg = zbufferView(zbuffer, w, h, scene.camera.near, scene.camera.far);

  ppm::write(framebuffer, "framebuffer.ppm");
  ppm::write(zbufferImg, "zbuffer.ppm");
  return 0;
}

