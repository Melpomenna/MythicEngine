#include <pch.h>
#include "GarbageCollector.h"

namespace MythicEngine::core::memory {

GarbageCollector::~GarbageCollector() { pointers_.clear(); }

void MYTHIC_ENGINE_WIN_API GarbageCollector::Release() {
  ::delete gc_;
  gc_ = nullptr;
}

bool MYTHIC_ENGINE_WIN_API GarbageCollector::Contains(void *ptr) noexcept {
  std::shared_lock lock{mt_};
  if (pointers_.empty())
    return false;
  return pointers_.contains(ptr);
}

void MYTHIC_ENGINE_WIN_API GarbageCollector::Mark(void *ptr) noexcept {
  std::unique_lock lock{mt_};
  if (pointers_.contains(ptr) || nullptr == ptr)
    return;
  pointers_.insert(ptr);
  notEmpty_.notify_one();
}

void MYTHIC_ENGINE_WIN_API GarbageCollector::UnMark(void *ptr) noexcept {
  std::unique_lock lock{mt_};
  if (pointers_.empty() || !pointers_.contains(ptr) || nullptr == ptr)
    return;
  pointers_.erase(ptr);
}

void MYTHIC_ENGINE_WIN_API GarbageCollector::Schedule(float dt) noexcept {
  currentTime_ += dt;
  if (std::abs(DefaultTimeForSchedule - currentTime_) >= 0.0001) {
    currentTime_ = 0;
  }
}

} // namespace MythicEngine::core::memory