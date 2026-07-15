#pragma once

#include "mat.hpp"
#include "vec.hpp"

#include <cmath>

inline Mat4 translate(Vec3 t) {
  return Mat4{{
    {1, 0, 0, t.x},
    {0, 1, 0, t.y},
    {0, 0, 1, t.z},
    {0, 0, 0, 1}
  }};
}

inline Mat4 scale(Vec3 s) {
  return Mat4{{
    {s.x, 0, 0, 0},
    {0, s.y, 0, 0},
    {0, 0, s.z, 0},
    {0, 0, 0, 1}
  }};
}

inline Mat4 rotateX(float rad) {
  return Mat4{{
  {1, 0, 0, 0},
  {0, std::cos(rad), -std::sin(rad), 0},
  {0, std::sin(rad), std::cos(rad), 0},
  {0, 0, 0, 1}
  }};
}

inline Mat4 rotateY(float rad) {
  return Mat4{{
    {std::cos(rad), 0, std::sin(rad), 0},
    {0, 1, 0, 0},
    {-std::sin(rad), 0, std::cos(rad), 0},
    {0, 0, 0, 1}
  }};
}

inline Mat4 rotateZ(float rad) {
  return Mat4{{
  {std::cos(rad), -std::sin(rad), 0, 0},
  {std::sin(rad), std::cos(rad), 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
  }};
}

struct Transform {
  Vec3 position = {0, 0, 0};
  Vec3 rotation = {0, 0, 0};
  Vec3 scale = {1, 1, 1};
};

inline Mat4 modelMat(const Transform &t) {
  const Mat4 rotationMat = rotateX(t.rotation.x) * rotateY(t.rotation.y) * rotateZ(t.rotation.z);
  return translate(t.position) * rotationMat * scale(t.scale);
}

inline Mat4 lookAt(const Vec3 &eye, const Vec3 &at, const Vec3 &up) {
  Vec3 n = normalize(eye - at);
  Vec3 r = normalize(cross(up, n));
  Vec3 u = cross(n, r);
  return Mat4{{
    {r.x, r.y, r.z, -dot(r, eye)},
    {u.x, u.y, u.z, -dot(u, eye)},
    {n.x, n.y, n.z, -dot(n, eye)},
    {0, 0, 0, 1}
  }};
}

// right-handed coordinate system
inline Mat4 perspective(float fovY, float ar, float near, float far) {
  const float tanHalfFovY = std::tan(fovY / 2);
  return Mat4{{
    {1 / (ar * tanHalfFovY), 0, 0, 0},
    {0, 1 / tanHalfFovY, 0, 0},
    {0, 0, (near + far) / (near - far), (2 * near * far) / (near - far)},
    {0, 0, -1, 0}
  }};
}

