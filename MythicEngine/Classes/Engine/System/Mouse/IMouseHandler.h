#pragma once

namespace MythicEngine::engine::system::mouse {
class IMouseHandler {
public:
  IMouseHandler() = default;
  virtual ~IMouseHandler() = default;

  IMouseHandler(const IMouseHandler &) = default;
  IMouseHandler &operator=(const IMouseHandler &) = default;

  IMouseHandler(IMouseHandler &&) noexcept = default;
  IMouseHandler &operator=(IMouseHandler &&) noexcept = default;

  virtual void OnMouseMoveEvent(int, int) noexcept = 0;
  virtual void OnLeftPressedEvent(int, int) noexcept = 0;
  virtual void OnLeftReleasedEvent(int, int) noexcept = 0;
  virtual void OnRightPressedEvent(int, int) noexcept = 0;
  virtual void OnRightReleasedEvent(int, int) noexcept = 0;
  virtual void OnWheelUpEvent(int, int) noexcept = 0;
  virtual void OnWheelDownEvent(int, int) noexcept = 0;
  virtual void OnMouseLeaveEvent() noexcept = 0;
  virtual void OnMouseEnterEvent() noexcept = 0;
};
} // namespace MythicEngine::engine::system::mouse