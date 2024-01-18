#include "inc/game.hpp"

int main()
{
  Game game;
  if (game.Init())
    game.Run();
  game.Deinit();
  return 0;
}
