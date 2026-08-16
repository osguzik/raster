#include "render/shading/shadow.hpp"

#include "math/vec.hpp"

#include <algorithm>

void renderShadowMap(const Scene &scene, const Mat4 &lightSpaceMat, std::vector<float> &shadowMap, int w, int h) {}

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

  int x = std::clamp(static_cast<int>(u * w), 0, w - 1);
  int y = std::clamp(static_cast<int>(v * h), 0, h - 1);

  float closestDepth = shadowMap[y * w + x];
  float bias = 0.01f;
  return (currDepth - bias > closestDepth) ? 1.0f : 0.0f;
}

