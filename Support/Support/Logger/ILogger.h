#pragma once

#include <Windows.h>
#include <string_view>

#include <Support/Utils/Utils.h>

namespace MythicEngine::support::logger {
class MYTHIC_ENGINE_EXPORT ILogger {
public:
  virtual ~ILogger() = default;
  virtual void Log(DWORD msg, LPARAM lp, WPARAM wp) const = 0;
  virtual void Warning(DWORD msg, LPARAM lp, WPARAM wp) const = 0;
  virtual void Error(DWORD msg, LPARAM lp, WPARAM wp) const = 0;
  virtual void Log(std::string_view) const = 0;
  virtual void Error(std::string_view) const = 0;
  virtual void Warning(std::string_view) const = 0;
};
} // namespace MythicEngine::support::logger