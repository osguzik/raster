#pragma once

#include "camera.hpp"
#include "image.hpp"
#include "math/mat.hpp"
#include "math/transform.hpp"
#include "model.hpp"
#include "render/material.hpp"
#include "render/shading/lighting.hpp"

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
  Mat4 lightSpaceMat;
};

Camera defaultCamera(float ar);

Lighting defaultLighting();

SceneObj addObj(Scene &scene, const std::string &filename, const Transform &transform, const Material &material, bool shouldRecalcNormals);

void renderScene(const Scene &scene, Image &framebuffer, std::vector<float> &zbuffer, int w, int h, const std::vector<float> &shadowMap);

Scene buildDragonsScene(int w, int h);

