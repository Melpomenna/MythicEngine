#include <pch.h>
#include "Timer.h"
#include <Support/Utils/IUpdateHandler.h>

namespace MythicEngine::core::time {
Timer::Timer() : last_(std::chrono::steady_clock::now()) {
  handlers_ = std::list<memory::OwnPtr<support::utils::IUpdateHandler>>();
}

float MYTHIC_ENGINE_WIN_API Timer::Mark() {
  const auto old = last_.load();
  last_.store(std::chrono::steady_clock::now());
  const std::chrono::duration<float> frameTime = last_.load() - old;
  return frameTime.count();
}

float MYTHIC_ENGINE_WIN_API Timer::Peek() const {
  return std::chrono::duration<float>(std::chrono::steady_clock::now() -
                                      last_.load())
      .count();
}

void MYTHIC_ENGINE_WIN_API Timer::AddHandler(const IUpdateHandlerPtr &handler) {
  auto it = std::ranges::find_if(
      handlers_.cbegin(), handlers_.cend(),
      [=](const auto &h) { return h && handler && handler == h; });
  if (it == handlers_.cend()) {
    handlers_.push_back(handler);
  }
}

void MYTHIC_ENGINE_WIN_API
Timer::RemoveHandler(const IUpdateHandlerPtr &handler) {
  auto it = std::ranges::find_if(
      handlers_.cbegin(), handlers_.cend(),
      [=](const auto &h) { return h && handler && handler == h; });
  if (it != handlers_.cend()) {
    handlers_.erase(it);
  }
}

void MYTHIC_ENGINE_WIN_API Timer::Post() {
  const auto duration = Peek();
  Post([duration](auto &h) { h->OnUpdate(duration); });
  Mark();
}

void MYTHIC_ENGINE_WIN_API Timer::Post(const HandleFuncType &event) {
  for (auto &h : handlers_) {
    event(h);
  }
}

} // namespace MythicEngine::core::time