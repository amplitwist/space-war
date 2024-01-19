#include "inc/game.hpp"

#include "inc/actor.hpp"
#include "inc/star.hpp"
#include "inc/sprite_component.hpp"
#include "inc/camera.hpp"
#include "inc/planet.hpp"

#include <SDL_image.h>

#include <algorithm>
#include <fstream>
#include <iostream>

Game::Game(std::filesystem::path parentPath)
: mAssetsPath{parentPath/"assets"}
, mCameraPosition{0.0f, 0.0f}
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

  //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

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
  SDL_Surface *surface{IMG_Load((mAssetsPath/"sprites"/name).string().c_str())};
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
  RETURN_IF_FALSE(LoadTexture("star.png"));
  RETURN_IF_FALSE(LoadTexture("planet_1.png"));
  RETURN_IF_FALSE(LoadTexture("planet_2.png"));
  RETURN_IF_FALSE(LoadTexture("planet_3.png"));
  RETURN_IF_FALSE(LoadTexture("moon_1.png"));

  new Camera{this};
  Star *star{new Star{this}};
  Planet *planet{new Planet{this, star, 2000.0f, 0.001f}};
  new Planet{this, planet, 300.0f, 0.01f, true};
  mMoon = planet;

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
      case SDL_MOUSEWHEEL:
        if (event.wheel.y < 0) //scroll down
        {
          f32 __x = event.wheel.mouseX / mCameraScale + mCameraPosition.x;
          f32 __y = event.wheel.mouseY / mCameraScale + mCameraPosition.y;
          mCameraScale -= 0.1f;
          f32 _x = event.wheel.mouseX / mCameraScale + mCameraPosition.x;
          f32 _y = event.wheel.mouseY / mCameraScale + mCameraPosition.y;
          mCameraPosition.x += _x - __x;
          mCameraPosition.x += _y - __y;
        }
        else if (event.wheel.y > 0) //scroll up
        {
          mCameraScale += 0.1f;
        }
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

Vector2 Game::MakeRelativeToCamera(Vector2 position)
{
  return { position.x - mCameraPosition.x, position.y - mCameraPosition.y };
}

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y)
  {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

    if (error <= 0)
    {
        ++y;
        error += ty;
        ty += 2;
    }

    if (error > 0)
    {
        --x;
        tx += 2;
        error += (tx - diameter);
    }
  }
}

void Game::DrawSprite(SpriteComponent *sprite)
{
  Actor *actor{sprite->GetOwner()};
  SDL_Texture *texture{mTextureMap[sprite->GetName()]};
  SDL_Rect rect;
  SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

  rect.w *= mCameraScale;
  rect.h *= mCameraScale;
  rect.x = (actor->GetPosition().x - mCameraPosition.x) * mCameraScale - rect.w / 2.0f;
  rect.y = (actor->GetPosition().y - mCameraPosition.y) * mCameraScale - rect.h / 2.0f;

  Vector2 topLeft    {actor->GetPosition().x,          actor->GetPosition().y         };
  Vector2 topRight   {actor->GetPosition().x + rect.w, actor->GetPosition().y         };
  Vector2 bottomLeft {actor->GetPosition().x,          actor->GetPosition().y + rect.h};
  Vector2 bottomRight{actor->GetPosition().x + rect.w, actor->GetPosition().y + rect.h};
  
  SDL_RenderCopyEx(mRenderer, texture, nullptr, &rect, -actor->GetAngle(), nullptr, SDL_FLIP_NONE);
}

void Game::Render()
{
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
  SDL_RenderClear(mRenderer);
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 0);
  DrawCircle(mRenderer, -(mCameraPosition.x - mMoon->GetPosition().x) * mCameraScale, -(mCameraPosition.y - mMoon->GetPosition().y) * mCameraScale, 300.0f * mCameraScale);
  DrawCircle(mRenderer, -mCameraPosition.x * mCameraScale, -mCameraPosition.y * mCameraScale, 2000.0f * mCameraScale);
  for (auto sprite : mSprites)
    DrawSprite(sprite);
  SDL_RenderPresent(mRenderer);
}
