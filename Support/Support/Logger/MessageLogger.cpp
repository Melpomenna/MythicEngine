#include <pch.h>
#include "MessageLogger.h"

#include <Support/Utils/Utils.h>

#include <iomanip>
#include <sstream>

#define MYTHIC_ENGINE_REGISTER_MESSAGE(__NAME__) {__NAME__, #__NAME__}

namespace {
inline constexpr auto COL_WIDTH = 10;
}

namespace MythicEngine::support::logger {
MessageLogger::MessageLogger()
    : messages_({
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
      }) {}

void MYTHIC_ENGINE_WIN_API MessageLogger::Log(DWORD msg, LPARAM lp,
                                              WPARAM wp) const {
  const auto messagePos = messages_.find(msg);
  std::ostringstream oss;
  if (messagePos != messages_.cend()) {
    oss << std::left << std::setw(COL_WIDTH) << "[INFO]:" << messagePos->second
        << std::right;
  } else {
    Warning(msg, lp, wp);
    return;
  }

  oss << "		LP: 0x" << std::hex << std::setfill('0') << std::setw(8)
      << lp;
  oss << "		WP: 0x" << std::hex << std::setfill('0') << std::setw(8)
      << wp << "\n";

  const auto str = oss.str();
  OutputDebugStringA(str.c_str());
}

void MYTHIC_ENGINE_WIN_API MessageLogger::Warning(DWORD msg, LPARAM lp,
                                                  WPARAM wp) const {

  std::ostringstream oss;
  std::ostringstream padss;
  padss << "[WARNING]: Unknown message: 0x" << std::hex << msg;
  oss << std::left << std::setw(COL_WIDTH) << padss.str() << std::right;

  oss << "		LP: 0x" << std::hex << std::setfill('0') << std::setw(8)
      << lp;
  oss << "		WP: 0x" << std::hex << std::setfill('0') << std::setw(8)
      << wp << "\n";
  const auto str = oss.str();
  OutputDebugStringA(str.c_str());
}

void MYTHIC_ENGINE_WIN_API MessageLogger::Error(DWORD msg, LPARAM lp,
                                                WPARAM wp) const {}

void MYTHIC_ENGINE_WIN_API MessageLogger::Log(std::string_view data) const {
  std::ostringstream oss;
  oss << std::left << std::setw(COL_WIDTH) << "[INFO]:" << data << std::right
      << "\n";
  OutputDebugStringA(oss.str().c_str());
}

void MYTHIC_ENGINE_WIN_API MessageLogger::Error(std::string_view data) const {
  std::ostringstream oss;
  oss << std::left << std::setw(COL_WIDTH) << "[ERROR]:" << data << std::right
      << "\n";
  OutputDebugStringA(oss.str().c_str());
}

void MYTHIC_ENGINE_WIN_API MessageLogger::Warning(std::string_view data) const {
  std::ostringstream oss;
  oss << std::left << std::setw(COL_WIDTH) << "[WARNING]:" << data << std::right
      << "\n";
  OutputDebugStringA(oss.str().c_str());
}

} // namespace MythicEngine::support::logger
