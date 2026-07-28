#pragma once

#include "camera.hpp"
#include "image.hpp"
#include "math/transform.hpp"
#include "model.hpp"
#include "render/material.hpp"
#include "render/shading.hpp"
#include <vector>

struct SceneObj {
  Model model;
  Transform transform;
  Material material;
};

struct Scene {
  Camera camera;
  Lighting lighting;
  std::vector<SceneObj> objects;
};

SceneObj addObj(Scene &scene, const Transform &transform, const Material &material);

void renderObj(const SceneObj &obj, const Camera &camera, Lighting &Lighting, Image &framebuffer, std::vector<float> &zbuffer, int w, int h);

void renderScene(const Scene &scene, Image &framebuffer, std::vector<float> &zbuffer, int w, int h);

