#include "inc/sprite_component.hpp"

SpriteComponent::SpriteComponent(Actor *owner, int drawOrder)
: Component{owner}
, mDrawOrder{drawOrder}
{

}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::SetTexture(SDL_Texture *texture)
{
  mTexture = texture;
  SDL_QueryTexture(texture, nullptr, nullptr, &mWidth, &mHeight);
}
