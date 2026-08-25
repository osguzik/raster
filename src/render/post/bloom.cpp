#include "render/post/bloom.hpp"

#include "image.hpp"

#include <cmath>
#include <vector>

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

std::vector<float> gaussianKernel(int radius) {
  std::vector<float> kernel(radius * 2 + 1);
  float sigma = radius / 2.0f;
  float sum = 0.0f;

  for (int i = -radius; i <= radius; i++) {
    float weight = std::exp(-(i * i) / (2.0f * sigma * sigma));
    kernel[i + radius] = weight;
    sum += weight;
  }

  for (float &weight : kernel) {
    weight /= sum;
  }
  return kernel;
}

Image gaussianBlur(const Image &img, int radius) {
  std::vector<float> kernel = gaussianKernel(radius);
}

}

void applyBloom(Image &img, float threshold, int blurRadius) {
  Image bright = extractLight(img, threshold);
  Image blurred = gaussianBlur(bright, blurRadius);
}

