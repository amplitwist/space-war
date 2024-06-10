#include "inc/states.hpp"

int main()
{
  System system;

  if (system.Startup("Space War", 800, 600))
    system.Run(std::make_unique<RaceSelectState>());

  system.Cleanup();

  return 0;
}
