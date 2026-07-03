#pragma once

struct Vec2 {
  float x = 0, y = 0;

  Vec2 operator+(const Vec2& v) const { return {x + v.x, y + v.y}; }
  Vec2 operator-(const Vec2& v) const { return {x - v.x, y - v.y}; }
  Vec2 operator*(float s) const { return {x * s, y * s}; }
};

struct Vec3 {
  float x = 0, y = 0, z = 0;

  Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
  Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
  Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
};

