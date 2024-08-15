#pragma once

#include <Core/Memory/GarbageCollector.h>
#include <Support/Utils/TypeTraits.h>
#include <Support/Utils/Utils.h>
#include <new>
#include <type_traits>

namespace MythicEngine::core::memory {

enum class MemoryOrderType { Owner, Parent, NoPointer };

template <class T> class OwnPtr final {
public:
  using PointerType = T *;

  MYTHIC_ENGINE_EXPORT OwnPtr();
  MYTHIC_ENGINE_EXPORT explicit OwnPtr(
      PointerType, MemoryOrderType type = MemoryOrderType::Owner);

  template <class Inhereted> MYTHIC_ENGINE_EXPORT explicit OwnPtr(Inhereted *);

  MYTHIC_ENGINE_EXPORT ~OwnPtr();

  MYTHIC_ENGINE_EXPORT OwnPtr(const OwnPtr &);
  MYTHIC_ENGINE_EXPORT OwnPtr &operator=(const OwnPtr &);

  MYTHIC_ENGINE_EXPORT OwnPtr(OwnPtr &&) noexcept;
  MYTHIC_ENGINE_EXPORT OwnPtr &operator=(OwnPtr &&) noexcept;

  MYTHIC_ENGINE_EXPORT bool operator==(const OwnPtr &) const noexcept;
  MYTHIC_ENGINE_EXPORT bool operator!=(const OwnPtr &) const noexcept;

  template <class Inhereted>
  MYTHIC_ENGINE_EXPORT OwnPtr(const OwnPtr<Inhereted> &);

  template <class Inhereted>
  MYTHIC_ENGINE_EXPORT OwnPtr &operator=(const OwnPtr<Inhereted> &);

  template <class Inhereted>
  MYTHIC_ENGINE_EXPORT OwnPtr(OwnPtr<Inhereted> &&) noexcept;

  template <class Inhereted>
  MYTHIC_ENGINE_EXPORT OwnPtr &operator=(OwnPtr<Inhereted> &&) noexcept;

  MYTHIC_ENGINE_EXPORT PointerType operator->() const;

  MYTHIC_ENGINE_EXPORT operator bool() const noexcept;

  MYTHIC_ENGINE_EXPORT MemoryOrderType GetType() const noexcept;

  MYTHIC_ENGINE_EXPORT PointerType Release() noexcept;
  MYTHIC_ENGINE_EXPORT PointerType Get() const noexcept;

private:
  PointerType pointer_;
  MemoryOrderType type_;
};

template <class T>
OwnPtr<T>::OwnPtr() : pointer_(nullptr), type_(MemoryOrderType::NoPointer) {}

template <class T>
OwnPtr<T>::OwnPtr(PointerType other, MemoryOrderType type) : OwnPtr() {
  pointer_ = other;
  if (pointer_) {
    type_ = type;
  }
  GC::ResolveGarbageCollector()->Mark(pointer_);
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
    GC::ResolveGarbageCollector()->UnMark(pointer_);
    delete pointer_;
    pointer_ = nullptr;
  }
  type_ = MemoryOrderType::NoPointer;
  pointer_ = other.pointer_;

  if (pointer_) {
    type_ = MemoryOrderType::Parent;
  }

  return *this;
}

template <class T> OwnPtr<T>::OwnPtr(OwnPtr &&other) noexcept : OwnPtr() {
  pointer_ = other.pointer_;
  type_ = other.type_;
  GC::ResolveGarbageCollector()->Mark(pointer_);
  other.pointer_ = nullptr;
  other.type_ = MemoryOrderType::NoPointer;
}

template <class T>
OwnPtr<T> &MYTHIC_ENGINE_WIN_API OwnPtr<T>::operator=(OwnPtr &&other) noexcept {
  if (this == &other)
    return *this;

  if (pointer_ != nullptr && type_ == MemoryOrderType::Owner) {
    GC::ResolveGarbageCollector()->UnMark(pointer_);
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
bool MYTHIC_ENGINE_WIN_API
OwnPtr<T>::operator==(const OwnPtr &other) const noexcept {
  return pointer_ == other.Get();
}

template <class T>
bool MYTHIC_ENGINE_WIN_API
OwnPtr<T>::operator!=(const OwnPtr &other) const noexcept {
  return !(*this == other);
}

template <class T>
template <class Inhereted>
OwnPtr<T>::OwnPtr(OwnPtr<Inhereted> &&other) noexcept : OwnPtr() {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  type_ = other.GetType();
  pointer_ = static_cast<PointerType>(other.Release());
  GC::ResolveGarbageCollector()->Mark(pointer_);
}

template <class T>
template <class Inhereted>
OwnPtr<T> &MYTHIC_ENGINE_WIN_API
OwnPtr<T>::operator=(OwnPtr<Inhereted> &&other) noexcept {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  if (pointer_ != nullptr && type_ == MemoryOrderType::Owner) {
    GC::ResolveGarbageCollector()->UnMark(pointer_);
    delete pointer_;
    pointer_ = nullptr;
  }

  type_ = other.GetType();
  pointer_ = static_cast<PointerType>(other.Release());

  return *this;
}

template <class T>
template <class Inhereted>
OwnPtr<T>::OwnPtr(const OwnPtr<Inhereted> &other) : OwnPtr() {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  pointer_ = static_cast<PointerType>(other.Get());
  if (pointer_) {
    type_ = MemoryOrderType::Parent;
  }
  GC::ResolveGarbageCollector()->Mark(pointer_);
}

template <class T>
template <class Inhereted>
OwnPtr<T> &MYTHIC_ENGINE_WIN_API
OwnPtr<T>::operator=(const OwnPtr<Inhereted> &other) {
  static_assert(std::is_convertible_v<Inhereted *, PointerType>);
  if (pointer_ != nullptr && type_ == MemoryOrderType::Owner) {
    GC::ResolveGarbageCollector()->UnMark(pointer_);
    delete pointer_;
    pointer_ = nullptr;
  }

  type_ = MemoryOrderType::NoPointer;
  pointer_ = static_cast<PointerType>(other.Get());
  if (pointer_) {
    type_ = MemoryOrderType::Parent;
  }
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
  GC::ResolveGarbageCollector()->Mark(pointer_);
}

template <class T> OwnPtr<T>::~OwnPtr() {
  if (type_ == MemoryOrderType::NoPointer || type_ == MemoryOrderType::Parent) {
    pointer_ = nullptr;
    type_ = MemoryOrderType::NoPointer;
    return;
  }
  if (type_ == MemoryOrderType::Owner) {
    GC::ResolveGarbageCollector()->UnMark(pointer_);
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
  return type_ != MemoryOrderType::NoPointer &&
         GC::ResolveGarbageCollector()->Contains(pointer_);
}

template <class T>
__NODISCARD__ MemoryOrderType MYTHIC_ENGINE_WIN_API
OwnPtr<T>::GetType() const noexcept {
  return type_;
}

template <class T>
__NODISCARD__ typename OwnPtr<T>::PointerType MYTHIC_ENGINE_WIN_API
OwnPtr<T>::Release() noexcept {
  PointerType ptr = pointer_;
  pointer_ = nullptr;
  type_ = MemoryOrderType::NoPointer;
  return ptr;
}

template <class T>
typename OwnPtr<T>::PointerType MYTHIC_ENGINE_WIN_API
OwnPtr<T>::Get() const noexcept {
  return pointer_;
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