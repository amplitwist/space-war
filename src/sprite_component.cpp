#include "inc/sprite_component.hpp"
#include "inc/actor.hpp"
#include "inc/game.hpp"

SpriteComponent::SpriteComponent(Actor *owner, std::string name, int drawOrder)
: Component{owner}
, mName{name}
, mDrawOrder{drawOrder}
{
  GetOwner()->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::SetTexture(SDL_Texture *texture)
{
  mTexture = texture;
  SDL_QueryTexture(texture, nullptr, nullptr, &mWidth, &mHeight);
}
