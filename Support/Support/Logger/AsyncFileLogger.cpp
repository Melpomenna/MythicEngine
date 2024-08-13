#include <pch.h>
#include "AsyncFileLogger.h"

#include <Support/Utils/Exceptions.h>
#include <Support/Utils/Utils.h>

#include <iomanip>
#include <sstream>

#define MYTHIC_ENGINE_REGISTER_MESSAGE(__NAME__) {__NAME__, #__NAME__}

namespace {
inline constexpr auto COL_WIDTH = 10;
}

namespace MythicEngine::support::logger {
void MYTHIC_ENGINE_WIN_API
AsyncFileLogger::AsyncFileLoggerObserver::UpdateBuffer(
    std::string buffer) const noexcept {
  buffer_.push(buffer);
}

__NODISCARD__ std::string MYTHIC_ENGINE_WIN_API
AsyncFileLogger::AsyncFileLoggerObserver::GetBuffer() const noexcept {
  if (IsEmpty())
    return "";
  const auto buffer = buffer_.front();
  buffer_.pop();
  return buffer;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API
AsyncFileLogger::AsyncFileLoggerObserver::IsEmpty() const noexcept {
  return buffer_.empty();
}

AsyncFileLogger::AsyncFileLogger(std::string_view fileName)
    : observer_(std::make_shared<AsyncFileLoggerObserver>()),
      fileName_(fileName),
      messages_({
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_CREATE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_DESTROY),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_MOVE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_SIZE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_ACTIVATE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_SETFOCUS),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_ENTERSIZEMOVE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_DWMNCRENDERINGCHANGED),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_NCMOUSELEAVE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_IME_NOTIFY),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_EXITSIZEMOVE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_IME_SETCONTEXT),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_PRINT),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_PRINTCLIENT),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_DEVICECHANGE),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_CAPTURECHANGED),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_MOVING),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_SIZING),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_SETICON),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_GETICON),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_STYLECHANGED),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_STYLECHANGING),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_ENTERMENULOOP),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_EXITMENULOOP),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_HOTKEY),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_POWERBROADCAST),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_SETHOTKEY),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_POWER),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_NOTIFY),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_HELP),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_DROPFILES),
          MYTHIC_ENGINE_REGISTER_MESSAGE(WM_TCARD),
      }),
      stopThread_(false) {
  HANDLE hFile =
      CreateFileA(fileName_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                  OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
  CloseHandle(hFile);
  stream_.open(fileName_);
#ifndef NDEBUG
  if (!stream_.is_open()) {
    throw utils::Exception(
        __LINE__, __FILE__,
        std::string("Cannot open log file:" + fileName_).c_str());
  }
#else
  if (!stream_.is_open()) {
    return;
  }
#endif
  std::thread worker{&AsyncFileLogger::Write, this};
  thread_ = std::move_if_noexcept(worker);
  thread_.detach();
}

AsyncFileLogger::~AsyncFileLogger() {
  stopThread_ = true;
  stream_.close();
}

void MYTHIC_ENGINE_WIN_API AsyncFileLogger::Log(DWORD msg, LPARAM lp,
                                                WPARAM wp) const {
  observer_->UpdateBuffer(GetLogMessage(msg, lp, wp, "[LOG]:"));
}

void MYTHIC_ENGINE_WIN_API AsyncFileLogger::Warning(DWORD msg, LPARAM lp,
                                                    WPARAM wp) const {
  observer_->UpdateBuffer(GetLogMessage(msg, lp, wp, "[WARNING]:"));
}

void MYTHIC_ENGINE_WIN_API AsyncFileLogger::Error(DWORD msg, LPARAM lp,
                                                  WPARAM wp) const {
  observer_->UpdateBuffer(GetLogMessage(msg, lp, wp, "[ERROR]:"));
}

void MYTHIC_ENGINE_WIN_API AsyncFileLogger::Log(std::string_view data) const {
  observer_->UpdateBuffer(std::string("[LOG]:") + data.data() + "\n");
}

void MYTHIC_ENGINE_WIN_API AsyncFileLogger::Error(std::string_view data) const {
  observer_->UpdateBuffer(std::string("[ERROR]:") + data.data() + "\n");
}

void MYTHIC_ENGINE_WIN_API
AsyncFileLogger::Warning(std::string_view data) const {
  observer_->UpdateBuffer(std::string("[WARNING]:") + data.data() + "\n");
}

__NODISCARD__ std::string MYTHIC_ENGINE_WIN_API AsyncFileLogger::GetLogMessage(
    DWORD msg, LPARAM lp, WPARAM wp, std::string desc) const noexcept {
  const auto messagePos = messages_.find(msg);
  std::ostringstream oss;
  if (messagePos != messages_.cend()) {
    oss << std::left << std::setw(COL_WIDTH) << desc << messagePos->second
        << std::right;
  } else {
    std::ostringstream padss;
    padss << desc << " Unknown message: 0x" << std::hex << msg;
    oss << std::left << std::setw(COL_WIDTH) << padss.str() << std::right;
  }

  oss << "		LP: 0x" << std::hex << std::setfill('0') << std::setw(8)
      << lp;
  oss << "		WP: 0x" << std::hex << std::setfill('0') << std::setw(8)
      << wp << "\n";

  return oss.str();
}

void MYTHIC_ENGINE_WIN_API AsyncFileLogger::Write() {
  while (!stopThread_) {
    const auto message = observer_->GetBuffer();
    if (message.empty()) {
      std::this_thread::yield();
    }
    stream_ << message;
  }

  while (!observer_->IsEmpty()) {
    const auto message = observer_->GetBuffer();
    stream_ << message;
  }
}
} // namespace MythicEngine::support::logger
