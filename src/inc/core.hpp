#ifndef CORE_HPP
#define CORE_HPP

#include <memory>
#include <string>

#include "types.hpp"
#include "vector2.hpp"

enum class Key
{
  kW, kS, kA, kD,
  kUp, kDown, kLeft, kRight,
  kSpace,
  kEscape
};

enum class MouseButton
{
  kLeft, kMiddle, kRight
};

struct Rect
{
  f32 x, y, w, h;
};

struct Sprite
{
  std::string texture;
  Rect frame;
  bool flip;
};

struct Pose
{
  Vector2 position;
  Vector2 orientation;
  Vector2 scale;
};

class Core
{
public:
  Core();
  ~Core();

  bool Startup(std::string title, i32 width, i32 height);
  void Cleanup();

  void Service();

  bool WindowClosed() const;

  i64 Time() const;

  bool KeyDown(Key key) const;
  bool KeyUp(Key key) const;
  bool KeyPressed(Key key) const;
  bool KeyReleased(Key key) const;

  bool MouseButtonDown(MouseButton button) const;
  bool MouseButtonUp(MouseButton button) const;
  bool MouseButtonPressed(MouseButton button) const;
  bool MouseButtonReleased(MouseButton button) const;

  Vector2 MousePosition() const;
  Vector2 MousePositionDelta() const;

  f32 MouseWheelDelta() const;

  bool LoadTexture(std::string name);
  void UnloadTexture(std::string name);

  void DrawRect(const Rect &rect, u32 color, i32 layer);
  void DrawSprite(const Sprite &sprite, const Pose &pose, i32 layer);
  void DrawText(std::string texture, std::string text, float h);
  void DrawCircle(const Vector2 &position, f32 radius);

  void Render();

  void SetCameraPosition(const Vector2 &position);
  void SetCameraScale(f32 scale);

  Vector2 WorldToScreen(const Vector2 &position) const;
  Vector2 ScreenToWorld(const Vector2 &position) const;

private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

#endif //CORE_HPP
