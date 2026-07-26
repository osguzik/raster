#include "render/shading.hpp"

#include "image.hpp"
#include "math/vec.hpp"
#include "render/material.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

std::uint8_t shadeColor(std::uint8_t baseColor, std::uint8_t lightColor, float specular, float intensity) {
  float combined = (baseColor / 255.0f) * (lightColor / 255.0f) * intensity + specular;
  combined = std::clamp(combined, 0.0f, 1.0f);
  return static_cast<std::uint8_t>(combined * 255.0f);
}

Color blinnPhong(const Vec3 &normal, const Lighting &lighting, const Material &material) {
  float diffuse = std::max(dot(normal, lighting.lightDir), 0.0f);
  float specular = 0.0f;

  if (diffuse > 0.0f) {
    Vec3 halfway = normalize(lighting.lightDir + lighting.viewDir);
    specular = material.specularStrength * std::pow(std::max(dot(normal, halfway), 0.0f), material.shininess);
  }

  float intensity = std::min(lighting.ambient + diffuse, 1.0f);
  return Color{
    shadeColor(material.baseColor.r, lighting.lightColor.r, specular, intensity),
    shadeColor(material.baseColor.g, lighting.lightColor.g, specular, intensity),
    shadeColor(material.baseColor.b, lighting.lightColor.b, specular, intensity)
  };
}

