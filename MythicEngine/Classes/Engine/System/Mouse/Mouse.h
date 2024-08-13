#pragma once

#include <Engine/System/Mouse/IMouse.h>
#include <Support/Utils/NonCopy.h>

#include <queue>

namespace MythicEngine::engine::system::mouse {
class Mouse final : public IMouse, public support::utils::NonCopy {
public:
  Mouse() = default;

  MousePosition GetPos() const noexcept override;
  int GetX() const noexcept override;
  int GetY() const noexcept override;
  bool LeftIsPressed() const noexcept override;
  bool RightIsPressed() const noexcept override;
  MouseEvent Read() noexcept override;
  bool IsEmpty() const noexcept override;
  void Flush() noexcept override;
  bool IsInWindow() const noexcept override;

private:
  void OnMouseMove(int, int) noexcept override;
  void OnLeftPressed(int, int) noexcept override;
  void OnLeftReleased(int, int) noexcept override;
  void OnRightPressed(int, int) noexcept override;
  void OnRightReleased(int, int) noexcept override;
  void OnWheelUp(int, int) noexcept override;
  void OnWheelDown(int, int) noexcept override;
  void OnWheel(int, int, int) noexcept override;
  void OnMouseLeave() noexcept override;
  void OnMouseEnter() noexcept override;
  void TrimBuffer() noexcept;

private:
  static constexpr unsigned int bufferSize = 16u;
  bool isInWindow_{false};
  int wheelDelta_;
  MousePosition position_{0, 0};
  bool leftIsPressed_{false}, rightIsPressed_{false};
  std::queue<MouseEvent> eventsBuffer_;
};
} // namespace MythicEngine::engine::system::mouse