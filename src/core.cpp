#include "inc/core.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <map>
#include <print>
#include <variant>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

/*
#ifdef _WIN32
#include <Windows.h>
#else //linux
#endif
*/

struct ButtonState
{
  bool down;
  bool pressed;
  bool released;
};

class Core::Impl
{
public:
  bool Startup(std::string title, i32 width, i32 height)
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      return false;

    window_ = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width,
      height,
      0
    );

    if (!window_)
      return false;

    renderer_ = SDL_CreateRenderer(
      window_,
      -1,
      SDL_RENDERER_ACCELERATED |
      SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer_)
      return false;

    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    key_states_.resize(SDL_NUM_SCANCODES);
    mouse_button_states_.resize(3);

    /*
    #ifdef _WIN32
    char temp[MAX_PATH];
    GetModuleFileNameA(NULL, temp, MAX_PATH);
    assets_path = std::filesystem::path{temp}.parent_path()/"assets";
    #else //linux
    #endif
    */

    assets_path = std::filesystem::current_path()/"assets";
    std::cout << "assets path: " << assets_path.string() << '\n';

    return true;
  }

  void Cleanup()
  {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  void Service()
  {
    for (ButtonState &state : key_states_)
    {
      state.pressed = false;
      state.released = false;
    }

    for (ButtonState &state : mouse_button_states_)
    {
      state.pressed = false;
      state.released = false;
    }

    mouse_position_delta_ = {0.0f, 0.0f};
    mouse_wheel_delta_ = 0.0f;

    for (SDL_Event event; SDL_PollEvent(&event);)
      ProcessEvent(event);
  }

  bool WindowClosed() const
  {
    return window_closed_;
  }

  i64 Time() const
  {
    using namespace std::chrono;
    return duration_cast<microseconds>(
      high_resolution_clock::now().time_since_epoch()
    ).count();
  }

  bool KeyDown(Key key) const
  {
    return key_states_[KeyToScancode(key)].down;
  }

  bool KeyUp(Key key) const
  {
    return !key_states_[KeyToScancode(key)].down;
  }

  bool KeyPressed(Key key) const
  {
    return key_states_[KeyToScancode(key)].pressed;
  }

  bool KeyReleased(Key key) const
  {
    return key_states_[KeyToScancode(key)].released;
  }

  bool MouseButtonDown(MouseButton button) const
  {
    return mouse_button_states_[(i32)button].down;
  }

  bool MouseButtonUp(MouseButton button) const
  {
    return !mouse_button_states_[(i32)button].down;
  }

  bool MouseButtonPressed(MouseButton button) const
  {
    return mouse_button_states_[(i32)button].pressed;
  }

  bool MouseButtonReleased(MouseButton button) const
  {
    return mouse_button_states_[(i32)button].released;
  }

  Vector2 MousePosition() const
  {
    i32 x, y;
    SDL_GetMouseState(&x, &y);
    return {(f32)x, (f32)y};
  }

  Vector2 MousePositionDelta() const
  {
    return mouse_position_delta_;
  }

  f32 MouseWheelDelta() const
  {
    return mouse_wheel_delta_;
  }

  bool LoadTexture(std::string name)
  {
    std::cout << "loading texture " << name << '\n';
    SDL_Surface *surface{IMG_Load(
      (assets_path/"textures"/name).string().c_str()
    )};
    SDL_Texture *texture{SDL_CreateTextureFromSurface(renderer_, surface)};
    SDL_FreeSurface(surface);
    if (!texture)
    {
      std::cout << "failed to load texture " << name << '\n';
      return false;
    }
    textures_[name] = texture;
    return true;
  }

  void UnloadTexture(std::string name)
  {
    std::cout << "unloading texture " << name << '\n';
    if (!textures_.contains(name))
    {
      std::cout << "failed to unload texture " << name << '\n';
      return;
    }
    SDL_DestroyTexture(textures_.at(name));
    textures_.erase(name);
  }

  void DrawRect(const Rect &rect, u32 color, i32 layer)
  {
    RenderItem item;
    item.type  = RenderItem::Type::kRect;
    item.rect  = rect;
    item.color = color;
    item.layer = layer;
    render_items_.push_back(item);
  }

  void DrawSprite(const Sprite &sprite, const Pose &pose, i32 layer)
  {
    RenderItem item;
    item.type   = RenderItem::Type::kSprite;
    item.sprite = sprite;
    item.pose   = pose;
    item.layer  = layer;
    render_items_.push_back(item);
  }

  void DrawText(std::string texture, std::string text, float h)
  {
    for (i32 i = 0; i < text.size(); i++)
    {
      DrawSprite(
        {
          texture,
          {
            1.0f + (text[i] - 33) * 6.0f,
            1.0f,
            5.0f,
            10.0f
          }
        },
        {
          { 60.0f + i * 5.0f * 2.0f, 400.0f + h },
          {},
          { 2.0f, 2.0f }
        },
        0
      );
    }
  }

  void DrawCircle(const Vector2 &position, f32 radius)
  {
    RenderItem item;
    item.type          = RenderItem::Type::kCircle;
    item.pose.position = position;
    item.radius        = radius;
    render_items_.push_back(item);
  }

  void Render()
  {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);

    for (const RenderItem &item : render_items_)
    {
      if (item.type == RenderItem::Type::kRect)
      {
        SDL_FRect rect{*(SDL_FRect*)&item.rect};
        SDL_SetRenderDrawColor(renderer_, 158, 158, 158, 100);
        SDL_RenderFillRectF(renderer_, &rect);
      }
      else if (item.type == RenderItem::Type::kSprite)
      {
        if (item.sprite.texture.empty())
          continue;

        Vector2 position{WorldToScreen(item.pose.position)};

        i32 width, height;
        SDL_QueryTexture(
          textures_[item.sprite.texture],
          nullptr,
          nullptr,
          &width,
          &height
        );

        SDL_Rect src_rect{
          (i32)item.sprite.frame.x,
          (i32)item.sprite.frame.y,
          (i32)item.sprite.frame.w,
          (i32)item.sprite.frame.h
        };

        SDL_FRect dst_rect{
          position.x - item.sprite.frame.w / 2.0f * camera_scale_ * std::abs(item.pose.scale.x),
          position.y - item.sprite.frame.h / 2.0f * camera_scale_ * std::abs(item.pose.scale.y),
          item.sprite.frame.w * camera_scale_ * std::abs(item.pose.scale.x),
          item.sprite.frame.h * camera_scale_ * std::abs(item.pose.scale.y)
        };

        SDL_RenderCopyExF(
          renderer_,
          textures_[item.sprite.texture],
          &src_rect,
          &dst_rect,
          0.0f,
          nullptr,
          item.pose.scale.x < 0.0f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
        );
      }
      else if (item.type == RenderItem::Type::kCircle)
      {
        SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
        Vector2 pos{WorldToScreen(item.pose.position)};
        /*
        for (i32 i{0}; i < 300; i++)
        {
          Vector2 pos_1{pos + Vector2{(2.0f * (f32)M_PI / 300.0f) * (i + 0)} * item.radius * camera_scale_};
          Vector2 pos_2{pos + Vector2{(2.0f * (f32)M_PI / 300.0f) * (i + 1)} * item.radius * camera_scale_};
          SDL_RenderDrawLineF(renderer_, pos_1.x, pos_1.y, pos_2.x, pos_2.y);
        }
        */
          const int32_t diameter = (item.radius * camera_scale_ * 2);

          int32_t x = (item.radius * camera_scale_ - 1);
          int32_t y = 0;
          int32_t tx = 1;
          int32_t ty = 1;
          int32_t error = (tx - diameter);

          while (x >= y)
          {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(renderer_, pos.x + x, pos.y - y);
            SDL_RenderDrawPoint(renderer_, pos.x + x, pos.y + y);
            SDL_RenderDrawPoint(renderer_, pos.x - x, pos.y - y);
            SDL_RenderDrawPoint(renderer_, pos.x - x, pos.y + y);
            SDL_RenderDrawPoint(renderer_, pos.x + y, pos.y - x);
            SDL_RenderDrawPoint(renderer_, pos.x + y, pos.y + x);
            SDL_RenderDrawPoint(renderer_, pos.x - y, pos.y - x);
            SDL_RenderDrawPoint(renderer_, pos.x - y, pos.y + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
          }
      }
    }
    render_items_.clear();

    SDL_RenderPresent(renderer_);
  }


  void SetCameraPosition(const Vector2 &position)
  {
    camera_position_ = position;
  }

  void SetCameraScale(f32 scale)
  {
    camera_scale_ = scale;
  }

  Vector2 WorldToScreen(const Vector2 &position) const
  {
    return (position - camera_position_) * camera_scale_;
  }

  Vector2 ScreenToWorld(const Vector2 &position) const
  {
    return position / camera_scale_ + camera_position_;
  }

private:
  struct RenderItem
  {
    enum class Type
    {
      kRect,
      kSprite,
      kCircle
    };

    Type type;
    Rect rect;
    u32 color;
    i32 layer;
    Sprite sprite;
    Pose pose;
    f32 radius;
  };

  void ProcessEvent(SDL_Event &event)
  {
    switch (event.type)
    {
    case SDL_QUIT:
      window_closed_ = true;
      break;
    case SDL_KEYDOWN:
      if (event.key.repeat)
        break;
      key_states_[event.key.keysym.scancode].down     = true;
      key_states_[event.key.keysym.scancode].pressed  = true;
      key_states_[event.key.keysym.scancode].released = false;
      break;
    case SDL_KEYUP:
      if (event.key.repeat)
        break;
      key_states_[event.key.keysym.scancode].down     = false;
      key_states_[event.key.keysym.scancode].pressed  = false;
      key_states_[event.key.keysym.scancode].released = true;
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button > SDL_BUTTON_RIGHT)
        break;
      mouse_button_states_[event.button.button - 1].down     = true;
      mouse_button_states_[event.button.button - 1].pressed  = true;
      mouse_button_states_[event.button.button - 1].released = false;
      break;
    case SDL_MOUSEBUTTONUP:
      if (event.button.button > SDL_BUTTON_RIGHT)
        break;
      mouse_button_states_[event.button.button - 1].down     = false;
      mouse_button_states_[event.button.button - 1].pressed  = false;
      mouse_button_states_[event.button.button - 1].released = true;
      break;
    case SDL_MOUSEMOTION:
      mouse_position_delta_ += {(f32)event.motion.xrel, (f32)event.motion.yrel};
      break;
    case SDL_MOUSEWHEEL:
      mouse_wheel_delta_ += event.wheel.preciseY;
      break;
    }
  }

  i32 KeyToScancode(Key key) const
  {
    switch (key)
    {
    case Key::kW:      return SDL_SCANCODE_W;
    case Key::kS:      return SDL_SCANCODE_S;
    case Key::kA:      return SDL_SCANCODE_A;
    case Key::kD:      return SDL_SCANCODE_D;
    case Key::kUp:     return SDL_SCANCODE_UP;
    case Key::kDown:   return SDL_SCANCODE_DOWN;
    case Key::kLeft:   return SDL_SCANCODE_LEFT;
    case Key::kRight:  return SDL_SCANCODE_RIGHT;
    case Key::kSpace:  return SDL_SCANCODE_SPACE;
    case Key::kEscape: return SDL_SCANCODE_ESCAPE;
    default:           return 0;
    }
  }

  SDL_Window *window_{nullptr};
  SDL_Renderer *renderer_{nullptr};
  bool window_closed_{false};

  std::vector<ButtonState> key_states_;
  std::vector<ButtonState> mouse_button_states_;
  Vector2 mouse_position_delta_{0.0f, 0.0f};
  f32 mouse_wheel_delta_{0.0f};

  std::filesystem::path assets_path;
  std::map<std::string, SDL_Texture*> textures_;
  std::vector<RenderItem> render_items_;

  Vector2 camera_position_{0.0f, 0.0f};
  f32 camera_scale_{1.0f};
};

