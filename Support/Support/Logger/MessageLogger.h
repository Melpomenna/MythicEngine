#pragma once

#include <Support/Logger/ILogger.h>
#include <Support/Utils/Utils.h>
#include <Windows.h>
#include <string>
#include <unordered_map>

namespace MythicEngine::support::logger {

class MessageLogger final : public ILogger {
  using MessageLogPoolType = std::unordered_map<DWORD, std::string>;

public:
  MYTHIC_ENGINE_EXPORT MessageLogger();
  MYTHIC_ENGINE_EXPORT ~MessageLogger() override = default;

  MYTHIC_ENGINE_EXPORT void Log(DWORD msg, LPARAM lp, WPARAM wp) const override;
  MYTHIC_ENGINE_EXPORT void Warning(DWORD msg, LPARAM lp,
                                    WPARAM wp) const override;
  MYTHIC_ENGINE_EXPORT void Error(DWORD msg, LPARAM lp,
                                  WPARAM wp) const override;
  MYTHIC_ENGINE_EXPORT void Log(std::string_view) const override;
  MYTHIC_ENGINE_EXPORT void Error(std::string_view) const override;
  MYTHIC_ENGINE_EXPORT void Warning(std::string_view) const override;

private:
  MessageLogPoolType messages_;
};
} // namespace MythicEngine::support::logger