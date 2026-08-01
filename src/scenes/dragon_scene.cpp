#include "image.hpp"
#include "render/material.hpp"
#include "scene.hpp"

#include <numbers>
#include <string>

Scene buildDragonsScene(int w, int h) {
  Scene scene;

  scene.camera = defaultCamera(/*ar=*/static_cast<float>(w) / h);
  scene.camera.eye = {0.0f, 1.0f, 1.5f};
  scene.camera.at  = {0.0f, -0.3f, -2.5f};

  scene.lighting = defaultLighting();
  scene.lighting.lightDir = normalize(Vec3{0.3f, 0.2f, 0.4f});

  Transform blueDragon;
  blueDragon.position = {-1.25f, -0.5f, -2.0f};
  blueDragon.rotation.y = std::numbers::pi_v<float>;
  blueDragon.scale = {0.2f, 0.2f, 0.2f};

  Material blueMaterial;
  blueMaterial.baseColor = {15, 80, 185};
  blueMaterial.specularStrength = 4.0f;
  blueMaterial.shininess = 128.0f;

  addObj(scene, std::string(ASSETS_DIR) + "dragon.obj", blueDragon, blueMaterial,
         /*shouldRecalcNormals=*/false); // flat shading

  Transform redDragon;
  redDragon.position = {1.25f, -0.5f, -2.0f};
  redDragon.rotation.y = std::numbers::pi_v<float> * 2.0f;
  redDragon.scale = {0.2f, 0.2f, 0.2f};

  Material redMaterial = blueMaterial;
  redMaterial.baseColor = {210, 30, 70};

  addObj(scene, std::string(ASSETS_DIR) + "dragon.obj", redDragon, redMaterial,
         /*shouldRecalcNormals=*/true); // smooth shading

  static Image checker = checkerTexture(64, 2);

  Material planeMaterial;
  planeMaterial.baseColor = {200, 200, 200};
  planeMaterial.specularStrength = 0.5f;
  planeMaterial.shininess = 16.0f;
  planeMaterial.texture = &checker;

  Transform plane;
  plane.position = {0.0f, -1.5f, -3.0f};
  plane.scale = {8.0f, 1.0f, 4.0f};

  addObj(scene, std::string(ASSETS_DIR) + "plane.obj", plane, planeMaterial,
         /*shouldRecalcNormals=*/false);

  return scene;
}

