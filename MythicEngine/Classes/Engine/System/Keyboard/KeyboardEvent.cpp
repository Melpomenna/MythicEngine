#include "KeyboardEvent.h"

#include <Support/Utils/Utils.h>

namespace MythicEngine::engine::system::keyboard {
KeyboardEvent::KeyboardEvent() : event_(Event::Invalid), code_(0) {}

KeyboardEvent::KeyboardEvent(EventType type, KeycodeType code) noexcept
    : event_(type), code_(code) {}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
KeyboardEvent::IsPress() const noexcept {
  return event_ == Event::Press;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
KeyboardEvent::IsRelease() const noexcept {
  return event_ == Event::Release;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
KeyboardEvent::IsValid() const noexcept {
  return event_ != Event::Invalid;
}

__NODISCARD__ auto MYTHIC_ENGINE_WIN_API KeyboardEvent::GetCode() const noexcept
    -> KeycodeType {
  return code_;
}
} // namespace MythicEngine::engine::system::keyboard
