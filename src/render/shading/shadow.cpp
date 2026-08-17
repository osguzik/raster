#include "render/shading/shadow.hpp"

#include "math/mat.hpp"
#include "math/transform.hpp"
#include "math/vec.hpp"
#include "model.hpp"
#include "render/clipping.hpp"
#include "render/raster/triangle.hpp"
#include "render/raster/vertex.hpp"
#include "render/vertex_transform.hpp"
#include "scene.hpp"

#include <algorithm>
#include <limits>
#include <vector>

namespace {

void renderObjShadow(const SceneObj &obj, const Mat4 &lightSpaceMat, float near, std::vector<float> &shadowMap, int w, int h) {
  const Model &model = obj.model;
  Mat4 modelTransform = modelMat(obj.transform);
  Mat4 mvp = lightSpaceMat * modelTransform;
  Mat4 vp = viewport(0, 0, w, h);

  for (int i = 0; i < static_cast<int>(model.faces.size() / 3); i++) {
    Vec3 modelPos0 = model.verts[model.faces[i * 3]];
    Vec3 modelPos1 = model.verts[model.faces[i * 3 + 1]];
    Vec3 modelPos2 = model.verts[model.faces[i * 3 + 2]];

    Vec4 clip0 = mvp * Vec4{modelPos0.x, modelPos0.y, modelPos0.z, 1.0f};
    Vec4 clip1 = mvp * Vec4{modelPos1.x, modelPos1.y, modelPos1.z, 1.0f};
    Vec4 clip2 = mvp * Vec4{modelPos2.x, modelPos2.y, modelPos2.z, 1.0f};

    // depth-only
    ClipVertex cv0 = {clip0, {}, {}, {}};
    ClipVertex cv1 = {clip1, {}, {}, {}};
    ClipVertex cv2 = {clip2, {}, {}, {}};

    ClipVertex clippedTriangles[2][3];
    int triangleCount = clipNearPlane(cv0, cv1, cv2, near, clippedTriangles);

    for (int j = 0; j < triangleCount; j++) {
      ScreenVertex sv0 = toScreen(clippedTriangles[j][0], vp);
      ScreenVertex sv1 = toScreen(clippedTriangles[j][1], vp);
      ScreenVertex sv2 = toScreen(clippedTriangles[j][2], vp);

      rasterize(sv0, sv1, sv2, w, h, [&](int x, int y, float w0, float w1, float w2) {
        float z = w0 * sv0.pos.z + w1 * sv1.pos.z + w2 * sv2.pos.z;
        int idx = y * w + x;
        if (z < shadowMap[idx]) {
          shadowMap[idx] = z;
        }
      });
    }
  }
}

}

void renderShadowMap(const Scene &scene, const Mat4 &lightSpaceMat, float near, std::vector<float> &shadowMap, int w, int h) {
  std::fill(shadowMap.begin(), shadowMap.end(), std::numeric_limits<float>::max());
  for (const SceneObj &obj : scene.objects) {
    renderObjShadow(obj, lightSpaceMat, near, shadowMap, w, h);
  }
}

float calcShadow(const Vec3 &worldPos, const Mat4 &lightSpaceMat, const std::vector<float> &shadowMap, int w, int h) {
  Vec4 lightSpacePos = lightSpaceMat * Vec4{worldPos.x, worldPos.y, worldPos.z, 1.0f};
  Vec3 projCoords = {
    lightSpacePos.x / lightSpacePos.w,
    lightSpacePos.y / lightSpacePos.w,
    lightSpacePos.z / lightSpacePos.w
  };

  float u = projCoords.x * 0.5f + 0.5f;
  float v = 1.0f - (projCoords.y * 0.5f + 0.5f);
  if (u < 0.0f || u > 1.0f || v < 0.0f || v > 1.0f) {
    return 0.0f;
  }
  float currDepth = projCoords.z;
  int centerX = std::clamp(static_cast<int>(u * w), 0, w - 1);
  int centerY = std::clamp(static_cast<int>(v * h), 0, h - 1);
  float bias = 0.01f;

  float factor = 0.0f;
  int sampleCount = 0;
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      int x = std::clamp(centerX + dx, 0, w - 1);
      int y = std::clamp(centerY + dy, 0, h - 1);
      float closestDepth = shadowMap[y * w + x];
      factor += (currDepth - bias > closestDepth) ? 1.0f : 0.0f;
      sampleCount++;
    }
  }

  return factor / sampleCount;
}

