#pragma once
#include <cstdint>

namespace MythicEngine::engine::system::keyboard {
enum class Event : uint_fast32_t { Press, Release, Invalid };

class KeyboardEvent final {
  using EventType = Event;
  using KeycodeType = unsigned char;

public:
  KeyboardEvent();
  explicit KeyboardEvent(EventType, KeycodeType) noexcept;
  ~KeyboardEvent() = default;

  bool IsPress() const noexcept;
  bool IsRelease() const noexcept;
  bool IsValid() const noexcept;
  KeycodeType GetCode() const noexcept;

private:
  EventType event_;
  KeycodeType code_;
};
} // namespace MythicEngine::engine::system::keyboard
