#include "inc/animation.hpp"

#include <cmath>

Animation::Animation(std::string texture, f32 frame_rate)
: texture_{texture}
, frame_rate_{frame_rate}
, time_{0.0f}
{
}

void Animation::AddFrame(const Rect &frame)
{
  frames_.push_back(frame);
}

void Animation::Update(f32 dt)
{
  time_ += dt;
}

Sprite Animation::GetSprite()
{
  //TODO: use frame_rate_
  return {
    texture_,
    frames_[static_cast<u64>(std::floor(time_ / 0.25f)) % frames_.size()]
  };
}
