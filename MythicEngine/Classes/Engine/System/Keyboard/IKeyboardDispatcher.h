#pragma once

#include <functional>
#include <list>
#include <memory>

namespace MythicEngine::engine::system::keyboard {
class IKeyboardHandler;

class IKeyboardDispatcher {
protected:
  using IKeyboardHandlerPtr = std::weak_ptr<IKeyboardHandler>;
  using HandleFuncType = std::function<void(IKeyboardHandler *)>;

public:
  IKeyboardDispatcher() = default;
  virtual ~IKeyboardDispatcher() = default;

  IKeyboardDispatcher(const IKeyboardDispatcher &) = default;
  IKeyboardDispatcher &operator=(const IKeyboardDispatcher &) = default;

  IKeyboardDispatcher(IKeyboardDispatcher &&) noexcept = default;
  IKeyboardDispatcher &operator=(IKeyboardDispatcher &&) noexcept = default;

  virtual void AddHandler(const IKeyboardHandlerPtr &) = 0;
  virtual void RemoveHandler(const IKeyboardHandlerPtr &) = 0;
  virtual void Post(const HandleFuncType &) = 0;

protected:
  std::pmr::list<IKeyboardHandlerPtr> handlers_;
};
} // namespace MythicEngine::engine::system::keyboard