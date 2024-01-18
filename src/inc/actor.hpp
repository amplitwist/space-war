#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "types.hpp"

#include <vector>

class Actor
{
public:
  enum class State
  {
    kActive,
    kPaused,
    kDead
  };

  Actor(class Game *game);
  virtual ~Actor();

  void Update(f32 deltaTime);

  void ProcessInput(const u8 *keyStates);

  void SetState(State state) { mState = state; }
  State GetState() const { return mState; }

  class Game* GetGame() const { return mGame; };

  void AddComponent(class Component *component);
  void RemoveComponent(class Component *component);

private:
  void UpdateComponents(f32 deltaTime);
  virtual void VUpdate(f32 deltaTime);

  virtual void VProcessInput(const u8 *keyStates);

  class Game *mGame;

  State mState{State::kActive};

  std::vector<class Component*> mComponents;
};

#endif //ACTOR_HPP
