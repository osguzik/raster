#include "camera.hpp"
#include "image.hpp"
#include "math/mat.hpp"
#include "math/transform.hpp"
#include "math/vec.hpp"
#include "model.hpp"
#include "render/material.hpp"
#include "render/raster/triangle.hpp"
#include "render/raster/vertex.hpp"
#include "render/shading.hpp"
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
  Model model = loadModel(argv[1], true);
  std::vector<float> zbuffer(w * h, std::numeric_limits<float>::max());

  Transform t;
  t.position = {0.0f, -0.5f, -2.0f};
  t.rotation.y = std::numbers::pi_v<float> * 2.0f;
  t.scale = {0.2f, 0.2f, 0.2f};

  Camera camera{
    .eye = {0.0f, 0.0f, 1.0f},
    .at = {0.0f, 0.0f, -2.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovY = std::numbers::pi_v<float> / 4.0f,
    .ar = static_cast<float>(w) / h,
    .near = 0.1f,
    .far = 10.0f
  };

  Mat4 modelTransform = modelMat(t);
  Mat4 mvp = projectionMat(camera) * viewMat(camera) * modelTransform;
  Mat4 vp = viewport(0, 0, w, h);
  Mat3 normalMat = transpose(inverse(upperLeft3(modelTransform)));

  Lighting lighting;
  lighting.lightDir = normalize(Vec3{0.0f, 0.0f, 1.0f});
  lighting.ambient = 0.1f;
  lighting.lightColor = white;

  Material material;
  material.baseColor = {210, 30, 70};
  Image checker = checkerTexture(128, 2);
  material.texture = &checker;
  material.specularStrength = 4.0f;
  material.shininess = 128.0f;

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 modelPos0 = model.verts[model.faces[i * 3]];
    Vec3 modelPos1 = model.verts[model.faces[i * 3 + 1]];
    Vec3 modelPos2 = model.verts[model.faces[i * 3 + 2]];

    Vec3 worldPos0 = transformPoint(modelMat(t), modelPos0);
    Vec3 worldPos1 = transformPoint(modelMat(t), modelPos1);
    Vec3 worldPos2 = transformPoint(modelMat(t), modelPos2);

    Vec3 centroid = (worldPos0 + worldPos1 + worldPos2) / 3.0f;
    lighting.viewDir = normalize(camera.eye - centroid);

    ClipVertex clip0 = toClip(modelPos0, model.normals[model.normalFaces[i * 3]], model.uvs[model.uvFaces[i * 3]], mvp, normalMat);
    ClipVertex clip1 = toClip(modelPos1, model.normals[model.normalFaces[i * 3 + 1]], model.uvs[model.uvFaces[i * 3 + 1]], mvp, normalMat);
    ClipVertex clip2 = toClip(modelPos2, model.normals[model.normalFaces[i * 3 + 2]], model.uvs[model.uvFaces[i * 3 + 2]], mvp, normalMat);

    ScreenVertex v0 = toScreen(clip0, vp);
    ScreenVertex v1 = toScreen(clip1, vp);
    ScreenVertex v2 = toScreen(clip2, vp);

    triangle(v0, v1, v2, framebuffer, zbuffer, lighting, material);
  }

  Image zbufferImg = zbufferView(zbuffer, w, h);

  ppm::write(framebuffer, "framebuffer.ppm");
  ppm::write(zbufferImg, "zbuffer.ppm");
  return 0;
}

