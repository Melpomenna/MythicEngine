#pragma once

#include <Core/Time/ITimeDispatcher.h>
#include <Support/Utils/Utils.h>
#include <atomic>
#include <chrono>

namespace MythicEngine::core::time {
class Timer final : public ITimeDispatcher {
public:
  MYTHIC_ENGINE_EXPORT Timer();
  MYTHIC_ENGINE_EXPORT float Mark();
  MYTHIC_ENGINE_EXPORT float Peek() const;

  MYTHIC_ENGINE_EXPORT void Post();
  MYTHIC_ENGINE_EXPORT void AddHandler(const IUpdateHandlerPtr &) override;
  MYTHIC_ENGINE_EXPORT void RemoveHandler(const IUpdateHandlerPtr &) override;

protected:
  MYTHIC_ENGINE_EXPORT void Post(const HandleFuncType &) override;

private:
  std::atomic<std::chrono::steady_clock::time_point> last_;
};
} // namespace MythicEngine::core::time