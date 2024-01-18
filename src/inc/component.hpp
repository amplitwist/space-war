#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "types.hpp"

class Component
{
public:
  Component(class Actor *owner, i32 updateOrder = 100);
  virtual ~Component();

  virtual void Update(f32 deltaTime) {};

  virtual void ProcessInput(const u8 *keyStates) {};

  i32 GetUpdateOrder() const { return mUpdateOrder; };

  class Actor *GetOwner() const { return mOwner; }

protected:
  class Actor *mOwner;
  i32 mUpdateOrder;
};

#endif //COMPONENT_HPP
