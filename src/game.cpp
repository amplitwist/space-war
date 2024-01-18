#include "inc/game.hpp"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return false;

  mWindow = SDL_CreateWindow(
    "Game",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    800,
    600,
    0
  );

  if (!mWindow)
    return false;

  mRenderer = SDL_CreateRenderer(
    mWindow,
    -1,
    SDL_RENDERER_ACCELERATED |
    SDL_RENDERER_PRESENTVSYNC
  );

  if (!mRenderer)
    return false;

  return true;
}

void Game::Deinit()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::Run()
{
  mIsRunning = true;

  while (mIsRunning)
  {
    ProcessInput();
  }
}

void Game::ProcessInput()
{
  for (SDL_Event event; SDL_PollEvent(&event);)
  {
    switch (event.type)
    {
      case SDL_QUIT:
        mIsRunning = false;
        break;
    }
  }
}
