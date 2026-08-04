#include "scene.hpp"

#include "camera.hpp"
#include "image.hpp"
#include "math/mat.hpp"
#include "math/transform.hpp"
#include "model.hpp"
#include "render/clipping.hpp"
#include "render/raster/triangle.hpp"
#include "render/raster/vertex.hpp"
#include "render/lighting.hpp"
#include "render/vertex_transform.hpp"

#include <numbers>
#include <utility>
#include <vector>

namespace {

Vec3 transformPoint(const Mat4 &m, const Vec3 &point) {
  Vec4 result = m * Vec4{point.x, point.y, point.z, 1.0f};
  return {result.x, result.y, result.z};
}

void renderObj(const SceneObj &obj, const Camera &camera, Lighting lighting, Image &framebuffer, std::vector<float> &zbuffer, int w, int h) {
  const Model &model = obj.model;

  Mat4 modelTransform = modelMat(obj.transform);
  Mat4 mvp = projectionMat(camera) * viewMat(camera) * modelTransform;
  Mat4 vp = viewport(0, 0, w, h);
  Mat3 normalMat = transpose(inverse(upperLeft3(modelTransform)));

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 modelPos0 = model.verts[model.faces[i * 3]];
    Vec3 modelPos1 = model.verts[model.faces[i * 3 + 1]];
    Vec3 modelPos2 = model.verts[model.faces[i * 3 + 2]];

    Vec3 worldPos0 = transformPoint(modelTransform, modelPos0);
    Vec3 worldPos1 = transformPoint(modelTransform, modelPos1);
    Vec3 worldPos2 = transformPoint(modelTransform, modelPos2);

    ClipVertex clip0 = toClip(modelPos0, worldPos0, model.normals[model.normalFaces[i * 3]], model.uvs[model.uvFaces[i * 3]], mvp, normalMat);
    ClipVertex clip1 = toClip(modelPos1, worldPos1, model.normals[model.normalFaces[i * 3 + 1]], model.uvs[model.uvFaces[i * 3 + 1]], mvp, normalMat);
    ClipVertex clip2 = toClip(modelPos2, worldPos2, model.normals[model.normalFaces[i * 3 + 2]], model.uvs[model.uvFaces[i * 3 + 2]], mvp, normalMat);

    ClipVertex clippedTriangles[2][3];
    int triangleCount = clipNearPlane(clip0, clip1, clip2, camera.near, clippedTriangles);
    for (int j = 0; j < triangleCount; j++) {
      ScreenVertex sv0 = toScreen(clippedTriangles[j][0], vp);
      ScreenVertex sv1 = toScreen(clippedTriangles[j][1], vp);
      ScreenVertex sv2 = toScreen(clippedTriangles[j][2], vp);
      triangle(sv0, sv1, sv2, framebuffer, zbuffer, lighting, obj.material, camera.eye);
    }
  }
}

}

Camera defaultCamera(float ar) {
  return Camera {
    .eye = {0.0f, 0.0f, 1.0f},
    .at = {0.0f, 0.0f, -2.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovY = std::numbers::pi_v<float> / 4.0f,
    .ar = ar,
    .near = 0.1f,
    .far = 10.0f
  };
}

Lighting defaultLighting() {
  Lighting lighting;
  lighting.lightDir = normalize(Vec3{0.0f, 0.0f, 1.0f});
  lighting.ambient = 0.1f;
  lighting.lightColor = {255, 255, 255};
  return lighting;
}

SceneObj addObj(Scene &scene, const std::string &filename, const Transform &transform, const Material &material, bool shouldRecalcNormals) {
  Model model = loadModel(filename, shouldRecalcNormals);
  scene.objects.push_back({std::move(model), transform, material});
  return scene.objects.back();
}

void renderScene(const Scene &scene, Image &framebuffer, std::vector<float> &zbuffer, int w, int h) {
  for (const SceneObj &obj : scene.objects) {
    renderObj(obj, scene.camera, scene.lighting, framebuffer, zbuffer, w, h);
  }
}

