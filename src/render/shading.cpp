#include "render/shading.hpp"

#include "image.hpp"

#include <algorithm>
#include <cstdint>

Color shade(const Vec3 &normal, const Vec3 &light, const Color &baseColor, float ambient) {
  float diffuse = std::max(dot(normal, light), 0.0f);
  float intensity = std::min(ambient + diffuse, 1.0f);
  return Color{
    static_cast<std::uint8_t>(baseColor.r * intensity),
    static_cast<std::uint8_t>(baseColor.g * intensity),
    static_cast<std::uint8_t>(baseColor.b * intensity)
  };
}

