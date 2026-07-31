#include "render/material.hpp"
#include "scene.hpp"

#include <numbers>
#include <string>

Scene buildDragonsScene(int w, int h) {
  Scene scene;
  scene.camera = defaultCamera(/*ar=*/static_cast<float>(w) / h);
  scene.lighting = defaultLighting();

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

  return scene;
}

