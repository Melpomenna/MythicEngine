#pragma once

#include <Support/Utils/Utils.h>
#include <functional>
#include <list>
#include <memory>

namespace MythicEngine::support::utils {

class IUpdateHandler;

class ITimerDispatcher {
protected:
  using IUpdateHandlerPtr = IUpdateHandler *;
  using HandleFuncType = std::function<void(IUpdateHandler *)>;

public:
  MYTHIC_ENGINE_EXPORT ITimerDispatcher() = default;
  MYTHIC_ENGINE_EXPORT virtual ~ITimerDispatcher() = default;

  MYTHIC_ENGINE_EXPORT ITimerDispatcher(const ITimerDispatcher &) = default;
  MYTHIC_ENGINE_EXPORT ITimerDispatcher &
  operator=(const ITimerDispatcher &) = default;

  MYTHIC_ENGINE_EXPORT ITimerDispatcher(ITimerDispatcher &&) noexcept = default;
  MYTHIC_ENGINE_EXPORT ITimerDispatcher &
  operator=(ITimerDispatcher &&) noexcept = default;

  MYTHIC_ENGINE_EXPORT virtual void AddHandler(IUpdateHandlerPtr) = 0;
  MYTHIC_ENGINE_EXPORT virtual void RemoveHandler(IUpdateHandlerPtr) = 0;
  MYTHIC_ENGINE_EXPORT virtual void Post(const HandleFuncType &) = 0;

protected:
  std::pmr::list<IUpdateHandlerPtr> handlers_;
};
} // namespace MythicEngine::support::utils
