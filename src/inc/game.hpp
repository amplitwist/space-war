#ifndef GAME_HPP
#define GAME_HPP

#include "types.hpp"

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

private:
  bool LoadData();
  void UnloadData();

  void ProcessInput();
  void Update(f32 deltaTime);
  void Render();

  std::filesystem::path mAssetsPath;

  SDL_Window *mWindow{nullptr};
  SDL_Renderer *mRenderer{nullptr};

  bool mIsRunning{true};

  std::vector<Actor*> mActors;
  std::vector<Actor*> mPendingActors;
  bool mUpdatingActors;

  std::map<std::string, SDL_Texture*> mTextureMap;
  std::vector<class SpriteComponent*> mSprites;
};

#endif //GAME_HPP
