#include <SDL.h>

#include "inc/game.hpp"

int main(int argc, char *argv[])
{
  Game game;

  if (game.Init())
    game.Run();

  game.Deinit();

  return 0;
}
