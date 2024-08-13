#pragma once

#include <Engine/System/Keyboard/IKeyboard.h>
#include <Support/Utils/NonCopy.h>

#include <bitset>
#include <queue>

namespace MythicEngine::engine::system::keyboard {
class Keyboard final : public IKeyboard, public support::utils::NonCopy {
public:
  Keyboard() = default;
  bool KeyIsPressed(unsigned char) const noexcept override;
  KeyboardEvent ReadKey() noexcept override;
  bool KeyIsEmpty() const noexcept override;
  void FlushKey() noexcept override;
  char ReadChar() noexcept override;
  bool CharIsEmpty() const noexcept override;
  void FlushChar() noexcept override;
  void Flush() noexcept override;
  void EnableAutorepeat() noexcept override;
  void DisableAutorepeat() noexcept override;
  bool AutorepeatIsEnabled() const noexcept override;

private:
  void OnKeyPressed(unsigned char) noexcept override;
  void OnKeyReleased(unsigned char) noexcept override;
  void OnChar(char) noexcept override;
  void ClearState() noexcept override;
  template <class T> static void TrimBuffer(std::queue<T> &) noexcept;

private:
  static constexpr unsigned int nKeys = 256u;
  static constexpr unsigned int bufferSize = 16u;
  bool autorepeatEnabled_ = false;
  std::bitset<nKeys> keyStates_;
  std::queue<KeyboardEvent> keyBuffer_;
  std::queue<char> charBuffer_;
};
} // namespace MythicEngine::engine::system::keyboard