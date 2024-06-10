#include "inc/vector2.hpp"

#include <cmath>

Vector2::Vector2()
: x{0.0f}, y{0.0f}
{
}

Vector2::Vector2(f32 x, f32 y)
: x{x}, y{y}
{
}

Vector2::Vector2(f32 angle)
: x{std::cos(angle)}, y{std::sin(angle)}
{
}

f32 &Vector2::operator[](i32 i)
{
  return (&x)[i];
}

Vector2 &Vector2::operator+=(const Vector2 &u)
{
  x += u.x;
  y += u.y;
  return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &u)
{
  x -= u.x;
  y -= u.y;
  return *this;
}

Vector2 &Vector2::operator*=(f32 k)
{
  x *= k;
  y *= k;
  return *this;
}

Vector2 &Vector2::operator/=(f32 k)
{
  x /= k;
  y /= k;
  return *this;
}

f32 Vector2::Length() const
{
  return std::sqrt(x * x + y * y);
}

f32 Vector2::SqrLength() const
{
  return x * x + y * y;
}

Vector2 &Vector2::Normalize()
{
  return *this /= Length();
}

Vector2 Vector2::Normalized() const
{
  return Vector2{*this} /= Length();
}

Vector2 operator+(const Vector2 &u, const Vector2 &v)
{
  return Vector2{u} += v;
}

Vector2 operator-(const Vector2 &u, const Vector2 &v)
{
  return Vector2{u} -= v;
}

Vector2 operator*(f32 k, const Vector2 &u)
{
  return Vector2{u} *= k;
}

Vector2 operator*(const Vector2 &u, f32 k)
{
  return Vector2{u} *= k;
}

Vector2 operator/(const Vector2 &u, f32 k)
{
  return Vector2{u} /= k;
}

f32 Dot(const Vector2 &u, const Vector2 &v)
{
  return u.x * v.x + u.y * v.y;
}

Vector2 Perp(const Vector2 &u)
{
  return {-u.y, u.x};
}

Vector2 Lerp(const Vector2 &u, const Vector2 &v, f32 frac)
{
  return {
    (1.0f - frac) * u.x + frac * v.x,
    (1.0f - frac) * u.y + frac * v.y
  };
}
