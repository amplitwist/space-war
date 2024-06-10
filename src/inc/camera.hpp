#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "core.hpp"

class Camera
{
public:
  Camera(Core *core, const Vector2 &position, f32 scale);

  void Update(f32 dt);

  Vector2 GetPosition(f32 frac) /*const*/;
  f32 GetScale(f32 frac) const;

  void SetPosition(const Vector2 &position);

  Vector2 WorldToScreen(const Vector2 &position) const;
  Vector2 ScreenToWorld(const Vector2 &position) const;

private:
  Core *core_;

  Vector2 last_position_;
  Vector2 current_position_;

  f32 last_scale_;
  f32 current_scale_;

  Vector2 mouse_position_screen_;
  Vector2 mouse_position_world_;
};

#endif //CAMERA_HPP
