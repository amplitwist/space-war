#include "inc/component.hpp"

#include "inc/actor.hpp"

Component::Component(Actor *owner, i32 updateOrder)
: mOwner{owner}
, mUpdateOrder{updateOrder}
{
  mOwner->AddComponent(this);
}

Component::~Component()
{
  mOwner->RemoveComponent(this);
}
