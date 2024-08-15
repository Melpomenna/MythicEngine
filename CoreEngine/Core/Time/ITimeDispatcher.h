#pragma once

#include <Core/Memory/OwnPtr.h>
#include <Support/Utils/Utils.h>
#include <functional>
#include <list>

namespace MythicEngine::support::utils {
class IUpdateHandler;
}

namespace MythicEngine::core::time {

class ITimeDispatcher {
protected:
  using IUpdateHandlerPtr = memory::OwnPtr<support::utils::IUpdateHandler>;
  using HandleFuncType = std::function<void(IUpdateHandlerPtr &)>;

public:
  MYTHIC_ENGINE_EXPORT ITimeDispatcher() = default;
  MYTHIC_ENGINE_EXPORT virtual ~ITimeDispatcher() = default;

  MYTHIC_ENGINE_EXPORT ITimeDispatcher(const ITimeDispatcher &) = default;
  MYTHIC_ENGINE_EXPORT ITimeDispatcher &
  operator=(const ITimeDispatcher &) = default;

  MYTHIC_ENGINE_EXPORT ITimeDispatcher(ITimeDispatcher &&) noexcept = default;
  MYTHIC_ENGINE_EXPORT ITimeDispatcher &
  operator=(ITimeDispatcher &&) noexcept = default;

  MYTHIC_ENGINE_EXPORT virtual void AddHandler(const IUpdateHandlerPtr &) = 0;
  MYTHIC_ENGINE_EXPORT virtual void
  RemoveHandler(const IUpdateHandlerPtr &) = 0;
  MYTHIC_ENGINE_EXPORT virtual void Post(const HandleFuncType &) = 0;

protected:
  std::list<IUpdateHandlerPtr> handlers_;
};
} // namespace MythicEngine::core::time
