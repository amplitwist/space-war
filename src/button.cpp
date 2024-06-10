#include "inc/button.hpp"

Button::Button(Core *core, const Rect &rect, std::function<void()> callback)
: core{core}
, rect{rect}
, callback{callback}
{
}

void Button::Update(f32 dt)
{
  if (core->MouseButtonPressed(MouseButton::kLeft))
  {
    Vector2 mouse_position{core->MousePosition()};
    if (mouse_position.x >= rect.x && mouse_position.x < rect.x + rect.w &&
        mouse_position.y >= rect.y && mouse_position.y < rect.y + rect.h)
      callback();
  }
}

void Button::Render(f32 frac)
{
  core->DrawRect(rect, 0, 0);
}
