#include "inc/game.hpp"

#include "inc/actor.hpp"
#include "inc/star.hpp"
#include "inc/sprite_component.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

Game::Game(std::filesystem::path parentPath)
: mAssetsPath{parentPath/"assets"}
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

  return LoadData();
}

void Game::Deinit()
{
  UnloadData();
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

void Game::AddSprite(SpriteComponent *sprite)
{
  auto comp{[] (SpriteComponent *a, SpriteComponent *b) {
    return a->GetDrawOrder() < b->GetDrawOrder();
  }};
  auto it{std::lower_bound(mSprites.begin(), mSprites.end(), sprite, comp)};
  mSprites.insert(it, sprite);
}

bool Game::LoadTexture(std::string name)
{
  //std::ifstream file{mAssetsPath/"textures"/name};
  //if (!file.is_open()) return false;
  //std::filesystem::path path{mAssetsPath/"textures"/name};
  mAssetsPath = "assets";
  std::cout << "loading sprite " << (mAssetsPath/"sprites"/name).string().c_str() << std::endl;
  SDL_Surface *surface{SDL_LoadBMP((mAssetsPath/"sprites"/name).string().c_str())};
  std::cout << surface << '\n';
  mTextureMap[name] = SDL_CreateTextureFromSurface(mRenderer, surface);
  SDL_FreeSurface(surface);
  if (!mTextureMap[name]) return false;
  return true;
}

#define RETURN_IF_FALSE(x) \
do { if (!(x)) return false; } while (0)

bool Game::LoadData()
{
  RETURN_IF_FALSE(LoadTexture("star.bmp"));
  new Star{this};
  return true;
}

void Game::UnloadData()
{
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

void Game::DrawSprite(SpriteComponent *sprite)
{
  Actor *actor{sprite->GetOwner()};
  SDL_Texture *texture{mTextureMap[sprite->GetName()]};
  SDL_Rect rect;
  SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
  rect.w *= 0.7f;
  rect.h *= 0.7f;
  rect.x = actor->GetPosition().x;
  rect.y = actor->GetPosition().y;
  SDL_RenderCopyEx(mRenderer, texture, nullptr, &rect, 0.0, nullptr, SDL_FLIP_NONE);
}

void Game::Render()
{
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
  SDL_RenderClear(mRenderer);
  for (auto sprite : mSprites)
  {
    
    DrawSprite(sprite);
  }
  SDL_RenderPresent(mRenderer);
}
