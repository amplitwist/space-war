#ifndef ORBIT_COMPONENT_HPP
#define ORBIT_COMPONENT_HPP

#include "component.hpp"

#include <cmath>
#include <iostream>

class OrbitComponent : public Component
{
public:
  OrbitComponent(Actor *owner, Actor *primaryBody, f32 radius)
  : Component{owner}
  , mPrimaryBody{primaryBody}
  , mRadius{radius}
  , mAngle{0.0f}
  {
  }

  void Update(f32 deltaTime) override
  {
    mAngle -= 0.001f * deltaTime;
    Vector2 position{mPrimaryBody->GetPosition()};
    position.x += std::cos(mAngle) * mRadius;
    position.y += std::sin(mAngle) * mRadius;
    std::cout << "(" << position.x << ", " << position.y << ")\n";
    mOwner->SetPosition(position);
  }

private:
  Actor *mPrimaryBody;
  f32 mRadius;
  f32 mAngle{0.0f};
};

#endif //ORBIT_COMPONENT
