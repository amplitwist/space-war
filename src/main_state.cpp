#include "inc/states.hpp"

#include <cmath>
#include <iostream>

class Planet : public Actor
{
public:

};

MainState::MainState()
{
}

MainState::~MainState()
{
}

void MainState::Startup()
{
  QUIT_ON_FAILURE(core_->LoadTexture("star.png"));
  QUIT_ON_FAILURE(core_->LoadTexture("planet_1.png"));

  camera_ = std::make_unique<Camera>(core_, Vector2{0.0f, 0.0f}, 1.0f);

  solar_panel_button_ = std::make_unique<Button>(
    core_,
    Rect{10.0f, 490.0f, 100.0f, 100.0f},
    [] { std::cout << "solar panel button pressed\n"; }
  );

  mineral_extractor_button_ = std::make_unique<Button>(
    core_,
    Rect{120.0f, 490.0f, 100.0f, 100.0f},
    [] { std::cout << "mineral extractor button pressed\n"; }
  );

  root_actor_.AppendSubnode(new Planet);
}

void MainState::Cleanup()
{
  core_->UnloadTexture("star.png");
  core_->UnloadTexture("planet_1.png");
}

void MainState::Update(f32 dt)
{
  QUIT_ON_FAILURE(!core_->KeyDown(Key::kEscape));

  camera_->Update(dt);
  solar_panel_button_->Update(dt);
  mineral_extractor_button_->Update(dt);

  last_planet_angle_ = current_planet_angle_;
  current_planet_angle_ += 0.30f * dt;
  if (lock_camera_)
    camera_->SetPosition(Vector2{std::cos(current_planet_angle_), -std::sin(current_planet_angle_)} * 800.0f - Vector2{400.0f, 300.0f} * camera_->GetScale(1.0f));
}

void MainState::Render(f32 frac)
{
  core_->SetCameraPosition(camera_->GetPosition(frac));
  core_->SetCameraScale(camera_->GetScale(frac));

  Sprite star_sprite{"star.png", {0.0f, 0.0f, 299.0f, 300.0f}, false};
  Pose star_pose{
    Vector2{0.0f, 0.0f},
    Vector2{1.0f, 0.0f},
    Vector2{1.0f, 1.0f}
  };
  core_->DrawSprite(star_sprite, star_pose, 0);
  core_->DrawCircle(Vector2{0.0f, 0.0f}, 800.0f);

  f32 temp_angle{(1.0f - frac) * last_planet_angle_ + frac * current_planet_angle_};
  planet_position_ = Vector2{std::cos(temp_angle), -std::sin(temp_angle)} * 800.0f;
  Sprite planet_sprite{"planet_1.png", {0.0f, 0.0f, 200.0f, 200.0f}, false};
  Pose planet_pose{
    planet_position_,
    Vector2{1.0f, 0.0f},
    Vector2{0.5f, 0.5f}
  };
  core_->DrawSprite(planet_sprite, planet_pose, 0);

  core_->DrawRect({0.0f, 480.0f, 800.0f, 120.0f}, 0, 0);
  solar_panel_button_->Render(frac);
  mineral_extractor_button_->Render(frac);
}
