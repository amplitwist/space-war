#include "inc/star.hpp"
#include "inc/sprite_component.hpp"

Star::Star(class Game *game)
: Actor{game}
{
  new SpriteComponent{this, "star.bmp", 150};
}

Star::~Star()
{
}

void Star::VUpdate(f32 deltaTime)
{
}
