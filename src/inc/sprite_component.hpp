#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "types.hpp"
#include "component.hpp"

#include <SDL.h>

class SpriteComponent : public Component
{
public:
  SpriteComponent(Actor *owner, int drawOrder = 100);
  ~SpriteComponent();

  virtual void SetTexture(SDL_Texture *texture);

  int GetDrawOrder() const { return mDrawOrder; }

private:
  int mDrawOrder;
  SDL_Texture *mTexture{nullptr};
  i32 mWidth{0};
  i32 mHeight{0};
};

#endif //SPRITE_COMPONENT
