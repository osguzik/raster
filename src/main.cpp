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
    std::cerr << "usage: " << argv[0] << " <bunny|dragons> [--frames N]\n";
    return 1;
  }

  std::string name = argv[1];
  int frameNum = 1;
  if (argc == 4 && std::string(argv[2]) == "--frames") {
    frameNum = std::atoi(argv[3]);
  }

  Scene scene;
  int w, h;

  std::string ppmDir = std::string(RENDERS_DIR) + "/ppm/";
  fs::remove_all(ppmDir);
  fs::create_directories(ppmDir);

  for (int frame = 0; frame < frameNum; frame++) {
    progressBar(frame, frameNum);
    float t = (frameNum == 1 ? 0.0f : static_cast<float>(frame) / frameNum);

    if (name == "bunny") {
      w = 1024;
      h = 720;
      scene = buildBunnyScene(w, h);
    } else if (name == "depth_demo") {
      w = 1024;
      h = 720;
      scene = buildDepthDemoScene(w, h, t);
    } else if (name == "dragons") {
      w = 2048;
      h = 1024;
      scene = buildDragonsScene(w, h, t);
    } else {
      std::clog << '\n';
      std::cerr << "error: unknown scene " << name << "\n";
      return 1;
    }

    Image framebuffer(w, h, scene.backgroundColor);
    std::vector<float> zbuffer(w * h, std::numeric_limits<float>::max());
    std::vector<float> shadowMap(w * h);

    renderShadowMap(scene, scene.lightSpaceMat, /*near=*/0.1f, shadowMap, w, h);
    renderScene(scene, framebuffer, zbuffer, w, h, shadowMap);
    if (scene.useBloom) {
      applyBloom(framebuffer, /*threshold=*/0.7f, /*blurRadius=*/10,
                 /*intensity=*/0.8f);
    }

    Image zbufferImg =
        zbufferView(zbuffer, w, h, scene.camera.near, scene.camera.far);

    std::string suffix = (frameNum == 1 ? "" : std::format("{:04d}", frame));
    std::string framebufferPath = ppmDir + name + suffix + "_framebuffer.ppm";
    std::string zbufferPath = ppmDir + name + suffix + "_zbuffer.ppm";

    ppm::write(framebuffer, framebufferPath);
    ppm::write(zbufferImg, zbufferPath);

    if (frameNum == 1) {
      std::clog << '\n';
      std::clog << "wrote " << framebufferPath << '\n';
      std::clog << "wrote " << zbufferPath << '\n';
    }
  }

  if (frameNum > 1) {
    std::clog << '\n';
    std::clog << "wrote " << frameNum << " frames of " << name
              << "*_framebuffer to " << ppmDir << '\n';
    std::clog << "wrote " << frameNum << " frames of " << name
              << "*_zbuffer to " << ppmDir << '\n';
  }

  return 0;
}
