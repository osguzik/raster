#pragma once

struct Vec3 {
  float x = 0, y = 0, z = 0;

  Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
  Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
  Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
};

struct Vec4 {
  float x = 0, y = 0, z = 0, w = 0;

  float operator[](int i) const {
    return (&x)[i];
  }

  float &operator[](int i) {
    return (&x)[i];
  }
};

