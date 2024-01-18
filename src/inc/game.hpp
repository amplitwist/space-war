#ifndef GAME_HPP
#define GAME_HPP

#include "types.hpp"

#include <SDL.h>

#include <vector>

class Actor;

class Game
{
public:
  Game();
  ~Game();

  bool Init();
  void Deinit();

  void Run();

  void AddActor(Actor *actor);
  void RemoveActor(Actor *actor);

private:
  void ProcessInput();
  void Update(f32 deltaTime);
  void Render();

  SDL_Window *mWindow{nullptr};
  SDL_Renderer *mRenderer{nullptr};

  bool mIsRunning{true};

  std::vector<Actor*> mActors;
  std::vector<Actor*> mPendingActors;
  bool mUpdatingActors;
};

#endif //GAME_HPP
