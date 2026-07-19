#include "render/shading.hpp"

#include "image.hpp"

#include <algorithm>
#include <cstdint>

Color shade(const Color &baseColor, float ambient) {
  float intensity = std::min(ambient, 1.0f);
  return Color{
    static_cast<std::uint8_t>(baseColor.r * intensity),
    static_cast<std::uint8_t>(baseColor.g * intensity),
    static_cast<std::uint8_t>(baseColor.b * intensity)
  };
}

