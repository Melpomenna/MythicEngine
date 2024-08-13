#pragma once

#include <functional>
#include <list>
#include <memory>

namespace MythicEngine::engine::system::mouse {
class IMouseHandler;

class IMouseDispatcher {
protected:
  using IMouseHandlerPtr = std::weak_ptr<IMouseHandler>;
  using HandleFuncType = std::function<void(IMouseHandler *)>;

public:
  IMouseDispatcher() = default;
  virtual ~IMouseDispatcher() = default;

  IMouseDispatcher(const IMouseDispatcher &) = default;
  IMouseDispatcher &operator=(const IMouseDispatcher &) = default;

  IMouseDispatcher(IMouseDispatcher &&) noexcept = default;
  IMouseDispatcher &operator=(IMouseDispatcher &&) noexcept = default;

  virtual void AddHandler(const IMouseHandlerPtr &) = 0;
  virtual void RemoveHandler(const IMouseHandlerPtr &) = 0;
  virtual void Post(const HandleFuncType &) = 0;

protected:
  std::pmr::list<IMouseHandlerPtr> handlers_;
};
} // namespace MythicEngine::engine::system::mouse