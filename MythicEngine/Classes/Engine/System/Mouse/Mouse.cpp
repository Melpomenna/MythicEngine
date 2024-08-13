#include "Mouse.h"

#include <Windows.h>

#include <Engine/System/Mouse/IMouseHandler.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::engine::system::mouse {
__NODISCARD__ MousePosition MYTHIC_ENGINE_WIN_API
Mouse::GetPos() const noexcept {
  return position_;
}

__NODISCARD__ int MYTHIC_ENGINE_WIN_API Mouse::GetX() const noexcept {
  return position_.x;
}

__NODISCARD__ int MYTHIC_ENGINE_WIN_API Mouse::GetY() const noexcept {
  return position_.y;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API Mouse::LeftIsPressed() const noexcept {
  return leftIsPressed_;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
Mouse::RightIsPressed() const noexcept {
  return rightIsPressed_;
}

__NODISCARD__ MouseEvent MYTHIC_ENGINE_WIN_API Mouse::Read() noexcept {
  if (!IsEmpty()) {
    const auto e = eventsBuffer_.front();
    eventsBuffer_.pop();
    return e;
  }

  return {};
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API Mouse::IsEmpty() const noexcept {
  return eventsBuffer_.empty();
}

void MYTHIC_ENGINE_WIN_API Mouse::Flush() noexcept { eventsBuffer_ = {}; }

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API Mouse::IsInWindow() const noexcept {
  return isInWindow_;
}

void MYTHIC_ENGINE_WIN_API Mouse::OnMouseMove(int x, int y) noexcept {
  position_ = {x, y};
  eventsBuffer_.emplace(Event::Move, *this);
  Post([x, y](auto h) { h->OnMouseMoveEvent(x, y); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnLeftPressed(int x, int y) noexcept {
  leftIsPressed_ = true;
  eventsBuffer_.emplace(Event::LPress, *this);
  Post([x, y](auto h) { h->OnLeftPressedEvent(x, y); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnLeftReleased(int x, int y) noexcept {
  leftIsPressed_ = false;
  eventsBuffer_.emplace(Event::LRelease, *this);
  Post([x, y](auto h) { h->OnLeftReleasedEvent(x, y); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnRightPressed(int x, int y) noexcept {
  rightIsPressed_ = true;
  eventsBuffer_.emplace(Event::RPress, *this);
  Post([x, y](auto h) { h->OnRightPressedEvent(x, y); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnRightReleased(int x, int y) noexcept {
  rightIsPressed_ = false;
  eventsBuffer_.emplace(Event::RRelease, *this);
  Post([x, y](auto h) { h->OnRightReleasedEvent(x, y); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnWheelUp(int x, int y) noexcept {
  eventsBuffer_.emplace(Event::WheelUp, *this);
  Post([x, y](auto h) { h->OnWheelUpEvent(x, y); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnWheelDown(int x, int y) noexcept {
  eventsBuffer_.emplace(Event::WheelDown, *this);
  Post([x, y](auto h) { h->OnWheelDownEvent(x, y); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnWheel(int x, int y, int delta) noexcept {
  wheelDelta_ += delta;
  while (wheelDelta_ >= WHEEL_DELTA) {
    wheelDelta_ -= WHEEL_DELTA;
    OnWheelUp(x, y);
  }
  while (wheelDelta_ <= -WHEEL_DELTA) {
    wheelDelta_ += WHEEL_DELTA;
    OnWheelDown(x, y);
  }
}

void MYTHIC_ENGINE_WIN_API Mouse::OnMouseLeave() noexcept {
  isInWindow_ = false;

  eventsBuffer_.emplace(Event::Leave, *this);
  Post([](auto h) { h->OnMouseLeaveEvent(); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::OnMouseEnter() noexcept {
  isInWindow_ = true;

  eventsBuffer_.emplace(Event::Enter, *this);
  Post([](auto h) { h->OnMouseEnterEvent(); });
  TrimBuffer();
}

void MYTHIC_ENGINE_WIN_API Mouse::TrimBuffer() noexcept {
  while (eventsBuffer_.size() > bufferSize) {
    eventsBuffer_.pop();
  }
}

} // namespace MythicEngine::engine::system::mouse
