#pragma once

#include <Support/Utils/TypeTraits.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::core::memory {

template <class T> struct MYTHIC_ENGINE_EXPORT AllocatorTraits {
  using PointerType = T *;

  AllocatorTraits() = default;
  virtual ~AllocatorTraits() = default;

  AllocatorTraits(const AllocatorTraits &) = default;
  AllocatorTraits &operator=(const AllocatorTraits &) = default;

  AllocatorTraits(AllocatorTraits &&) noexcept = default;
  AllocatorTraits &operator=(AllocatorTraits &&) noexcept = default;

  __CONSTEXPR__ void *Allocate(size_t);
  __CONSTEXPR__ void Deallocate(void *);
  __CONSTEXPR__ void Construct(void *);
  __CONSTEXPR__ void Destroy(void *);

  template <class... Args> __CONSTEXPR__ void Construct(void *, Args &&...);
};

template <class T>
__CONSTEXPR__ __NODISCARD__ void *MYTHIC_ENGINE_WIN_API
AllocatorTraits<T>::Allocate(size_t bytes) {
  if (bytes == 0)
    return nullptr;
  return malloc(bytes);
}

template <class T>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API
AllocatorTraits<T>::Deallocate(void *ptr) {
  free(ptr);
}

template <class T>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API
AllocatorTraits<T>::Construct(void *ptr) {
  PointerType object = static_cast<PointerType>(ptr);
  object->T();
}

template <class T>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API
AllocatorTraits<T>::Destroy(void *ptr) {
  PointerType object = static_cast<PointerType>(ptr);
  object->~T();
}

template <class T>
template <class... Args>
__CONSTEXPR__ void MYTHIC_ENGINE_WIN_API
AllocatorTraits<T>::Construct(void *ptr, Args &&...args) {
  PointerType object = static_cast<PointerType>(ptr);
  ::new (object) T(support::utils::Forward<Args>(args)...);
}

} // namespace MythicEngine::core::memory