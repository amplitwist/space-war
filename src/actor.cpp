#include "inc/actor.hpp"

Vector2 Actor::GetPosition(f32 frac)
{
  return Lerp(last_position_, current_position_, frac);
}
