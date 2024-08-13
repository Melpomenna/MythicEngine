#include "MouseEvent.h"

#include <Engine/System/Mouse/IMouse.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::engine::system::mouse {
MouseEvent::MouseEvent() noexcept : type_(Event::Invalid) {}

MouseEvent::MouseEvent(EventType type, bool leftIsPressed, bool rightIsPressed,
                       int x, int y) noexcept
    : x_(x), y_(y), leftIsPressed_(leftIsPressed),
      rightPressed_(rightIsPressed), type_(type) {}

MouseEvent::MouseEvent(EventType type, const IMouse &mouse) noexcept
    : x_(mouse.GetX()), y_(mouse.GetY()), leftIsPressed_(mouse.LeftIsPressed()),
      rightPressed_(mouse.RightIsPressed()), type_(type) {}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API MouseEvent::IsValid() const noexcept {
  return type_ != Event::Invalid;
}

__NODISCARD__ auto MYTHIC_ENGINE_WIN_API MouseEvent::GetType() const noexcept
    -> EventType {
  return type_;
}

__NODISCARD__ MousePosition MYTHIC_ENGINE_WIN_API
MouseEvent::GetPos() const noexcept {
  return {x_, y_};
}

__NODISCARD__ int MYTHIC_ENGINE_WIN_API MouseEvent::GetPosX() const noexcept {
  return x_;
}

__NODISCARD__ int MYTHIC_ENGINE_WIN_API MouseEvent::GetPosY() const noexcept {
  return y_;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
MouseEvent::LeftIsPressed() const noexcept {
  return leftIsPressed_;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
MouseEvent::RightIsPressed() const noexcept {
  return rightPressed_;
}
} // namespace MythicEngine::engine::system::mouse
