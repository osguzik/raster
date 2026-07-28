#include "scene.hpp"

#include "camera.hpp"
#include "image.hpp"
#include "render/shading.hpp"
#include <vector>

SceneObj addObj(Scene &scene, const Transform &transform, const Material &material) {}

void renderObj(const SceneObj &obj, const Camera &camera, Lighting &Lighting, Image &framebuffer, std::vector<float> &zbuffer, int w, int h) {}

void renderScene(const Scene &scene, Image &framebuffer, std::vector<float> &zbuffer, int w, int h) {
  for (const SceneObj &obj : scene.objects) {
    renderObj(obj, scene.camera, scene.lighting, framebuffer, zbuffer, w, h);
  }
}

