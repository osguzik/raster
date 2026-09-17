#include "image.hpp"
#include "progress.hpp"
#include "render/post/bloom.hpp"
#include "render/shading/shadow.hpp"
#include "render/zbuffer_view.hpp"
#include "scene.hpp"

#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <dragons>\n";
    return 1;
  }

  std::string name = argv[1];
  int frameNum = 1;
  if (argc == 4 && std::string(argv[2]) == "--frames") {
    frameNum = std::atoi(argv[3]);
  }

  Scene scene;
  int w, h;

  std::string ppmDir = std::string(RENDERS_DIR) + "ppm/";
  fs::remove_all(ppmDir);
  fs::create_directories(ppmDir);

  for (int frame = 0; frame < frameNum; frame++) {
    progressBar(frame, frameNum);
    float t = (frameNum == 1 ? 0.0f : static_cast<float>(frame) / frameNum);

    if (name == "dragons") {
      w = 2048; h = 1024;
      scene = buildDragonsScene(w, h, t);
    } else if (name == "bunny") {
      w = 512; h = 512;
      scene = buildBunnyScene(w, h);
    } else {
      std::cerr << "error: unknown scene " << name << "\n";
      return 1;
    }

    Image framebuffer(w, h);
    std::vector<float> zbuffer(w * h, std::numeric_limits<float>::max());
    std::vector<float> shadowMap(w * h);

    renderShadowMap(scene, scene.lightSpaceMat, /*near=*/0.1f, shadowMap, w, h);
    renderScene(scene, framebuffer, zbuffer, w, h, shadowMap);
    if (scene.useBloom) {
      applyBloom(framebuffer, /*threshold=*/0.7f, /*blurRadius=*/10, /*intensity=*/0.8f);
    }

    Image zbufferImg = zbufferView(zbuffer, w, h, scene.camera.near, scene.camera.far);

    std::string suffix = (frameNum == 1 ? "" : std::format("{:04d}", frame));
    ppm::write(framebuffer, ppmDir + name + suffix + "_framebuffer.ppm");
    ppm::write(zbufferImg, ppmDir + name + suffix + "_zbuffer.ppm");
  }

  std::clog << '\n';
  return 0;
}

