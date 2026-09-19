#include "render/rasterize/triangle.hpp"

#include "image.hpp"
#include "math/mat.hpp"
#include "math/vec.hpp"
#include "render/material.hpp"
#include "render/rasterize/vertex.hpp"
#include "render/shading/lighting.hpp"
#include "render/shading/shadow.hpp"

#include <algorithm>
#include <vector>

void triangle(const ScreenVertex &v0, const ScreenVertex &v1,
              const ScreenVertex &v2, Image &img, std::vector<float> &zbuffer,
              Lighting lighting, const Material &material, const Vec3 &eye,
              const Mat4 &lightSpaceMat, const std::vector<float> &shadowMap) {
  rasterize(v0, v1, v2, img.w, img.h,
            [&](int x, int y, float w0, float w1, float w2) {
              float z = w0 * v0.pos.z + w1 * v1.pos.z + w2 * v2.pos.z;
              int idx = y * img.w + x;
              if (z >= zbuffer[idx]) {
                return;
              }
              zbuffer[idx] = z;

              float invW = w0 * v0.invW + w1 * v1.invW + w2 * v2.invW;

              if (material.useVertexColor) {
                Vec3 color =
                    (v0.color * v0.invW * w0 + v1.color * v1.invW * w1 +
                     v2.color * v2.invW * w2) /
                    invW;
                img.set(x, y,
                        Color{static_cast<unsigned char>(
                                  std::clamp(color.x, 0.0f, 1.0f) * 255.0f),
                              static_cast<unsigned char>(
                                  std::clamp(color.y, 0.0f, 1.0f) * 255.0f),
                              static_cast<unsigned char>(
                                  std::clamp(color.z, 0.0f, 1.0f) * 255.0f)});
                return;
              }

              Vec2 uv = (v0.uv * v0.invW * w0 + v1.uv * v1.invW * w1 +
                         v2.uv * v2.invW * w2) /
                        invW;
              Vec3 normal = normalize((v0.normal * v0.invW * w0 +
                                       v1.normal * v1.invW * w1 +
                                       v2.normal * v2.invW * w2) /
                                      invW);
              Vec3 worldPos =
                  (v0.worldPos * v0.invW * w0 + v1.worldPos * v1.invW * w1 +
                   v2.worldPos * v2.invW * w2) /
                  invW;
              lighting.viewDir = normalize(eye - worldPos);
              float shadow =
                  calcShadow(worldPos, lightSpaceMat, shadowMap, img.w, img.h);
              img.set(x, y, blinnPhong(normal, uv, lighting, material, shadow));
            });
}
