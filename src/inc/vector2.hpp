#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "types.hpp"

struct Vector2
{
  Vector2() = default;
  Vector2(f32 x, f32 y) : x{x}, y{y} {}

  f32 x, y;
};

#endif //VECTOR2_HPP
