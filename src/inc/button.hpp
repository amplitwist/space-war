#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>

#include "core.hpp"

class Button
{
public:
  Button(Core *core, const Rect &rect, std::function<void()> callback);

  void Update(f32 dt);
  void Render(f32 frac);

private:
  Core *core;
  Rect rect;
  std::function<void()> callback;
};

#endif //BUTTON_HPP
