#ifndef GAME_HPP
#define GAME_HPP

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

private:
  void ProcessInput();
  void Update();
  void Render();

  SDL_Window *mWindow{nullptr};
  SDL_Renderer *mRenderer{nullptr};

  bool mIsRunning{true};

  std::vector<Actor*> mActors;
};

#endif //GAME_HPP
