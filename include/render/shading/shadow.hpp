#pragma once

#include "math/mat.hpp"
#include "math/vec.hpp"
#include "scene.hpp"

#include <vector>

void renderShadowMap(const Scene &scene, const Mat4 &lightSpaceMat, std::vector<float> &shadowMap, int w, int h);

float calcShadow(const Vec3 &worldPos, const Mat4 &lightSpaceMat, const std::vector<float> &shadowMap, int w, int h);

