#include "camera.hpp"
#include "image.hpp"
#include "math/mat.hpp"
#include "math/transform.hpp"
#include "math/vec.hpp"
#include "model.hpp"
#include "render/raster/triangle.hpp"
#include "render/raster/vertex.hpp"
#include "render/vertex_transform.hpp"
#include "render/zbuffer_view.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <numbers>
#include <vector>

constexpr int w = 1024;
constexpr int h = 1024;

constexpr Color white = {255, 255, 255};

Vec3 transformPoint(const Mat4 &m, const Vec3 &point) {
  Vec4 result = m * Vec4{point.x, point.y, point.z, 1.0f};
  return {result.x, result.y, result.z};
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

  Color baseColor = white;
  float ambient = 0.1f;
  Vec3 lightDir = normalize(Vec3{0.0f, 0.0f, 1.0f});
  float shininess = 32.0f;

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 modelPos0 = model.verts[model.faces[i * 3]];
    Vec3 modelPos1 = model.verts[model.faces[i * 3 + 1]];
    Vec3 modelPos2 = model.verts[model.faces[i * 3 + 2]];

    Vec3 worldPos0 = transformPoint(modelMat(t), modelPos0);
    Vec3 worldPos1 = transformPoint(modelMat(t), modelPos1);
    Vec3 worldPos2 = transformPoint(modelMat(t), modelPos2);

    Vec3 normal = normalize(cross(worldPos1 - worldPos0, worldPos2 - worldPos0));

    Vec3 centroid = (worldPos0 + worldPos1 + worldPos2) / 3.0f;
    Vec3 viewDir = normalize(camera.eye - centroid);

    ClipVertex clip0 = toClip(modelPos0, mvp);
    ClipVertex clip1 = toClip(modelPos1, mvp);
    ClipVertex clip2 = toClip(modelPos2, mvp);

    ScreenVertex v0 = toScreen(clip0, vp);
    ScreenVertex v1 = toScreen(clip1, vp);
    ScreenVertex v2 = toScreen(clip2, vp);

    triangle(v0, v1, v2, framebuffer, zbuffer, normal, lightDir, viewDir, baseColor, ambient, shininess);
  }

  Image zbufferImg = zbufferView(zbuffer, w, h);

  ppm::write(framebuffer, "framebuffer.ppm");
  ppm::write(zbufferImg, "zbuffer.ppm");
  return 0;
}

