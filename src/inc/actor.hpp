#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "vector2.hpp"

class Actor
{
public:
  Vector2 GetPosition(f32 frac);

private:
  Vector2 last_position_;
  Vector2 current_position_;
};

#endif //ACTOR_HPP
