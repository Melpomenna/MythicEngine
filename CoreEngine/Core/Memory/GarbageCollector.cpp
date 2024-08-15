#include <pch.h>
#include "GarbageCollector.h"

namespace MythicEngine::core::memory {

GarbageCollector::~GarbageCollector() {
  for (auto &it : pointers_) {
    GarbageCollector::UnMark(it);
  }
}

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

void MYTHIC_ENGINE_WIN_API GarbageCollector::Schedule() noexcept {}

} // namespace MythicEngine::core::memory