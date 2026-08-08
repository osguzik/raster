#pragma once

#include "mat.hpp"
#include "vec.hpp"

#include <cmath>

inline Mat4 translate(Vec3 t) {
  return Mat4{{
    {1.0f, 0.0f, 0.0f, t.x},
    {0.0f, 1.0f, 0.0f, t.y},
    {0.0f, 0.0f, 1.0f, t.z},
    {0.0f, 0.0f, 0.0f, 1.0f}
  }};
}

inline Mat4 rotateX(float rad) {
  return Mat4{{
  {1.0f, 0.0f, 0.0f, 0.0f},
  {0.0f, std::cos(rad), -std::sin(rad), 0.0f},
  {0.0f, std::sin(rad), std::cos(rad), 0.0f},
  {0.0f, 0.0f, 0.0f, 1.0f}
  }};
}

inline Mat4 rotateY(float rad) {
  return Mat4{{
    {std::cos(rad), 0.0f, std::sin(rad), 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {-std::sin(rad), 0.0f, std::cos(rad), 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
  }};
}

inline Mat4 rotateZ(float rad) {
  return Mat4{{
  {std::cos(rad), -std::sin(rad), 0.0f, 0.0f},
  {std::sin(rad), std::cos(rad), 0.0f, 0.0f},
  {0.0f, 0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 0.0f, 1.0f}
  }};
}

inline Mat4 scale(Vec3 s) {
  return Mat4{{
    {s.x, 0.0f, 0.0f, 0.0f},
    {0.0f, s.y, 0.0f, 0.0f},
    {0.0f, 0.0f, s.z, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
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
    {0.0f, 0.0f, 0.0f, 1.0f}
  }};
}

// right-handed coordinate system
inline Mat4 perspective(float fovY, float ar, float near, float far) {
  const float tanHalfFovY = std::tan(fovY / 2.0f);
  return Mat4{{
    {1.0f / (ar * tanHalfFovY), 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f / tanHalfFovY, 0.0f, 0.0f},
    {0.0f, 0.0f, (near + far) / (near - far), (2.0f * near * far) / (near - far)},
    {0.0f, 0.0f, -1.0f, 0.0f}
  }};
}

inline Mat4 viewport(int x, int y, int w, int h) {
  return Mat4{{
  {w / 2.0f, 0.0f, 0.0f, x + w / 2.0f},
  {0.0f, -h / 2.0f, 0.0f, y + h / 2.0f},
  {0.0f, 0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 0.0f, 1.0f}
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

inline Mat4 ortho(float left, float right, float bottom, float top, float near, float far) {
  return Mat4{{
  {2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left)},
  {0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom)},
  {0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near)},
  {0.0f, 0.0f, 0.0f, 1.0f}
  }};
}

inline Mat4 lightSpaceMat(const Vec3 &lightDir, const Vec3 &sceneCenter, float distance, float coverage, float near, float far) {
  Mat4 lightProjection = ortho(-coverage, coverage, -coverage, coverage, near, far);
  Vec3 lightEye = sceneCenter - lightDir * distance;
  Mat4 lightView = lookAt(lightEye, sceneCenter, {0.0f, 1.0f, 0.0f});
  return lightProjection * lightView;
}

