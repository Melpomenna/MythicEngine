#pragma once

#include <Windows.h>

#include <Core/Memory/OwnPtr.h>
#include <Core/Time/Timer.h>
#include <Support/Utils/Exceptions.h>
#include <Support/Utils/LPWStringWrapper.h>
#include <Support/Utils/NonCopy.h>

#include <optional>

namespace MythicEngine::engine {

namespace system {
namespace keyboard {
class IKeyboard;
}

namespace mouse {
class IMouse;
}
} // namespace system

class MainWindow final : public support::utils::NonCopy {
  class MainWindowImpl;
  friend class MainWindowImpl;

  using KeyboardPtr = core::memory::OwnPtr<system::keyboard::IKeyboard>;
  using MousePtr = core::memory::OwnPtr<system::mouse::IMouse>;
  using BaseTimerPtr = core::memory::OwnPtr<core::time::Timer>;

public:
  class Exception final : public support::utils::Exception {
  public:
    explicit Exception(int line, const char *file, HRESULT hr) noexcept;
    const char *what() const noexcept override;
    const char *GetType() const noexcept override;
    static std::string_view TranslateErrorCode(HRESULT);
    std::string_view GetErrorString() const noexcept;
    HRESULT GetErrorCode() const noexcept;

  private:
    HRESULT hr_;
  };

  MainWindow(unsigned width, unsigned height, const char *name);
  ~MainWindow();

  MainWindow(MainWindow &&) noexcept = default;
  MainWindow &operator=(MainWindow &&) noexcept = default;

  LRESULT RunLoop() const noexcept;

private:
  static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam,
                                             LPARAM lParam) noexcept;
  static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam,
                                             LPARAM lParam) noexcept;
  LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam,
                        LPARAM lParam) noexcept;
  std::optional<LRESULT> ProcessMessage(MSG *) const noexcept;

private:
  unsigned width_;
  unsigned height_;
  support::utils::LPWStringWrapper name_;
  BaseTimerPtr timer_;
  KeyboardPtr keyboard_;
  MousePtr mouse_;
  HWND hWnd_;
};

#define CHWND_EXCEPTION(hr)                                                    \
  MythicEngine::engine::MainWindow::Exception(__LINE__, __FILE__, hr)

#define CHWND_EXCEPTION_LAST_ERROR()                                           \
  MythicEngine::engine::MainWindow::Exception(__LINE__, __FILE__,              \
                                              GetLastError())

} // namespace MythicEngine::engine