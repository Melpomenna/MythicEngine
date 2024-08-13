#pragma once
#include <cstdint>

namespace MythicEngine::engine::system::mouse {

class IMouse;

enum class Event : uint_fast32_t {
  LPress,
  LRelease,
  RPress,
  RRelease,
  WheelUp,
  WheelDown,
  Move,
  Enter,
  Leave,
  Invalid
};

struct MousePosition final {
  int x;
  int y;
};

class MouseEvent final {
  using EventType = Event;

public:
  MouseEvent() noexcept;
  explicit MouseEvent(EventType, bool, bool, int, int) noexcept;
  explicit MouseEvent(EventType, const IMouse &) noexcept;
  ~MouseEvent() = default;

  bool IsValid() const noexcept;
  EventType GetType() const noexcept;
  MousePosition GetPos() const noexcept;
  int GetPosX() const noexcept;
  int GetPosY() const noexcept;
  bool LeftIsPressed() const noexcept;
  bool RightIsPressed() const noexcept;

private:
  int x_ = 0, y_ = 0;
  bool leftIsPressed_ = false, rightPressed_ = false;
  EventType type_;
};
} // namespace MythicEngine::engine::system::mouse
