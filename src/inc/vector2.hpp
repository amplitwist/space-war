#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "types.hpp"

struct Vector2
{
  Vector2();
  Vector2(f32 x, f32 y);
  Vector2(f32 angle);

  f32 &operator[](i32 i);

  Vector2 &operator+=(const Vector2 &u);
  Vector2 &operator-=(const Vector2 &u);
  Vector2 &operator*=(f32 k);
  Vector2 &operator/=(f32 k);

  f32 Length() const;
  f32 SqrLength() const;

  Vector2 &Normalize();
  Vector2 Normalized() const;

  f32 x, y;
};

Vector2 operator+(const Vector2 &u, const Vector2 &v);
Vector2 operator-(const Vector2 &u, const Vector2 &v);
Vector2 operator*(f32 k, const Vector2 &u);
Vector2 operator*(const Vector2 &u, f32 k);
Vector2 operator/(const Vector2 &u, f32 k);

f32 Dot(const Vector2 &u, const Vector2 &v);
Vector2 Perp(const Vector2 &u);

Vector2 Lerp(const Vector2 &u, const Vector2 &v, f32 frac);

#endif //VECTOR2_HPP
