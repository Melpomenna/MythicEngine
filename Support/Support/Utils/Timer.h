#pragma once

#include <Support/Utils/ITimerDispatcher.h>
#include <Support/Utils/Utils.h>
#include <atomic>
#include <chrono>

namespace MythicEngine::support::utils {
class Timer final : public ITimerDispatcher {
public:
  MYTHIC_ENGINE_EXPORT Timer();
  MYTHIC_ENGINE_EXPORT float Mark();
  MYTHIC_ENGINE_EXPORT float Peek() const;

  MYTHIC_ENGINE_EXPORT void Post();
  MYTHIC_ENGINE_EXPORT void AddHandler(IUpdateHandlerPtr) override;
  MYTHIC_ENGINE_EXPORT void RemoveHandler(IUpdateHandlerPtr) override;

protected:
  MYTHIC_ENGINE_EXPORT void Post(const HandleFuncType &) override;

private:
  std::atomic<std::chrono::steady_clock::time_point> last_;
};
} // namespace MythicEngine::support::utils