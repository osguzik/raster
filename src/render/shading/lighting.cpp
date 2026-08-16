#include "render/shading/lighting.hpp"

#include "image.hpp"
#include "math/vec.hpp"
#include "render/material.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace {

std::uint8_t combineColor(std::uint8_t baseColor, std::uint8_t lightColor, float specular, float intensity) {
  float combined = (baseColor / 255.0f) * (lightColor / 255.0f) * intensity + specular;
  combined = std::clamp(combined, 0.0f, 1.0f);
  return static_cast<std::uint8_t>(combined * 255.0f);
}

}

Color blinnPhong(const Vec3 &normal, const Vec2 &uv, const Lighting &lighting, const Material &material, float shadow) {
  float diffuse = std::max(dot(normal, lighting.lightDir), 0.0f);
  float specular = 0.0f;

  if (diffuse > 0.0f) {
    Vec3 halfway = normalize(lighting.lightDir + lighting.viewDir);
    specular = material.specularStrength * std::pow(std::max(dot(normal, halfway), 0.0f), material.shininess);
  }

  float lit = 1.0f - shadow;
  float intensity = std::min(lighting.ambient + diffuse * lit, 1.0f);
  specular *= lit;
  Color surface = material.texture ? material.texture->sample(uv.x, uv.y) : material.baseColor;
  return Color{
    combineColor(surface.r, lighting.lightColor.r, specular, intensity),
    combineColor(surface.g, lighting.lightColor.g, specular, intensity),
    combineColor(surface.b, lighting.lightColor.b, specular, intensity)
  };
}

