#include "camera.hpp"
#include "image.hpp"
#include "math/transform.hpp"
#include "math/vec.hpp"
#include "model.hpp"
#include "raster/triangle.hpp"
#include "raster/vertex.hpp"
#include "zbuffer_view.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <numbers>
#include <vector>

constexpr int w = 1024;
constexpr int h = 1024;

constexpr float rotationAngle = std::numbers::pi_v<float> / 6.0f;

Vec3 toScreen(const Vec3 &modelPos, const Transform &t, const Camera &camera) {
  Mat4 m = modelMat(t);
  Mat4 view = viewMat(camera);
  Mat4 projection = projectionMat(camera);

  Vec4 clip = projection * view * m * Vec4{modelPos.x, modelPos.y, modelPos.z, 1.0f};
  Vec3 ndc = {clip.x / clip.w, clip.y / clip.w, clip.z / clip.w};

  Mat4 vp = viewport(0, 0, w, h);
  Vec4 screen = vp * Vec4{ndc.x, ndc.y, ndc.z, 1.0f};
  return {screen.x, screen.y, screen.z};
}

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
  t.rotation.y = rotationAngle;
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

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 screenPos0 = toScreen(model.verts[model.faces[i * 3]], t, camera);
    Vec3 screenPos1 = toScreen(model.verts[model.faces[i * 3 + 1]], t, camera);
    Vec3 screenPos2 = toScreen(model.verts[model.faces[i * 3 + 2]], t, camera);

    Color c0{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c1{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};
    Color c2{static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255), static_cast<std::uint8_t>(std::rand() % 255)};

    ScreenVertex v0{screenPos0, c0};
    ScreenVertex v1{screenPos1, c1};
    ScreenVertex v2{screenPos2, c2};

    triangle(v0, v1, v2, framebuffer, zbuffer);
  }

  Image zbufferImg = zbufferView(zbuffer, w, h);

  ppm::write(framebuffer, "framebuffer.ppm");
  ppm::write(zbufferImg, "zbuffer.ppm");
  return 0;
}

