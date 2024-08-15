#pragma once

#include <Support/Utils/NonCopy.h>
#include <Support/Utils/Utils.h>

#include <Support/Utils/TypeTraits.h>
#include <new>
#include <type_traits>

namespace MythicEngine::core::memory {

template <class T>
class MYTHIC_ENGINE_EXPORT UniquePtr final : public support::utils::NonCopy {
public:
  using PointerType = T *;

public:
  UniquePtr();
  explicit UniquePtr(PointerType *);

  template <class Inhereted> explicit UniquePtr(Inhereted *);

  template <class... Args>
  explicit UniquePtr(Args &&...) noexcept(
      std::is_nothrow_constructible_v<PointerType, Args...>);

  UniquePtr(UniquePtr &&) noexcept;
  UniquePtr &operator=(UniquePtr &&) noexcept;

  template <class Inhereted> UniquePtr(UniquePtr<Inhereted> &&) noexcept;

  template <class Inhereted>
  UniquePtr &operator=(UniquePtr<Inhereted> &&) noexcept;

  ~UniquePtr();

  PointerType Release() noexcept;

  PointerType operator->() const noexcept;

  operator bool() const noexcept;

private:
  PointerType ptr_;
};

template <class T> UniquePtr<T>::UniquePtr() : ptr_(nullptr) {}

template <class T> UniquePtr<T>::UniquePtr(PointerType *ptr) { ptr_ = ptr; }

template <class T>
template <class Inhereted>
UniquePtr<T>::UniquePtr(Inhereted *ptr) {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  ptr_ = static_cast<PointerType>(ptr);
}

template <class T>
template <class... Args>
UniquePtr<T>::UniquePtr(Args &&...args) noexcept(
    std::is_nothrow_constructible_v<PointerType, Args...>) {
  ptr_ = ::new (std::nothrow) T(support::utils::Forward<Args>(args)...);
}

template <class T> UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept {
  ptr_ = other.Release();
}

template <class T>
UniquePtr<T> &MYTHIC_ENGINE_WIN_API
UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
  if (this == &other)
    return *this;

  ptr_ = other.Release();
  return *this;
}

template <class T>
template <class Inhereted>
UniquePtr<T>::UniquePtr(UniquePtr<Inhereted> &&other) noexcept {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  ptr_ = static_cast<PointerType>(other.Release());
}

template <class T>
template <class Inhereted>
UniquePtr<T> &MYTHIC_ENGINE_WIN_API
UniquePtr<T>::operator=(UniquePtr<Inhereted> &&other) noexcept {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  ptr_ = static_cast<PointerType>(other.Release());
  return *this;
}

template <class T> UniquePtr<T>::~UniquePtr() {
  delete ptr_;
  ptr_ = nullptr;
}

template <class T>
__NODISCARD__ typename UniquePtr<T>::PointerType MYTHIC_ENGINE_WIN_API
UniquePtr<T>::Release() noexcept {
  auto copy = ptr_;
  ptr_ = nullptr;
  return copy;
}

template <class T>
typename UniquePtr<T>::PointerType MYTHIC_ENGINE_WIN_API
UniquePtr<T>::operator->() const noexcept {
  return ptr_;
}

template <class T>
MYTHIC_ENGINE_WIN_API UniquePtr<T>::operator bool() const noexcept {
  return ptr_;
}

template <class T, class... Args>
__NODISCARD__ UniquePtr<T> MYTHIC_ENGINE_WIN_API MakeUnique(
    Args &&...args) noexcept(std::is_nothrow_constructible_v<T *, Args...>) {
  return support::utils::Move(
      UniquePtr<T>(support::utils::Forward<Args>(args)...));
}

template <class Base, class Inhereted, class... Args>
__NODISCARD__ UniquePtr<Base>
    MYTHIC_ENGINE_WIN_API MakeUnique(Args &&...args) noexcept(
        std::is_nothrow_constructible_v<Inhereted *, Args...>) {
  static_assert(std::is_convertible_v<Inhereted *, Base *>);
  return UniquePtr<Base>(::new (std::nothrow)
                             Inhereted(support::utils::Forward<Args>(args)...));
}

} // namespace MythicEngine::core::memory