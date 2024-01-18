#include "inc/game.hpp"

#include "inc/actor.hpp"

#include <algorithm>

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
    "Space War",
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
  while (mIsRunning)
  {
    ProcessInput();
    Update(1.0f);
    Render();
  }
}

void Game::AddActor(Actor *actor)
{
  if (mUpdatingActors)
    mPendingActors.push_back(actor);
  else
    mActors.push_back(actor);
}

void Game::RemoveActor(Actor *actor)
{
  auto it{std::find(mPendingActors.begin(), mPendingActors.end(), actor)};
  if (it != mPendingActors.end())
  {
    std::iter_swap(it, mPendingActors.end() - 1);
    mPendingActors.pop_back();
  }
  it = std::find(mActors.begin(), mActors.end(), actor);
  if (it != mActors.end())
  {
    std::iter_swap(it, mActors.end() - 1);
    mActors.pop_back();
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

  const u8 *keyStates{
    reinterpret_cast<const u8*>(SDL_GetKeyboardState(nullptr))
  };

  if (keyStates[SDL_SCANCODE_ESCAPE])
    mIsRunning = false;

  mUpdatingActors = true;
  for (auto actor : mActors)
    actor->ProcessInput(keyStates);
  mUpdatingActors = false;
}

void Game::Update(f32 deltaTime)
{
  mUpdatingActors = true;
  for (auto actor : mActors)
    actor->Update(deltaTime);
  mUpdatingActors = false;

  for (auto actor : mPendingActors)
    mActors.push_back(actor);
  mPendingActors.clear();

  std::vector<Actor*> deadActors;
  for (auto actor : mActors)
  {
    if (actor->GetState() == Actor::State::kDead)
      deadActors.push_back(actor);
  }

  for (auto actor : deadActors)
    delete actor;
}

void Game::Render()
{
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
  SDL_RenderClear(mRenderer);
  /* ... */
  SDL_RenderPresent(mRenderer);
}
