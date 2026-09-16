#include "image.hpp"
#include "render/material.hpp"
#include "scene.hpp"

#include <numbers>
#include <string>

Scene buildDragonsScene(int w, int h, float t) {
  Scene scene;

  scene.camera = defaultCamera(/*ar=*/static_cast<float>(w) / h);
  scene.camera.eye = {0.0f, 1.0f, 1.5f};
  scene.camera.at  = {0.0f, -0.3f, -2.5f};

  float phase = t * 2.0f * std::numbers::pi_v<float>;

  scene.lighting = defaultLighting();
  scene.lighting.lightDir = normalize(Vec3{0.0f, 0.2f, 0.4f});

  scene.useBloom = true;

  Transform blueDragon;
  blueDragon.position = {-1.25f, -0.5f, -2.0f};
  blueDragon.rotation.y = std::numbers::pi_v<float>;
  blueDragon.scale = {0.2f, 0.2f, 0.2f};

  Material blueMaterial;
  blueMaterial.baseColor = {15, 80, 185};
  blueMaterial.specularStrength = 4.0f;
  blueMaterial.shininess = 128.0f;

  addObj(scene, std::string(ASSETS_DIR) + "/obj/dragon.obj", blueDragon, blueMaterial,
         /*shouldRecalcNormals=*/false); // flat shading

  Transform redDragon;
  redDragon.position = {1.25f, -0.5f, -2.0f};
  redDragon.rotation.y = std::numbers::pi_v<float> * 2.0f;
  redDragon.scale = {0.2f, 0.2f, 0.2f};

  Material redMaterial = blueMaterial;
  redMaterial.baseColor = {210, 30, 70};

  addObj(scene, std::string(ASSETS_DIR) + "/obj/dragon.obj", redDragon, redMaterial,
         /*shouldRecalcNormals=*/true); // smooth shading

  static Image checker;
  ppm::read(std::string(ASSETS_DIR) + "/textures/checker.ppm", checker);

  Material planeMaterial;
  planeMaterial.baseColor = {200, 200, 200};
  planeMaterial.specularStrength = 0.5f;
  planeMaterial.shininess = 16.0f;
  planeMaterial.texture = &checker;

  Transform plane;
  plane.position = {0.0f, -0.5f, -1.0f};
  plane.scale = {8.0f, 1.0f, 4.0f};

  addObj(scene, std::string(ASSETS_DIR) + "/obj/plane.obj", plane, planeMaterial,
         /*shouldRecalcNormals=*/false);

  Vec3 sceneCenter = {0.0f, -0.5f, -1.5f};
  scene.lightSpaceMat = lightSpaceMat(scene.lighting.lightDir, sceneCenter,
                                      /*distance=*/6.0f, /*coverage=*/8.0f, /*near=*/0.1f, /*far=*/12.0f);

  return scene;
}

