#pragma once

namespace MythicEngine::engine::system::keyboard {
class IKeyboardHandler {
public:
  IKeyboardHandler() = default;
  virtual ~IKeyboardHandler() = default;

  IKeyboardHandler(const IKeyboardHandler &) = default;
  IKeyboardHandler &operator=(const IKeyboardHandler &) = default;

  IKeyboardHandler(IKeyboardHandler &&) noexcept = default;
  IKeyboardHandler &operator=(IKeyboardHandler &&) noexcept = default;

  virtual void OnKeyPressedEvent(unsigned char) noexcept = 0;
  virtual void OnKeyReleasedEvent(unsigned char) noexcept = 0;
  virtual void OnCharEvent(char) noexcept = 0;
};
} // namespace MythicEngine::engine::system::keyboard