#include "IMouse.h"

#include <Support/Utils/Utils.h>
#include <algorithm>

namespace MythicEngine::engine::system::mouse {
void MYTHIC_ENGINE_WIN_API IMouse::AddHandler(const IMouseHandlerPtr &handler) {
  auto it = std::ranges::find_if(
      handlers_.cbegin(), handlers_.cend(), [=](const auto &h) {
        return !h.expired() && !handler.expired() && h.lock() == handler.lock();
      });
  if (it == handlers_.cend()) {
    handlers_.emplace_back(handler);
  }
}

void MYTHIC_ENGINE_WIN_API
IMouse::RemoveHandler(const IMouseHandlerPtr &handler) {
  auto it = std::ranges::find_if(
      handlers_.cbegin(), handlers_.cend(), [=](const auto &h) {
        return !h.expired() && !handler.expired() && h.lock() == handler.lock();
      });
  if (it != handlers_.cend()) {
    handlers_.erase(it);
  }
}

void MYTHIC_ENGINE_WIN_API IMouse::Post(const HandleFuncType &event) {
  const auto eventWrapper = [&](const IMouseHandlerPtr &h) {
    if (h.expired())
      return;
    event(h.lock().get());
  };
  std::ranges::for_each(handlers_.cbegin(), handlers_.cend(), eventWrapper);
}
} // namespace MythicEngine::engine::system::mouse
