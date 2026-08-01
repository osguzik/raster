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

inline Mat4 scale(Vec3 s) {
  return Mat4{{
    {s.x, 0, 0, 0},
    {0, s.y, 0, 0},
    {0, 0, s.z, 0},
    {0, 0, 0, 1}
  }};
}

struct Transform {
  Vec3 position = {0.0f, 0.0f, 0.0f};
  Vec3 rotation = {0.0f, 0.0f, 0.0f};
  Vec3 scale = {1.0f, 1.0f, 1.0f};
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

inline Mat4 viewport(int x, int y, int w, int h) {
  return Mat4{{
  {w / 2.0f, 0, 0, x + w / 2.0f},
  {0, -h / 2.0f, 0, y + h / 2.0f},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
  }};
}

inline Mat3 upperLeft3(const Mat4 &m) {
  return Mat3{{
  {m.rows[0].x, m.rows[0].y, m.rows[0].z},
  {m.rows[1].x, m.rows[1].y, m.rows[1].z},
  {m.rows[2].x, m.rows[2].y, m.rows[2].z}
  }};
}

inline Mat3 transpose(const Mat3 &m) {
  return Mat3{{
    {m.rows[0].x, m.rows[1].x, m.rows[2].x},
    {m.rows[0].y, m.rows[1].y, m.rows[2].y},
    {m.rows[0].z, m.rows[1].z, m.rows[2].z}
  }};
}

inline Mat3 inverse(const Mat3 &m) {
  float a = m.rows[0].x, b = m.rows[0].y, c = m.rows[0].z;
  float d = m.rows[1].x, e = m.rows[1].y, f = m.rows[1].z;
  float g = m.rows[2].x, h = m.rows[2].y, i = m.rows[2].z;

  float A = e * i - f * h;
  float B = f * g - d * i;
  float C = d * h - e * g;
  float D = c * h - b * i;
  float E = a * i - c * g;
  float F = b * g - a * h;
  float G = b * f - c * e;
  float H = c * d - a * f;
  float I = a * e - b * d;

  float det = a * A + b * B + c * C;
  float invDet = 1.0f / det;

  return Mat3{{
  {A * invDet, D * invDet, G * invDet},
  {B * invDet, E * invDet, H * invDet},
  {C * invDet, F * invDet, I * invDet}
  }};
}

