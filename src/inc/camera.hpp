#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "actor.hpp"
#include "game.hpp"

#include <SDL.h>

#include <iostream>

class Camera : public Actor
{
public:
  Camera(Game *game) : Actor{game} {}

  void VUpdate(f32 deltaTime) override
  {
    Vector2 position{GetPosition()};
    position.x += mHorizontalSpeed * deltaTime;
    position.y += mVerticalSpeed * deltaTime;
    SetPosition(position);
    GetGame()->SetCameraPosition(position);
    std::cout << "(" << position.x << ", " << position.y << ")\n";
  }

  void VProcessInput(const u8 *keyStates)
  {
    mVerticalSpeed = 0.0f;
    mHorizontalSpeed = 0.0f;

    if (keyStates[SDL_SCANCODE_A]) mHorizontalSpeed -= 10.0f;
    if (keyStates[SDL_SCANCODE_D]) mHorizontalSpeed += 10.0f;
    if (keyStates[SDL_SCANCODE_W]) mVerticalSpeed   -= 10.0f;
    if (keyStates[SDL_SCANCODE_S]) mVerticalSpeed   += 10.0f;
  }

private:
  f32 mHorizontalSpeed{0.0f};
  f32 mVerticalSpeed{0.0f};
};

#endif //CAMERA_HPP
