#ifndef PLANET_HPP
#define PLANET_HPP

#include "actor.hpp"

class Planet : public Actor
{
public:
  Planet(class Game *game, class Star *star);
  ~Planet();

  void VUpdate(f32 deltaTime) override;

private:
  class Star *mStar;
};

#endif //PLANET_HPP
