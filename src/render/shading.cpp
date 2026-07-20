#include "render/shading.hpp"

#include "image.hpp"
#include "math/vec.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

Color shade(const Vec3 &normal, const Lighting &lighting, const Color &baseColor) {
  float diffuse = std::max(dot(normal, lighting.lightDir), 0.0f);

  Vec3 halfway = normalize(lighting.lightDir + lighting.viewDir);
  float specular = std::pow(std::max(dot(normal, halfway), 0.0f), lighting.shininess);

  float intensity = std::min(lighting.ambient + diffuse + specular, 1.0f);
  return Color{
    static_cast<std::uint8_t>(baseColor.r * intensity),
    static_cast<std::uint8_t>(baseColor.g * intensity),
    static_cast<std::uint8_t>(baseColor.b * intensity)
  };
}

