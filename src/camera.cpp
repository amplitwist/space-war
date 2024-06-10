#include "inc/camera.hpp"

#include <print>

Camera::Camera(Core *core, const Vector2 &position, f32 scale)
: core_{core}
, last_position_{position}
, current_position_{position}
, last_scale_{scale}
, current_scale_{scale}
, mouse_position_screen_{0.0f, 0.0f}
, mouse_position_world_{0.0f, 0.0f}
{
}

void Camera::Update(f32 dt)
{
  last_position_ = current_position_;
  last_scale_ = current_scale_;

  f32 mouse_wheel_delta{core_->MouseWheelDelta()};
  if (mouse_wheel_delta != 0.0f)
  {
    mouse_position_screen_ = core_->MousePosition();
    mouse_position_world_ = ScreenToWorld(mouse_position_screen_);
    current_scale_ += mouse_wheel_delta * 0.1f;
    if (current_scale_ < 0.1f)
      current_scale_ = 0.1f;
    else if (current_scale_ > 2.0f)
      current_scale_ = 2.0f;
    Vector2 after_zoom{ScreenToWorld(mouse_position_screen_)};
    current_position_ += mouse_position_world_ - after_zoom;
  }
  else
  {
    f32 move_speed{200.0f};
    Vector2 move_delta{0.0f, 0.0f};

    if (core_->KeyDown(Key::kW)) move_delta.y -= 1.0f;
    if (core_->KeyDown(Key::kS)) move_delta.y += 1.0f;
    if (core_->KeyDown(Key::kA)) move_delta.x -= 1.0f;
    if (core_->KeyDown(Key::kD)) move_delta.x += 1.0f;

    if (move_delta.x != 0.0f || move_delta.y != 0.0f)
      move_delta = move_delta.Normalize() * move_speed * dt;

    current_position_ += move_delta / current_scale_;

    if (core_->MouseButtonDown(MouseButton::kRight) ||
        core_->MouseButtonDown(MouseButton::kMiddle))
    {
      Vector2 delta{core_->MousePositionDelta()};
      current_position_ -= delta / current_scale_;
    }
  }
}

Vector2 Camera::GetPosition(f32 frac) /*const*/
{
  if (last_scale_ == current_scale_)
  {
    if (last_position_.x == current_position_.x &&
        last_position_.y == current_position_.y)
      return current_position_;

    return Lerp(last_position_, current_position_, frac);
  }

  f32 temp{current_scale_};
  current_scale_ = GetScale(frac);
  Vector2 after_zoom{ScreenToWorld(mouse_position_screen_)};
  Vector2 position{current_position_ + mouse_position_world_ - after_zoom};
  current_scale_ = temp;
  return position;
}

f32 Camera::GetScale(f32 frac) const
{
  if (last_scale_ == current_scale_)
    return current_scale_;

  return (1.0f - frac) * last_scale_ + frac * current_scale_;
}

void Camera::SetPosition(const Vector2 &position)
{
  current_position_ = position;
}

Vector2 Camera::WorldToScreen(const Vector2 &position) const
{
  return (position - current_position_) * current_scale_;
}

Vector2 Camera::ScreenToWorld(const Vector2 &position) const
{
  return position / current_scale_ + current_position_;
}
