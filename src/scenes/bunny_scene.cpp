#include "math/transform.hpp"
#include "render/material.hpp"
#include "scene.hpp"

#include <numbers>
#include <string>

Scene buildBunnyScene(int w, int h) {
  Scene scene;

  scene.camera = defaultCamera(static_cast<float>(w) / h);
  scene.camera.eye = {0.0f, 0.0f, 2.0f};
  scene.camera.at = {0.0f, 0.0f, -2.0f};

  scene.lighting = defaultLighting();

  Transform bunny;
  bunny.position = {0.2f, -0.2f, -3.0f};
  bunny.rotation.y = std::numbers::pi_v<float> * 2.0f;
  bunny.scale = {1.0f, 1.0f, 1.0f};

  Material bunnyMaterial;
  bunnyMaterial.useVertexColor = true;

  addObj(scene, std::string(ASSETS_DIR) + "/obj/bunny.obj", bunny, bunnyMaterial,
         /*shouldRecalcNormals=*/true);

  scene.lightSpaceMat = lightSpaceMat(scene.lighting.lightDir, bunny.position,
                                      /*distance=*/6.0f, /*coverage=*/4.0f, /*near=*/0.1f, /*far=*/12.0f);

  return scene;
}