Core::Core()
: pimpl_{std::make_unique<Impl>()}
{
}

Core::~Core()
{
}

bool Core::Startup(std::string title, i32 width, i32 height)
{
  return pimpl_->Startup(title, width, height);
}

void Core::Cleanup()
{
  pimpl_->Cleanup();
}

void Core::Service()
{
  pimpl_->Service();
}

bool Core::WindowClosed() const
{
  return pimpl_->WindowClosed();
}

i64 Core::Time() const
{
  return pimpl_->Time();
}

bool Core::KeyDown(Key key) const
{
  return pimpl_->KeyDown(key);
}

bool Core::KeyUp(Key key) const
{
  return pimpl_->KeyUp(key);
}

bool Core::KeyPressed(Key key) const
{
  return pimpl_->KeyPressed(key);
}

bool Core::KeyReleased(Key key) const
{
  return pimpl_->KeyReleased(key);
}

bool Core::MouseButtonDown(MouseButton button) const
{
  return pimpl_->MouseButtonDown(button);
}

bool Core::MouseButtonUp(MouseButton button) const
{
  return pimpl_->MouseButtonUp(button);
}

bool Core::MouseButtonPressed(MouseButton button) const
{
  return pimpl_->MouseButtonPressed(button);
}

bool Core::MouseButtonReleased(MouseButton button) const
{
  return pimpl_->MouseButtonReleased(button);
}

