#include "Window.h"

#include <Core/Functions/FunctionRunnable.h>
#include <Core/Memory/UniquePtr.h>
#include <Core/Threads/ThreadPool.h>
#include <Engine/System/Keyboard/Keyboard.h>
#include <Engine/System/Mouse/Mouse.h>
#include <Render/DirectX11/Render.h>
#include <Support/Logger/AsyncFileLogger.h>
#include <Support/Logger/MessageLogger.h>
#include <Support/Utils/Utils.h>
#include <resource.h>

#include <sstream>

namespace {

namespace memory = MythicEngine::core::memory;
namespace threads = MythicEngine::core::threads;
namespace functions = MythicEngine::core::functions;
namespace logger = MythicEngine::support::logger;

using DebugLoggerPtr = memory::UniquePtr<logger::ILogger>;
const DebugLoggerPtr Logger =
    memory::MakeUnique<logger::ILogger, logger::MessageLogger>();
const DebugLoggerPtr AsyncFileLogger =
    memory::MakeUnique<logger::ILogger, logger::AsyncFileLogger>(
        "./Logs/log.txt");

threads::StaticThreadPool Pool = threads::StaticThreadPool();

} // namespace

namespace MythicEngine::engine {
class MainWindow::MainWindowImpl final : public NonCopy {
public:
  static support::utils::LPWStringWrapper GetName() noexcept;
  static HINSTANCE GetInstance() noexcept;

