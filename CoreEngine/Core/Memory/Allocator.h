#pragma once

#include <Core/Memory/AllocatorTraits.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::core::memory {
template <class T> class MYTHIC_ENGINE_EXPORT Allocator {
public:
  using Traits = AllocatorTraits<T>;
  using PointerType = typename Traits::PointerType;

public:
  Allocator() = default;
  virtual ~Allocator() = default;

  Allocator(const Allocator &) = default;
  Allocator &operator=(const Allocator &) = default;

  Allocator(Allocator &&) noexcept = default;
  Allocator &operator=(Allocator &&) noexcept = default;

  __CONSTEXPR__ void *Allocate(size_t);
  __CONSTEXPR__ void Deallocate(void *);
  __CONSTEXPR__ void Construct(void *);
  __CONSTEXPR__ void Destroy(void *);

  template <class... Args> __CONSTEXPR__ void Construct(void *, Args &&...);

private:
  Traits traits_;
};

template <class T>
__CONSTEXPR__ __NODISCARD__ void *MYTHIC_ENGINE_WIN_API
Allocator<T>::Allocate(size_t bytes) {
  return traits_.Allocate(bytes);
}

template <class T>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API Allocator<T>::Deallocate(void *ptr) {
  traits_.Deallocate(ptr);
}

template <class T>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API Allocator<T>::Construct(void *ptr) {
  traits_.Construct(ptr);
}

template <class T>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API Allocator<T>::Destroy(void *ptr) {
  traits_.Destroy(ptr);
}

template <class T>
template <class... Args>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API
Allocator<T>::Construct(void *ptr, Args &&...args) {
  static_assert(std::is_constructible_v<T, Args...>);
  traits_.Construct(ptr, std::forward<Args>(args)...);
}

} // namespace MythicEngine::core::memory