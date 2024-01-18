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

  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;

  bool mIsRunning;

  std::vector<Actor*> mActors;
};

#endif //GAME_HPP