  MainWindowImpl(MainWindowImpl &&) noexcept = default;
  MainWindowImpl &operator=(MainWindowImpl &&) noexcept = default;

private:
  MainWindowImpl() noexcept;
  ~MainWindowImpl();
  static inline support::utils::LPWStringWrapper wndClassName{
      "Direct3D Engine Window"};
  static MainWindowImpl wnd_;
  HINSTANCE hInstance_;
};

MainWindow::MainWindowImpl MainWindow::MainWindowImpl::wnd_;

MainWindow::MainWindowImpl::MainWindowImpl() noexcept
    : hInstance_(GetModuleHandle(nullptr)) {
  WNDCLASSEX wc;
  wc.cbSize = sizeof(wc);
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = HandleMessageSetup;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetInstance();
  wc.hIcon = static_cast<HICON>(
      LoadImage(hInstance_, MAKEINTRESOURCE(APP_ICON), IMAGE_ICON, 32, 32, 0));
  wc.hCursor = nullptr;
  wc.hbrBackground = nullptr;
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = GetName();
  wc.hIconSm = static_cast<HICON>(
      LoadImage(hInstance_, MAKEINTRESOURCE(APP_ICON), IMAGE_ICON, 16, 16, 0));
  RegisterClassEx(&wc);
}

MainWindow::MainWindowImpl::~MainWindowImpl() {
  UnregisterClass(wndClassName, GetInstance());
}

__NODISCARD__ support::utils::LPWStringWrapper MYTHIC_ENGINE_WIN_API
MainWindow::MainWindowImpl::GetName() noexcept {
  return wndClassName;
}

__NODISCARD__ HINSTANCE MYTHIC_ENGINE_WIN_API
MainWindow::MainWindowImpl::GetInstance() noexcept {
  return wnd_.hInstance_;
}

MainWindow::MainWindow(unsigned width, unsigned height, const char *name)
    : width_(width), height_(height), name_(name),
      keyboard_(core::memory::MakeOwn<system::keyboard::Keyboard>()),
      mouse_(core::memory::MakeOwn<system::mouse::Mouse>()),
      timer_(core::memory::MakeOwn<core::time::Timer>()) {
  RECT wr;
  wr.left = 100;
  wr.right = static_cast<LONG>(width) + wr.left;
  wr.top = 100;
  wr.bottom = static_cast<LONG>(height) + wr.top;

  if (AdjustWindowRect(
          &wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_EX_ACCEPTFILES,
          FALSE) == 0) {
    throw CHWND_EXCEPTION_LAST_ERROR();
  }

  hWnd_ = CreateWindow(
      MainWindowImpl::GetName(), name_,
      WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_EX_ACCEPTFILES,
      CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
      nullptr, nullptr, MainWindowImpl::GetInstance(), this);

  if (nullptr == hWnd_) {
    throw CHWND_EXCEPTION_LAST_ERROR();
  }

  render::directx11::DirectX11Render::ResolveDirectX11Render(timer_, hWnd_);

  SetWindowTextA(hWnd_, name_);
  ShowWindow(hWnd_, SW_SHOWDEFAULT);
}

MainWindow::~MainWindow() {
  Pool.SafeJoin();
  DestroyWindow(hWnd_);
}

__NODISCARD__ LRESULT MYTHIC_ENGINE_WIN_API MainWindow::HandleMessageSetup(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
  if (msg == WM_NCCREATE) {
    const CREATESTRUCTW *const pCreate =
        reinterpret_cast<CREATESTRUCTW *>(lParam);
    MainWindow *const pWnd = static_cast<MainWindow *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
    SetWindowLongPtr(
        hWnd, GWLP_WNDPROC,
        reinterpret_cast<LONG_PTR>(&MainWindow::HandleMessageThunk));
    return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

__NODISCARD__ LRESULT MYTHIC_ENGINE_WIN_API MainWindow::HandleMessage(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
  AsyncFileLogger->Log(msg, lParam, wParam);
  switch (msg) {
  case WM_CLOSE: {
    const auto logData = "Exit from application";
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    PostQuitMessage(0);
  }
    return 0;
  case WM_KILLFOCUS: {
    const auto logData = "On Focus Kill";
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    keyboard_->ClearState();
  } break;
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN: {
    if (wParam == VK_ESCAPE) {
      const auto logData = "Pressed escape";
      if (AsyncFileLogger)
        AsyncFileLogger->Log(logData);
      if (Logger)
        Logger->Log(logData);
      PostQuitMessage(0);
      return 0;
    }
    if (keyboard_->AutorepeatIsEnabled() || !(lParam & 0x40000000)) {
      const auto logData =
          std::string("Pressed key:") + static_cast<char>(wParam);
      if (AsyncFileLogger)
        AsyncFileLogger->Log(logData);
      if (Logger)
        Logger->Log(logData);
      keyboard_->OnKeyPressed(static_cast<unsigned char>(wParam));
    }
  } break;
  case WM_KEYUP:
  case WM_SYSKEYUP: {
    const auto logData = std::string("KeyUp:") + static_cast<char>(wParam);
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    keyboard_->OnKeyReleased(static_cast<unsigned char>(wParam));
  } break;
  case WM_CHAR: {
    const auto logData =
        std::string("On char pressed:") + static_cast<char>(wParam);
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    keyboard_->OnChar(static_cast<char>(wParam));
  } break;
  case WM_MOUSEMOVE: {
    const POINTS pt = MAKEPOINTS(lParam);
    if (pt.x >= 0 && pt.x < static_cast<SHORT>(width_) && pt.y >= 0 &&
        pt.y < static_cast<SHORT>(height_)) {
      {
        const auto logData = std::string("Mouse move outside window:") +
                             " x:" + std::to_string(pt.x) +
                             " y:" + std::to_string(pt.y);
        if (AsyncFileLogger)
          AsyncFileLogger->Log(logData);
        if (Logger)
          Logger->Log(logData);
      }
      mouse_->OnMouseMove(pt.x, pt.y);
      if (!mouse_->IsInWindow()) {
        SetCapture(hWnd);
        {
          const auto logData = "Mouse enter in window";
          if (AsyncFileLogger)
            AsyncFileLogger->Log(logData);
          if (Logger)
            Logger->Log(logData);
        }
        mouse_->OnMouseEnter();
      }
    } else {
      if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
        const auto logData = std::string("Mouse move:") +
                             " x:" + std::to_string(pt.x) +
                             " y:" + std::to_string(pt.y);
        if (AsyncFileLogger)
          AsyncFileLogger->Log(logData);
        if (Logger)
          Logger->Log(logData);
        mouse_->OnMouseMove(pt.x, pt.y);
      } else {
        ReleaseCapture();
        const auto logData = "Mouse leave from window";
        if (AsyncFileLogger)
          AsyncFileLogger->Log(logData);
        if (Logger)
          Logger->Log(logData);
        mouse_->OnMouseLeave();
      }
    }
  } break;
  case WM_LBUTTONDOWN: {
    const POINTS pt = MAKEPOINTS(lParam);
    const auto logData = std::string("Mouse left button pressed:") +
                         " x:" + std::to_string(pt.x) +
                         " y:" + std::to_string(pt.y);
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    mouse_->OnLeftPressed(pt.x, pt.y);
  } break;
  case WM_RBUTTONDOWN: {
    const POINTS pt = MAKEPOINTS(lParam);
    const auto logData = std::string("Mouse right button pressed:") +
                         " x:" + std::to_string(pt.x) +
                         " y:" + std::to_string(pt.y);
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    mouse_->OnRightPressed(pt.x, pt.y);
  } break;
  case WM_LBUTTONUP: {
    const POINTS pt = MAKEPOINTS(lParam);
    const auto logData = std::string("Mouse left button released:") +
                         " x:" + std::to_string(pt.x) +
                         " y:" + std::to_string(pt.y);
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    mouse_->OnLeftReleased(pt.x, pt.y);
  } break;
  case WM_RBUTTONUP: {
    const POINTS pt = MAKEPOINTS(lParam);
    const auto logData = std::string("Mouse right button released:") +
                         " x:" + std::to_string(pt.x) +
                         " y:" + std::to_string(pt.y);
    if (AsyncFileLogger)
      AsyncFileLogger->Log(logData);
    if (Logger)
      Logger->Log(logData);
    mouse_->OnRightReleased(pt.x, pt.y);
  } break;
  case WM_MOUSEWHEEL: {
    const POINTS pt = MAKEPOINTS(lParam);
    const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
    if (delta > 0) {
      const auto logData = std::string("Mouse wheel up:") +
                           " x:" + std::to_string(pt.x) +
                           " y:" + std::to_string(pt.y);
      if (AsyncFileLogger)
        AsyncFileLogger->Log(logData);
      if (Logger)
        Logger->Log(logData);
    } else {
      const auto logData = std::string("Mouse wheel down:") +
                           " x:" + std::to_string(pt.x) +
                           " y:" + std::to_string(pt.y);
      if (AsyncFileLogger)
        AsyncFileLogger->Log(logData);
      if (Logger)
        Logger->Log(logData);
    }
    mouse_->OnWheel(pt.x, pt.y, delta);
  } break;
  default:
    break;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

__NODISCARD__ LRESULT MYTHIC_ENGINE_WIN_API MainWindow::HandleMessageThunk(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
  MainWindow *const pWnd =
      reinterpret_cast<MainWindow *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
}

__NODISCARD__ LRESULT MYTHIC_ENGINE_WIN_API
MainWindow::RunLoop() const noexcept {
  MSG message{};
  LRESULT result = 1;
  try {
    while (result > 0) {
      if (const auto code = ProcessMessage(&message); code) {
        result = code.value();
      }
      timer_->Post();
    }
  } catch (const Exception &e) {
    MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    if (AsyncFileLogger)
      AsyncFileLogger->Error(message.message, message.lParam, message.wParam);
    result = -1;
  } catch (const std::exception &e) {
    if (AsyncFileLogger)
      AsyncFileLogger->Error(message.message, message.lParam, message.wParam);
    MessageBoxA(nullptr, e.what(), "[Standart Exception]",
                MB_OK | MB_ICONEXCLAMATION);
    result = -1;
  } catch (...) {
    if (AsyncFileLogger)
      AsyncFileLogger->Error(message.message, message.lParam, message.wParam);
    MessageBoxA(nullptr, "No details available", "Unknown Exception",
                MB_OK | MB_ICONEXCLAMATION);
    result = -1;
  }

  if (result == -1) {
    return -1;
  }
  return static_cast<LRESULT>(message.wParam);
}

__NODISCARD__ std::optional<LRESULT> MYTHIC_ENGINE_WIN_API
MainWindow::ProcessMessage(MSG *message) const noexcept {
  while (PeekMessage(message, nullptr, 0, 0, PM_REMOVE)) {
    if (message->message == WM_QUIT) {
      return message->wParam;
    }
    [[maybe_unused]] LRESULT translateResult = TranslateMessage(message);
    [[maybe_unused]] LRESULT dispatchResult = DispatchMessage(message);
  }
  return {};
}

MainWindow::Exception::Exception(int line, const char *file,
                                 HRESULT hr) noexcept
    : support::utils::Exception(line, file), hr_(hr) {}

__NODISCARD__ const char *MYTHIC_ENGINE_WIN_API
MainWindow::Exception::what() const noexcept {
  std::ostringstream oss;
  oss << GetType() << "\n"
      << "[Error Code]: " << GetErrorCode() << "\n"
      << "[Description]: " << GetErrorString() << "\n"
      << "[File]:" << GetFile() << "\n"
      << "[Line]:" << GetLine() << "\n"
      << "[Origin String]:" << GetOriginString();
  exceptionBuffer_ = oss.str();
  return exceptionBuffer_.c_str();
}

__NODISCARD__ const char *MYTHIC_ENGINE_WIN_API
MainWindow::Exception::GetType() const noexcept {
  return "[Window Exception]:";
}

__NODISCARD__ std::string_view MYTHIC_ENGINE_WIN_API
MainWindow::Exception::TranslateErrorCode(HRESULT hr) {
  char *pMsgBuf = nullptr;
  DWORD nMsgLen = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
  if (0 == nMsgLen) {
    return "Unidentified error code";
  }
  std::string_view errorString = pMsgBuf;
  LocalFree(pMsgBuf);
  return errorString;
}

__NODISCARD__ HRESULT MYTHIC_ENGINE_WIN_API
MainWindow::Exception::GetErrorCode() const noexcept {
  return hr_;
}

__NODISCARD__ std::string_view MYTHIC_ENGINE_WIN_API
MainWindow::Exception::GetErrorString() const noexcept {
  return TranslateErrorCode(hr_);
}

} // namespace MythicEngine::engine
