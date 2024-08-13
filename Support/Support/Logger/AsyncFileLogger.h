#pragma once

#include <Support/Logger/ILogger.h>
#include <Support/Utils/NonCopy.h>
#include <fstream>
#include <memory>
#include <queue>
#include <thread>
#include <unordered_map>

namespace MythicEngine::support::logger {
class AsyncFileLogger final : public ILogger, utils::NonCopy {
  using MessageLogPoolType = std::unordered_map<DWORD, std::string>;

  class AsyncFileLoggerObserver final {
  public:
    AsyncFileLoggerObserver() = default;
    ~AsyncFileLoggerObserver() = default;
    void UpdateBuffer(std::string) const noexcept;
    std::string GetBuffer() const noexcept;
    bool IsEmpty() const noexcept;

  private:
    mutable std::queue<std::string> buffer_;
  };

public:
  MYTHIC_ENGINE_EXPORT AsyncFileLogger() = default;
  MYTHIC_ENGINE_EXPORT explicit AsyncFileLogger(std::string_view fileName);
  MYTHIC_ENGINE_EXPORT ~AsyncFileLogger() override;

  MYTHIC_ENGINE_EXPORT AsyncFileLogger(AsyncFileLogger &&) noexcept = default;
  MYTHIC_ENGINE_EXPORT AsyncFileLogger &
  operator=(AsyncFileLogger &&) noexcept = default;

  MYTHIC_ENGINE_EXPORT void Log(DWORD msg, LPARAM lp, WPARAM wp) const override;
  MYTHIC_ENGINE_EXPORT void Warning(DWORD msg, LPARAM lp,
                                    WPARAM wp) const override;
  MYTHIC_ENGINE_EXPORT void Error(DWORD msg, LPARAM lp,
                                  WPARAM wp) const override;
  MYTHIC_ENGINE_EXPORT void Log(std::string_view) const override;
  MYTHIC_ENGINE_EXPORT void Error(std::string_view) const override;
  MYTHIC_ENGINE_EXPORT void Warning(std::string_view) const override;

private:
  void Write();
  std::string GetLogMessage(DWORD msg, LPARAM lp, WPARAM wp,
                            std::string desc) const noexcept;

private:
  std::shared_ptr<AsyncFileLoggerObserver> observer_;
  std::thread thread_;
  std::string buffer_;
  std::string fileName_;
  std::fstream stream_;
  MessageLogPoolType messages_;
  bool stopThread_;
};
} // namespace MythicEngine::support::logger