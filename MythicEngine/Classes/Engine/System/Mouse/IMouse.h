#pragma once

#include <Engine/System/Mouse/IMouseDispatcher.h>
#include <Engine/System/Mouse/MouseEvent.h>

namespace MythicEngine::engine::system::mouse {
class IMouse : public IMouseDispatcher {
  friend class MainWindow;

public:
  IMouse() = default;
  ~IMouse() override = default;

  virtual MousePosition GetPos() const noexcept = 0;
  virtual int GetX() const noexcept = 0;
  virtual int GetY() const noexcept = 0;
  virtual bool LeftIsPressed() const noexcept = 0;
  virtual bool RightIsPressed() const noexcept = 0;
  virtual MouseEvent Read() noexcept = 0;
  virtual bool IsEmpty() const noexcept = 0;
  virtual void Flush() noexcept = 0;
  virtual bool IsInWindow() const noexcept = 0;

  void AddHandler(const IMouseHandlerPtr &) override;
  void RemoveHandler(const IMouseHandlerPtr &) override;
  void Post(const HandleFuncType &) override;

protected:
  virtual void OnMouseMove(int, int) noexcept = 0;
  virtual void OnLeftPressed(int, int) noexcept = 0;
  virtual void OnLeftReleased(int, int) noexcept = 0;
  virtual void OnRightPressed(int, int) noexcept = 0;
  virtual void OnRightReleased(int, int) noexcept = 0;
  virtual void OnWheelUp(int, int) noexcept = 0;
  virtual void OnWheelDown(int, int) noexcept = 0;
  virtual void OnWheel(int, int, int) noexcept = 0;
  virtual void OnMouseLeave() noexcept = 0;
  virtual void OnMouseEnter() noexcept = 0;
};

} // namespace MythicEngine::engine::system::mouse