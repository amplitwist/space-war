#ifndef PLANET_HPP
#define PLANET_HPP

#include "actor.hpp"

class Planet : public Actor
{
public:
  Planet(class Game *game, Actor *primaryBody, f32 radius, f32 speed, bool is_moon = false);
  ~Planet();

  void VUpdate(f32 deltaTime) override;

private:
  Actor *primaryBody;
};

#endif //PLANET_HPP
