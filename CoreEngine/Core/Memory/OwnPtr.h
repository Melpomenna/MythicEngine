#pragma once

#include <Support/Utils/TypeTraits.h>
#include <Support/Utils/Utils.h>
#include <new>
#include <type_traits>

namespace MythicEngine::core::memory {

enum class MemoryOrderType { Owner, Parent, NoPointer };

template <class T> class MYTHIC_ENGINE_EXPORT OwnPtr final {
public:
  using PointerType = T *;

  OwnPtr();
  explicit OwnPtr(PointerType);

  template <class Inhereted> explicit OwnPtr(Inhereted *);

  ~OwnPtr();

  OwnPtr(const OwnPtr &);
  OwnPtr &operator=(const OwnPtr &);

  OwnPtr(OwnPtr &&) noexcept;
  OwnPtr &operator=(OwnPtr &&) noexcept;

  PointerType operator->() const;

  operator bool() const noexcept;

  MemoryOrderType GetType() const noexcept;

private:
  PointerType pointer_;
  MemoryOrderType type_;
};

template <class T>
OwnPtr<T>::OwnPtr() : pointer_(nullptr), type_(MemoryOrderType::NoPointer) {}

template <class T> OwnPtr<T>::OwnPtr(PointerType other) : OwnPtr() {
  pointer_ = other;
  if (pointer_) {
    type_ = MemoryOrderType::Owner;
  }
}

template <class T> OwnPtr<T>::OwnPtr(const OwnPtr &other) : OwnPtr() {
  pointer_ = other.pointer_;
  if (pointer_) {
    type_ = MemoryOrderType::Parent;
  }
}

template <class T>
OwnPtr<T> &MYTHIC_ENGINE_WIN_API OwnPtr<T>::operator=(const OwnPtr &other) {
  if (this == &other)
    return *this;

  if (pointer_ != nullptr && type_ == MemoryOrderType::Owner) {
    delete pointer_;
    pointer_ = nullptr;
  }

  pointer_ = other.pointer_;

  if (pointer_ == nullptr) {
    type_ = MemoryOrderType::NoPointer;
  } else {
    type_ = MemoryOrderType::Parent;
  }

  return *this;
}

template <class T> OwnPtr<T>::OwnPtr(OwnPtr &&other) noexcept : OwnPtr() {
  pointer_ = other.pointer_;
  type_ = other.type_;
  other.pointer_ = nullptr;
  other.type_ = MemoryOrderType::NoPointer;
}

template <class T>
OwnPtr<T> &MYTHIC_ENGINE_WIN_API OwnPtr<T>::operator=(OwnPtr &&other) noexcept {
  if (this == &other)
    return *this;

  if (pointer_ != nullptr && type_ == MemoryOrderType::Owner) {
    delete pointer_;
    pointer_ = nullptr;
  }

  pointer_ = other.pointer_;
  type_ = other.type_;
  other.pointer_ = nullptr;
  other.type_ = MemoryOrderType::NoPointer;

  return *this;
}

template <class T>
template <class Inhereted>
OwnPtr<T>::OwnPtr(Inhereted *pointer) : OwnPtr() {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  pointer_ = static_cast<PointerType>(pointer);
  if (pointer_) {
    type_ = MemoryOrderType::Owner;
  }
}

template <class T> OwnPtr<T>::~OwnPtr() {
  if (type_ == MemoryOrderType::NoPointer || type_ == MemoryOrderType::Parent)
    return;

  if (type_ == MemoryOrderType::Owner) {
    delete pointer_;
  }
  pointer_ = nullptr;
  type_ = MemoryOrderType::NoPointer;
}

template <class T>
typename OwnPtr<T>::PointerType MYTHIC_ENGINE_WIN_API
OwnPtr<T>::operator->() const {
  return pointer_;
}

template <class T>
MYTHIC_ENGINE_WIN_API OwnPtr<T>::operator bool() const noexcept {
  return pointer_ != nullptr;
}

template <class T>
__NODISCARD__ MemoryOrderType MYTHIC_ENGINE_WIN_API
OwnPtr<T>::GetType() const noexcept {
  return type_;
}

template <class T, class... Args>
__NODISCARD__ OwnPtr<T> MYTHIC_ENGINE_WIN_API MakeOwn(Args &&...args) {
  return OwnPtr<T>(::new (std::nothrow)
                       T(support::utils::Forward<Args>(args)...));
}

template <class Base, class Inhereted, class... Args>
__NODISCARD__ OwnPtr<Base> MYTHIC_ENGINE_WIN_API MakeOwn(Args &&...args) {
  static_assert(std::is_constructible_v<Inhereted, Args...>);
  auto pointer =
      ::new (std::nothrow) Inhereted(support::utils::Forward<Args>(args)...);
  return OwnPtr<Base>(pointer);
}

} // namespace MythicEngine::core::memory