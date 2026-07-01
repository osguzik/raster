#include "raster/line.hpp"

#include <cmath>
#include <utility>

void line(int x0, int y0, int x1, int y1, Image &img, const Color &color) {
  bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
  if(steep) {
    std::swap(x0, y0);
    std::swap(x1, y1);
  }

  if(x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = std::abs(y1 - y0);

  int err = 2*dy - dx;
  int ystep = (y0 < y1) ? 1 : -1;
  int y = y0;

  for(int x = x0; x <= x1; x++) {
    if(steep) {
      img.set(y, x, color);
    } else {
      img.set(x, y, color);
    }

    if(err >= 0) {
      y += ystep;
      err -= 2*dx;
    }

    err += 2*dy;
  }
}

