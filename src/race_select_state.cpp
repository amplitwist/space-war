#include "inc/states.hpp"

RaceSelectState::RaceSelectState()
{
}

RaceSelectState::~RaceSelectState()
{
}

void RaceSelectState::Startup()
{
  QUIT_ON_FAILURE(core_->LoadTexture("creature_1.png"));
  QUIT_ON_FAILURE(core_->LoadTexture("right_arrow_2.png"));
  QUIT_ON_FAILURE(core_->LoadTexture("font.png"));

  animation_ = std::make_unique<Animation>("creature_1.png", 20);
  animation_->AddFrame({  0.0f, 0.0f, 256.0f, 256.0f});
  animation_->AddFrame({256.0f, 0.0f, 256.0f, 256.0f});
  animation_->AddFrame({512.0f, 0.0f, 256.0f, 256.0f});
  animation_->AddFrame({768.0f, 0.0f, 256.0f, 256.0f});
}

void RaceSelectState::Cleanup()
{
  core_->UnloadTexture("creature_1.png");
  core_->UnloadTexture("right_arrow_2.png");
  core_->UnloadTexture("font.png");
}

void RaceSelectState::Update(f32 dt)
{
  QUIT_ON_FAILURE(!core_->KeyDown(Key::kEscape));

  animation_->Update(dt);

  if (core_->KeyPressed(Key::kSpace))
    system_->ChangeState(std::make_unique<MainState>());
}

void RaceSelectState::Render(f32 frac)
{
  Pose creature_pose{
    Vector2{400.0f, 200.0f},
    Vector2{1.0f, 0.0f},
    Vector2{1.0f, 1.0f}
  };
  core_->DrawSprite(animation_->GetSprite(), creature_pose, 0);

  Sprite arrow_sprite{
    "right_arrow_2.png",
    { 0.0f, 0.0f, 128.0f, 128.0f }
  };
  Pose arrow_pose{
    Vector2{650.0f, 200.0f},
    Vector2{1.0f, 0.0f},
    Vector2{1.0f, 1.0f}
  };
  core_->DrawSprite(arrow_sprite, arrow_pose, 0);
  arrow_pose.position.x = 150.0f;
  arrow_pose.scale.x = -1.0f;
  core_->DrawSprite(arrow_sprite, arrow_pose, 0);
  std::string d1 = "The Tooth Crabs, scientifically referred to as Dens Kancerious, are a";
  std::string d2 = "fascinating extraterrestrial species that have captivated scientists";
  std::string d3 = "and explorers alike across the galaxy. Originating from the distant";
  std::string d4 = "planet of Carapodia, these unique creatures have evolved in a truly";
  std::string d5 = "remarkable manner, casting off the traditional exoskeleton of their";
  std::string d6 = "crab-like ancestors in favor of a dense enamel-like substance that";
  std::string d7 = "encases their bodies, providing them with unparalleled protection and";
  std::string d8 = "adaptability in their harsh environment.";
  core_->DrawText("font.png", d1,   0.0f);
  core_->DrawText("font.png", d2,  20.0f);
  core_->DrawText("font.png", d3,  40.0f);
  core_->DrawText("font.png", d4,  60.0f);
  core_->DrawText("font.png", d5,  80.0f);
  core_->DrawText("font.png", d6, 100.0f);
  core_->DrawText("font.png", d7, 120.0f);
  core_->DrawText("font.png", d8, 140.0f);
}
