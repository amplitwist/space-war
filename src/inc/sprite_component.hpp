#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include "types.hpp"
#include "component.hpp"

#include <SDL.h>

#include <string_view>

class SpriteComponent : public Component
{
public:
  SpriteComponent(Actor *owner, std::string name, int drawOrder = 100);
  ~SpriteComponent();

  virtual void SetTexture(SDL_Texture *texture);

  std::string GetName() const { return mName; }
  i32 GetDrawOrder() const { return mDrawOrder; }

private:
  std::string mName;
  int mDrawOrder;

  SDL_Texture *mTexture{nullptr};
  i32 mWidth{0};
  i32 mHeight{0};
};

#endif //SPRITE_COMPONENT
