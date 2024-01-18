#include <SDL.h>

#include "inc/game.hpp"

int main(int argc, char *argv[])
{
  Game game{std::filesystem::path(argv[0]).parent_path()};

  if (game.Init())
    game.Run();

  game.Deinit();

  return 0;
}
