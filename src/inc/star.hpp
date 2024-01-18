#ifndef STAR_HPP
#define STAR_HPP

#include "actor.hpp"

class Star : public Actor
{
public:
  Star(class Game *game);
  ~Star();

  void VUpdate(f32 deltaTime) override;
};

#endif //STAR_HPP
