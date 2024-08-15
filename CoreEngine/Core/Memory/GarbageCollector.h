#pragma once

#include <Support/Utils/TypeTraits.h>
#include <Support/Utils/Utils.h>
#include <condition_variable>
#include <set>
#include <shared_mutex>

namespace MythicEngine::core::memory {

class GarbageCollector
    : public support::utils::EnableDefaultConstructable<false, false, false,
                                                        false> {
public:
  MYTHIC_ENGINE_EXPORT virtual ~GarbageCollector();

  template <class GC = GarbageCollector, class... Args>
  MYTHIC_ENGINE_EXPORT static GC *ResolveGarbageCollector(Args &&...);

  MYTHIC_ENGINE_EXPORT static void Release();

  MYTHIC_ENGINE_EXPORT virtual bool Contains(void *) noexcept;
  MYTHIC_ENGINE_EXPORT virtual void Mark(void *) noexcept;
  MYTHIC_ENGINE_EXPORT virtual void UnMark(void *) noexcept;
  MYTHIC_ENGINE_EXPORT virtual void Schedule(float) noexcept;

private:
  MYTHIC_ENGINE_EXPORT GarbageCollector() = default;

private:
  std::shared_mutex mt_;
  std::condition_variable_any notEmpty_;
  std::set<void *> pointers_;
  float currentTime_ = 0;
  static __CONSTEXPR__ float DefaultTimeForSchedule = 1.5f;
  static __INLINE__ GarbageCollector *gc_ = nullptr;
};

template <class GC, class... Args>
__NODISCARD__ GC *MYTHIC_ENGINE_WIN_API
GarbageCollector::ResolveGarbageCollector(Args &&...args) {
  if (!gc_) {
    gc_ = ::new GC(support::utils::Forward<Args>(args)...);
  }
  return gc_;
}

using GC = GarbageCollector;

} // namespace MythicEngine::core::memory