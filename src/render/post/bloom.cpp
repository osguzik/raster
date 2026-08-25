#include "render/post/bloom.hpp"

#include "image.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
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

  Image horizontal(img.w, img.h, Color{0, 0, 0});
  for (int y = 0; y < img.h; y++) {
    for (int x = 0; x < img.w; x++) {
      float sumR = 0.0f;
      float sumG = 0.0f;
      float sumB = 0.0f;

      for (int offset = -radius; offset <= radius; offset++) {
        int sampleX = std::clamp(x + offset, 0, img.w - 1);
        Color sample = img.get(sampleX, y);
        float weight = kernel[offset + radius];

        sumR += sample.r * weight;
        sumG += sample.g * weight;
        sumB += sample.b * weight;
      }

      horizontal.set(x, y, Color{
        static_cast<std::uint8_t>(std::clamp(sumR, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(sumG, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(sumB, 0.0f, 255.0f))
      });
    }
  }

  Image blurred(img.w, img.h, Color{0, 0, 0});
  for (int y = 0; y < img.h; y++) {
    for (int x = 0; x < img.w; x++) {
      float sumR = 0.0f;
      float sumG = 0.0f;
      float sumB = 0.0f;

      for (int offset = -radius; offset <= radius; offset++) {
        int sampleY = std::clamp(y + offset, 0, img.h - 1);
        Color sample = img.get(x, sampleY);
        float weight = kernel[offset + radius];

        sumR += sample.r * weight;
        sumG += sample.g * weight;
        sumB += sample.b * weight;
      }

      blurred.set(x, y, Color{
        static_cast<std::uint8_t>(std::clamp(sumR, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(sumG, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(sumB, 0.0f, 255.0f)),
      });
    }
  }

  return blurred;
}

}

void applyBloom(Image &img, float threshold, int blurRadius, float intensity) {
  Image bright = extractLight(img, threshold);
  Image blurred = gaussianBlur(bright, blurRadius);

  for (int y = 0; y < img.h; y++) {
    for (int x = 0; x < img.w; x++) {
      Color base = img.get(x, y);
      Color glow = blurred.get(x, y);
      Color combined = {
        static_cast<std::uint8_t>(std::clamp(base.r + glow.r * intensity, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(base.g + glow.g * intensity, 0.0f, 255.0f)),
        static_cast<std::uint8_t>(std::clamp(base.b + glow.b * intensity, 0.0f, 255.0f)),
      };
      img.set(x, y, combined);
    }
  }
}

