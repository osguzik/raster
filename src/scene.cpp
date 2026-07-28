#include "scene.hpp"

#include "camera.hpp"
#include "image.hpp"
#include "math/mat.hpp"
#include "math/transform.hpp"
#include "model.hpp"
#include "render/raster/triangle.hpp"
#include "render/raster/vertex.hpp"
#include "render/shading.hpp"
#include "render/vertex_transform.hpp"
#include <utility>
#include <vector>

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

    Vec3 worldPos0 = transformPoint(modelMat(obj.transform), modelPos0);
    Vec3 worldPos1 = transformPoint(modelMat(obj.transform), modelPos1);
    Vec3 worldPos2 = transformPoint(modelMat(obj.transform), modelPos2);

    Vec3 centroid = (worldPos0 + worldPos1 + worldPos2) / 3.0f;
    lighting.viewDir = normalize(camera.eye - centroid);

    ClipVertex clip0 = toClip(modelPos0, model.normals[model.normalFaces[i * 3]], model.uvs[model.uvFaces[i * 3]], mvp, normalMat);
    ClipVertex clip1 = toClip(modelPos1, model.normals[model.normalFaces[i * 3 + 1]], model.uvs[model.uvFaces[i * 3 + 1]], mvp, normalMat);
    ClipVertex clip2 = toClip(modelPos2, model.normals[model.normalFaces[i * 3 + 2]], model.uvs[model.uvFaces[i * 3 + 2]], mvp, normalMat);

    ScreenVertex v0 = toScreen(clip0, vp);
    ScreenVertex v1 = toScreen(clip1, vp);
    ScreenVertex v2 = toScreen(clip2, vp);

    triangle(v0, v1, v2, framebuffer, zbuffer, lighting, obj.material);
  }
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

