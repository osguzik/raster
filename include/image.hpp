#pragma once

#include <cstdint>
#include <vector>

struct Color {
  std::uint8_t r, g, b;
};

struct Image {
  int w = 0, h = 0;
  std::vector<std::uint8_t> pixels;

  Image() = default;
  Image(int w, int h, Color fill = {});

  Color get(int x, int y) const;
  void set(int x, int y, const Color &color);
};

