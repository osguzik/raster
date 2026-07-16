#include "camera.hpp"
#include "image.hpp"
#include "math/transform.hpp"
#include "math/vec.hpp"
#include "model.hpp"
#include "raster/triangle.hpp"
#include "raster/vertex.hpp"
#include "vertex_transform.hpp"
#include "zbuffer_view.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <numbers>
#include <vector>

constexpr int w = 1024;
constexpr int h = 1024;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " /path/to/model.obj\n";
    return 1;
  }

  Image framebuffer(w, h);
  Model model = loadModel(argv[1]);
  std::vector<float> zbuffer(w * h, std::numeric_limits<float>::max());

  Transform t;
  t.position = {0.0f, 0.0f, -2.0f};
  t.rotation.y = std::numbers::pi_v<float> / 6.0f;
  t.scale = {1.0f, 1.0f, 1.0f};

  Camera camera{
    .eye = {0.0f, 0.0f, 1.0f},
    .at = t.position,
    .up = {0.0f, 1.0f, 0.0f},
    .fovY = std::numbers::pi_v<float> / 4.0f,
    .ar = static_cast<float>(w) / h,
    .near = 0.1f,
    .far = 10.0f
  };

  Mat4 mvp = projectionMat(camera) * viewMat(camera) * modelMat(t);
  Mat4 vp = viewport(0, 0, w, h);

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Color c0{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c1{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c2{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};

    ClipVertex clip0 = toClip(model.verts[model.faces[i * 3]], c0, mvp);
    ClipVertex clip1 = toClip(model.verts[model.faces[i * 3 + 1]], c1, mvp);
    ClipVertex clip2 = toClip(model.verts[model.faces[i * 3 + 2]], c2, mvp);

    ScreenVertex v0 = toScreen(clip0, vp);
    ScreenVertex v1 = toScreen(clip1, vp);
    ScreenVertex v2 = toScreen(clip2, vp);

    triangle(v0, v1, v2, framebuffer, zbuffer);
  }

  Image zbufferImg = zbufferView(zbuffer, w, h);

  ppm::write(framebuffer, "framebuffer.ppm");
  ppm::write(zbufferImg, "zbuffer.ppm");
  return 0;
}

