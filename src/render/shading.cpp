#include "render/shading.hpp"

#include "image.hpp"
#include "math/vec.hpp"
#include "render/material.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

Color blinnPhong(const Vec3 &normal, const Lighting &lighting, const Material &material) {
  float diffuse = std::max(dot(normal, lighting.lightDir), 0.0f);
  float specular = 0.0f;

  if (diffuse > 0.0f) {
    Vec3 halfway = normalize(lighting.lightDir + lighting.viewDir);
    specular = material.specularStrength * std::pow(std::max(dot(normal, halfway), 0.0f), material.shininess);
  }

  float intensity = std::min(lighting.ambient + diffuse + specular, 1.0f);
  return Color{
    static_cast<std::uint8_t>(material.baseColor.r * intensity),
    static_cast<std::uint8_t>(material.baseColor.g * intensity),
    static_cast<std::uint8_t>(material.baseColor.b * intensity)
  };
}

