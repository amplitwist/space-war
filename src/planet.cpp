#include "inc/planet.hpp"
#include "inc/sprite_component.hpp"
#include "inc/orbit_component.hpp"
#include "inc/star.hpp"

Planet::Planet(class Game *game, Star *star)
: Actor{game}
{
  new SpriteComponent{this, "planet_1.png", 180};
  new OrbitComponent{this, star, 300.0f};
}

Planet::~Planet()
{
}

void Planet::VUpdate(f32 deltaTime)
{
}
