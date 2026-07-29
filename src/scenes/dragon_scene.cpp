#include "scene.hpp"

#include <numbers>

Scene buildDragonScene(int w, int h) {
  Scene scene;
  scene.camera = defaultCamera(/*ar=*/static_cast<float>(w) / h);
  scene.lighting = defaultLighting();

  Transform transform;
  transform.position = {0.0f, -0.5f, -2.0f};
  transform.rotation.y = std::numbers::pi_v<float> * 2.0f;
  transform.scale = {0.2f, 0.2f, 0.2f};

  Material material;
  material.baseColor = {210, 30, 70};
  material.specularStrength = 4.0f;
  material.shininess = 128.0f;

  addObj(scene, "../assets/dragon.obj", transform, material, /*shouldRecalcNormals=*/true);

  return scene;
}

