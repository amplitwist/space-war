#include "inc/system.hpp"

void State::Startup()
{
}

void State::Cleanup()
{
}

void State::Update(f32)
{
}

void State::Render(f32)
{
}

bool System::Startup(std::string title, i32 width, i32 height)
{
  return core_.Startup(title, width, height);
}

void System::Cleanup()
{
  core_.Cleanup();
}

void System::Run(std::unique_ptr<State> state)
{
  ChangeState(std::move(state));

  i64 last_time{core_.Time()};
  i64 accumulated_time{0};
  i64 time_step{40'000}; //25 updates per second

  while (running_ && !core_.WindowClosed())
  {
    if (next_state_)
    {
      if (current_state_)
        current_state_->Cleanup();

      current_state_ = std::move(next_state_);
      current_state_->system_ = this;
      current_state_->core_ = &core_;
      current_state_->Startup();

      last_time = core_.Time();
      accumulated_time = 0;
    }
    else
    {
      i64 current_time{core_.Time()};
      accumulated_time += current_time - last_time;
      last_time = current_time;

      //todo: limit the number of iterations
      while (accumulated_time >= time_step)
      {
        core_.Service();
        current_state_->Update((f32)((f64)time_step / 1'000'000.0));
        accumulated_time -= time_step;
      }

      current_state_->Render((f32)((f64)accumulated_time / (f64)time_step));
      core_.Render();
    }
  }

  current_state_->Cleanup();
}

void System::ChangeState(std::unique_ptr<State> state)
{
  next_state_ = std::move(state);
}

void System::Quit()
{
  running_ = false;
}