Vector2 Core::MousePosition() const
{
  return pimpl_->MousePosition();
}

Vector2 Core::MousePositionDelta() const
{
  return pimpl_->MousePositionDelta();
}

f32 Core::MouseWheelDelta() const
{
  return pimpl_->MouseWheelDelta();
}

bool Core::LoadTexture(std::string name)
{
  return pimpl_->LoadTexture(name);
}

void Core::UnloadTexture(std::string name)
{
  pimpl_->UnloadTexture(name);
}

void Core::DrawRect(const Rect &rect, u32 color, i32 layer)
{
  pimpl_->DrawRect(rect, color, layer);
}

void Core::DrawSprite(const Sprite &sprite, const Pose &pose, i32 layer)
{
  pimpl_->DrawSprite(sprite, pose, layer);
}

void Core::DrawText(std::string texture, std::string text, float h)
{
  pimpl_->DrawText(texture, text, h);
}

void Core::DrawCircle(const Vector2 &position, f32 radius)
{
  pimpl_->DrawCircle(position, radius);
}

void Core::Render()
{
  pimpl_->Render();
}

void Core::SetCameraPosition(const Vector2 &position)
{
  pimpl_->SetCameraPosition(position);
}

void Core::SetCameraScale(f32 scale)
{
  pimpl_->SetCameraScale(scale);
}

Vector2 Core::WorldToScreen(const Vector2 &position) const
{
  return pimpl_->WorldToScreen(position);
}

Vector2 Core::ScreenToWorld(const Vector2 &position) const
{
  return pimpl_->ScreenToWorld(position);
}
