#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>

#include "core.hpp"

class Animation
{
public:
  Animation(std::string texture, f32 frame_rate);

  void AddFrame(const Rect &frame);

  void Update(f32 dt);

  Sprite GetSprite();

private:
  std::string texture_;
  f32 frame_rate_;
  std::vector<Rect> frames_;
  f32 time_;
};

#endif //ANIMATION_HPP
