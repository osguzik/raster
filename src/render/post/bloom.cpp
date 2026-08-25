#include "render/post/bloom.hpp"
#include "image.hpp"

namespace {

Image extractLight(const Image &img, float threshold) {
  Image result(img.w, img.h, Color{0, 0, 0});
  for (int y = 0; y < img.h; y++) {
    for (int x = 0; x < img.w; x++) {
      Color color = img.get(x, y);
      float brightness = (color.r + color.g + color.b) / (3.0f * 255.0f);
      if (brightness > threshold) {
        result.set(x, y, color);
      }
    }
  }
  return result;
}

  // Image gaussianBlur(const Image &img) {}

}

void applyBloom(Image &img, float threshold) {
  Image bright = extractLight(img, threshold);
  // Image blurred = gaussianBlur(bright);
}

