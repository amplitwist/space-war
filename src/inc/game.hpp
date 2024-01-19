#ifndef GAME_HPP
#define GAME_HPP

#include "types.hpp"
#include "vector2.hpp"

#include <SDL.h>

#include <filesystem>
#include <map>
#include <string>
#include <vector>

class Actor;

class Game
{
public:
  Game(std::filesystem::path parentPath);
  ~Game();

  bool Init();
  void Deinit();

  void Run();

  void AddActor(Actor *actor);
  void RemoveActor(Actor *actor);

  bool LoadTexture(std::string name);

  void AddSprite(class SpriteComponent *sprite);
  void DrawSprite(class SpriteComponent *sprite);

  void SetCameraPosition(Vector2 position) { mCameraPosition = position; }
  f32 GetScale() const { return mCameraScale; }
  void SetScale(f32 scale) { mCameraScale = scale; }

private:
  bool LoadData();
  void UnloadData();

  void ProcessInput();
  void Update(f32 deltaTime);
  void Render();

  Vector2 MakeRelativeToCamera(Vector2 position);

  std::filesystem::path mAssetsPath;

  SDL_Window *mWindow{nullptr};
  SDL_Renderer *mRenderer{nullptr};

  bool mIsRunning{true};

  std::vector<Actor*> mActors;
  std::vector<Actor*> mPendingActors;
  bool mUpdatingActors;

  std::map<std::string, SDL_Texture*> mTextureMap;
  std::vector<class SpriteComponent*> mSprites;

  Vector2 mCameraPosition;
  i32 mCameraWidth{800};
  i32 mCameraHeight{600};
  f32 mCameraScale{1.0f};

  class Planet *mMoon;
};

#endif //GAME_HPP
