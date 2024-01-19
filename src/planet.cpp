#include "inc/planet.hpp"
#include "inc/sprite_component.hpp"
#include "inc/orbit_component.hpp"
#include "inc/star.hpp"

Planet::Planet(class Game *game, Actor *primaryBody, f32 radius, f32 speed, bool is_moon)
: Actor{game}
{
  new SpriteComponent{this, (is_moon ? "moon_1.png" : "planet_1.png"), 180};
  new OrbitComponent{this, primaryBody, radius, speed};
}

Planet::~Planet()
{
}

void Planet::VUpdate(f32 deltaTime)
{
  SetAngle(GetAngle() + 0.5f * deltaTime);
}
