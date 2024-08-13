#pragma once

#include <Engine/System/Keyboard/IKeyboardDispatcher.h>
#include <Engine/System/Keyboard/KeyboardEvent.h>

namespace MythicEngine::engine::system::keyboard {
class IKeyboard : public IKeyboardDispatcher {
  friend class MainWindow;

public:
  IKeyboard() = default;
  ~IKeyboard() override = default;

  virtual bool KeyIsPressed(unsigned char) const noexcept = 0;
  virtual KeyboardEvent ReadKey() noexcept = 0;
  virtual bool KeyIsEmpty() const noexcept = 0;
  virtual void FlushKey() noexcept = 0;
  virtual char ReadChar() noexcept = 0;
  virtual bool CharIsEmpty() const noexcept = 0;
  virtual void FlushChar() noexcept = 0;
  virtual void Flush() noexcept = 0;
  virtual void EnableAutorepeat() noexcept = 0;
  virtual void DisableAutorepeat() noexcept = 0;
  virtual bool AutorepeatIsEnabled() const noexcept = 0;

  void AddHandler(const IKeyboardHandlerPtr &) override;
  void RemoveHandler(const IKeyboardHandlerPtr &) override;
  void Post(const HandleFuncType &) override;

protected:
  virtual void OnKeyPressed(unsigned char) noexcept = 0;
  virtual void OnKeyReleased(unsigned char) noexcept = 0;
  virtual void OnChar(char) noexcept = 0;
  virtual void ClearState() noexcept = 0;

protected:
  KeyboardEvent event_;
};
} // namespace MythicEngine::engine::system::keyboard
