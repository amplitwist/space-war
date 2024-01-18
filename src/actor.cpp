#include "inc/actor.hpp"

#include "inc/game.hpp"
#include "inc/component.hpp"

Actor::Actor(Game *game)
: mGame{game}
{
  mGame->AddActor(this);
}

Actor::~Actor()
{
  mGame->RemoveActor(this);
}

void Actor::Update(f32 deltaTime)
{
  if (mState != State::kActive)
    return;

  UpdateComponents(deltaTime);
  VUpdate(deltaTime);
}

void Actor::ProcessInput(const u8 *keyStates)
{
  if (mState != State::kActive)
    return;

  for (auto component : mComponents)
    component->ProcessInput(keyStates);
}

void Actor::AddComponent(class Component *component)
{
}

void Actor::RemoveComponent(class Component *component)
{
}

void Actor::UpdateComponents(f32 deltaTime)
{
  for (auto component : mComponents)
    component->Update(deltaTime);
}

void Actor::VUpdate(f32 deltaTime)
{
}

void Actor::VProcessInput(const u8 *keyStates)
{
}
