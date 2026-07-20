#include "render/shading.hpp"

#include "image.hpp"
#include "math/vec.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

Color shade(const Vec3 &normal, const Vec3 &lightDir, const Vec3 &viewDir, const Color &baseColor, float ambient, float shininess) {
  float diffuse = std::max(dot(normal, lightDir), 0.0f);

  Vec3 halfway = normalize(lightDir + viewDir);
  float specular = std::pow(std::max(dot(normal, halfway), 0.0f), shininess);

  float intensity = std::min(ambient + diffuse + specular, 1.0f);
  return Color{
    static_cast<std::uint8_t>(baseColor.r * intensity),
    static_cast<std::uint8_t>(baseColor.g * intensity),
    static_cast<std::uint8_t>(baseColor.b * intensity)
  };
}

