#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <memory>
#include <string>

#include "core.hpp"
#include "types.hpp"

class State
{
  friend class System;

public:
  virtual ~State() = default;

  virtual void Startup();
  virtual void Cleanup();

  virtual void Update(f32 dt);
  virtual void Render(f32 frac);

protected:
  System *system_;
  Core *core_;
};

class System
{
public:
  bool Startup(std::string title, i32 width, i32 height);
  void Cleanup();

  void Run(std::unique_ptr<State> state);

  void ChangeState(std::unique_ptr<State> state);

  void Quit();

private:
  Core core_;

  bool running_{true};

  std::unique_ptr<State> current_state_;
  std::unique_ptr<State> next_state_;
};

#define QUIT_ON_FAILURE(x) \
do { if (!(x)) { system_->Quit(); return; } } while (0)

#endif //SYSTEM_HPP
