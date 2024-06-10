#ifndef STATES_HPP
#define STATES_HPP

#include "animation.hpp"
#include "button.hpp"
#include "camera.hpp"
#include "system.hpp"

class RaceSelectState : public State
{
public:
  RaceSelectState();
  ~RaceSelectState();

  void Startup() override;
  void Cleanup() override;

  void Update(f32 dt) override;
  void Render(f32 frac) override;

private:
  std::unique_ptr<Animation> animation_;
};

class MainState : public State
{
public:
  MainState();
  ~MainState();

  void Startup() override;
  void Cleanup() override;

  void Update(f32 dt) override;
  void Render(f32 frac) override;

private:
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<Button> solar_panel_button_;
  std::unique_ptr<Button> mineral_extractor_button_;
  f32 last_planet_angle_{0.0f};
  f32 current_planet_angle_{0.0f};
  Vector2 planet_position_{800.0f, 0.0f};
  bool lock_camera_{true};
};

#endif //STATES_HPP
