#pragma once

#include <cmath>

struct Vec3 {
  float x = 0, y = 0, z = 0;

  Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
  Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
  Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
  Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }
};

inline float dot(const Vec3 &v1, const Vec3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
  return {
    v1.y * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x
  };
}

inline Vec3 normalize(const Vec3 &v) {
  float len = std::sqrt(dot(v, v));
  return {v.x / len, v.y / len, v.z / len};
}

struct Vec4 {
  float x = 0, y = 0, z = 0, w = 0;

  float operator[](int i) const {
    return (&x)[i];
  }

  float &operator[](int i) {
    return (&x)[i];
  }
};

