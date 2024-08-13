#include "Keyboard.h"

#include <Engine/System/Keyboard/IKeyboardHandler.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::engine::system::keyboard {
__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
  return keyStates_[keycode];
}

__NODISCARD__ KeyboardEvent MYTHIC_ENGINE_WIN_API Keyboard::ReadKey() noexcept {
  if (!KeyIsEmpty()) {
    const auto e = keyBuffer_.front();
    keyBuffer_.pop();
    return e;
  }

  return {};
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API Keyboard::KeyIsEmpty() const noexcept {
  return keyBuffer_.empty();
}

void MYTHIC_ENGINE_WIN_API Keyboard::FlushKey() noexcept {
  if (!KeyIsEmpty()) {
    keyBuffer_.pop();
  }
}

__NODISCARD__ char MYTHIC_ENGINE_WIN_API Keyboard::ReadChar() noexcept {
  if (!CharIsEmpty()) {
    const auto sym = charBuffer_.front();
    charBuffer_.pop();
    return sym;
  }

  return 0;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
Keyboard::CharIsEmpty() const noexcept {
  return charBuffer_.empty();
}

void MYTHIC_ENGINE_WIN_API Keyboard::FlushChar() noexcept {
  if (!CharIsEmpty()) {
    charBuffer_.pop();
  }
}

void MYTHIC_ENGINE_WIN_API Keyboard::Flush() noexcept {
  FlushKey();
  FlushChar();
}

void MYTHIC_ENGINE_WIN_API Keyboard::EnableAutorepeat() noexcept {
  autorepeatEnabled_ = true;
}

void MYTHIC_ENGINE_WIN_API Keyboard::DisableAutorepeat() noexcept {
  autorepeatEnabled_ = false;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
Keyboard::AutorepeatIsEnabled() const noexcept {
  return autorepeatEnabled_;
}

void MYTHIC_ENGINE_WIN_API
Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
  keyStates_[keycode] = true;
  keyBuffer_.emplace(Event::Press, keycode);
  Post([keycode](auto h) { h->OnKeyPressedEvent(keycode); });
  TrimBuffer(keyBuffer_);
}

void MYTHIC_ENGINE_WIN_API
Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
  keyStates_[keycode] = false;
  keyBuffer_.emplace(Event::Release, keycode);
  Post([keycode](auto h) { h->OnKeyReleasedEvent(keycode); });
  TrimBuffer(keyBuffer_);
}

void MYTHIC_ENGINE_WIN_API Keyboard::OnChar(char sym) noexcept {
  charBuffer_.emplace(sym);
  Post([sym](auto h) { h->OnCharEvent(sym); });
  TrimBuffer(charBuffer_);
}

void MYTHIC_ENGINE_WIN_API Keyboard::ClearState() noexcept {
  keyStates_.reset();
}

template <class T>
void MYTHIC_ENGINE_WIN_API
Keyboard::TrimBuffer(std::queue<T> &buffer) noexcept {
  while (buffer.size() > bufferSize) {
    buffer.pop();
  }
}

} // namespace MythicEngine::engine::system::keyboard
