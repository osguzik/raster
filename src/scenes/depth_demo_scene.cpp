#include "image.hpp"
#include "math/vec.hpp"
#include "render/material.hpp"
#include "scene.hpp"

#include <cmath>
#include <numbers>
#include <string>

Scene buildDepthDemoScene(int w, int h, float t) {
  Scene scene;

  scene.camera = defaultCamera(/*ar=*/static_cast<float>(w) / h);
  scene.camera.eye = {0.0f, 0.5f, 1.5f};
  scene.camera.at  = {0.0f, -0.2f, -2.5f};

  float phase = t * 2.0f * std::numbers::pi_v<float>;

  scene.lighting = defaultLighting();
  scene.lighting.lightDir = normalize(Vec3{std::sin(phase), 0.5f, std::cos(phase)});

  scene.useBloom = true;
  scene.backgroundColor = {0, 0, 0};

  Transform diablo3Pose;
  diablo3Pose.position = {0.5f, 0.4f, -0.5f};
  diablo3Pose.rotation.y = std::numbers::pi_v<float> * 2.0f;
  diablo3Pose.scale = {0.4f, 0.4f, 0.4f};

  Material purpleMaterial;
  purpleMaterial.baseColor = {88, 28, 135};
  purpleMaterial.specularStrength = 4.0f;
  purpleMaterial.shininess = 128.0f;

  addObj(scene, std::string(ASSETS_DIR) + "/obj/diablo3_pose.obj", diablo3Pose, purpleMaterial,
         /*shouldRecalcNormals=*/false);

  Transform cube;
  cube.position = {-0.4f, 0.2f, 0.0f};
  cube.rotation.y = std::numbers::pi_v<float> / 6.0f;
  cube.scale = {0.4f, 0.4f, 0.4f};

  Material yellowMaterial = purpleMaterial;
  yellowMaterial.baseColor = {252, 252, 106};

  addObj(scene, std::string(ASSETS_DIR) + "/obj/cube.obj", cube, yellowMaterial,
         /*shouldRecalcNormals=*/false);

  static Image checker;
  ppm::read(std::string(ASSETS_DIR) + "/textures/checker.ppm", checker);

  Material planeMaterial;
  planeMaterial.baseColor = {200, 200, 200};
  planeMaterial.specularStrength = 0.5f;
  planeMaterial.shininess = 16.0f;
  planeMaterial.texture = &checker;

  Transform plane;
  plane.position = {0.0f, 0.0f, -1.0f};
  plane.scale = {8.0f, 1.0f, 4.0f};

  addObj(scene, std::string(ASSETS_DIR) + "/obj/plane.obj", plane, planeMaterial,
         /*shouldRecalcNormals=*/false);

  Vec3 sceneCenter = {0.0f, -0.5f, -1.5f};
  scene.lightSpaceMat = lightSpaceMat(scene.lighting.lightDir, sceneCenter,
                                      /*distance=*/6.0f, /*coverage=*/8.0f, /*near=*/0.1f, /*far=*/12.0f);

  return scene;
}

