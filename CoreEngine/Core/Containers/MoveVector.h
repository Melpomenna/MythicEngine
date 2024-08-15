#pragma once

#include <Core/Algorithms/Algorithms.h>
#include <Core/Containers/ContainerExceptions.h>
#include <Core/Memory/Allocator.h>
#include <Support/Utils/NonCopy.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::core::containers {

template <class T, class Allocator = memory::Allocator<T>>
class MYTHIC_ENGINE_EXPORT MoveVector : public support::utils::NonCopy {
public:
  class MYTHIC_ENGINE_EXPORT Iterator final {
  public:
    friend class MoveVector;
    Iterator();
    explicit Iterator(T *);
    ~Iterator();

    Iterator(const Iterator &) noexcept;
    Iterator &operator=(const Iterator &) noexcept;

    Iterator(Iterator &&) noexcept;
    Iterator &operator=(Iterator &&) noexcept;

    Iterator &operator++() noexcept;
    Iterator operator++(int) noexcept;

    Iterator &operator--() noexcept;
    Iterator operator--(int) noexcept;

    Iterator operator+(size_t) noexcept;
    Iterator operator-(size_t) noexcept;

    bool operator==(const Iterator &) const noexcept;
    bool operator!=(const Iterator &) const noexcept;

    size_t operator+(const Iterator &) const noexcept;
    size_t operator-(const Iterator &) const noexcept;

    T &&operator*();

    const T &&operator*() const;

    T *operator->() const noexcept;
  private:
    explicit Iterator(T*, size_t);
  private:
    T *ptr_;
    size_t shift_;
  };

  class MYTHIC_ENGINE_EXPORT ReversIterator final {
  public:
    ReversIterator();
    explicit ReversIterator(T *);
    ~ReversIterator();

    ReversIterator(const ReversIterator &) noexcept;
    ReversIterator &operator=(const ReversIterator &) noexcept;

    ReversIterator(ReversIterator &&) noexcept;
    ReversIterator &operator=(ReversIterator &&) noexcept;

    ReversIterator &operator++() noexcept;
    ReversIterator operator++(int) noexcept;

    ReversIterator &operator--() noexcept;
    ReversIterator operator--(int) noexcept;

    ReversIterator operator+(size_t) noexcept;
    ReversIterator operator-(size_t) noexcept;

    size_t operator+(const ReversIterator &) noexcept;
    size_t operator-(const ReversIterator &) noexcept;

    bool operator==(const ReversIterator &) const noexcept;
    bool operator!=(const ReversIterator &) const noexcept;

    T &&operator*();
    const T &&operator*() const;

    T *operator->() const noexcept;

  private:
    explicit ReversIterator(T *, size_t);

  private:
    T *ptr_;
    size_t shift_;
  };

  using SizeType = size_t;
  using ValueType = support::utils::RemoveReference_t<T>;
  using AllocatorType = Allocator;
  using PointerType = ValueType *;
  using RValueType = ValueType &&;
  using IteratorType = Iterator;
  using ReversIteratorType = ReversIterator;

  MoveVector();

  explicit MoveVector(SizeType,
                      AllocatorType allocator = memory::Allocator<T>{});

  ~MoveVector();

  MoveVector(MoveVector &&) noexcept;
  MoveVector &operator=(MoveVector &&) noexcept;

  void PushBack(RValueType) noexcept(std::is_nothrow_move_assignable_v<T>);
  template <class... Args>
  void
  EmplaceBack(Args &&...) noexcept(std::is_nothrow_constructible_v<T, Args...>);

  void Insert(Iterator,
              RValueType) noexcept(std::is_nothrow_move_assignable_v<T>);
  template <class... Args>
  void Insert(Iterator,
              Args &&...) noexcept(std::is_nothrow_move_assignable_v<T>);

  void EraseBack();
  void Erase(Iterator);
  void Erase(Iterator, Iterator);

  RValueType At(SizeType);
  RValueType operator[](SizeType) noexcept;

  void Reserve(SizeType);

  SizeType Size() const noexcept;
  bool IsEmpty() const noexcept;
  void Clear() noexcept;

  Iterator Begin() noexcept;
  Iterator End() noexcept;
  ReversIterator RBegin() noexcept;
  ReversIterator REnd() noexcept;

private:
  bool TryResize() noexcept;
  bool TryResize(SizeType) noexcept;

private:
  SizeType size_;
  SizeType capacity_;
  static constexpr SizeType capacityMultiplier_ = 2;
  static constexpr SizeType sizeOfType = sizeof(T);
  AllocatorType allocator_;
  PointerType data_;
};

template <class T, class Allocator>
MoveVector<T, Allocator>::MoveVector()
    : size_(0), capacity_(0), allocator_(), data_(nullptr) {}

template <class T, class Allocator>
MoveVector<T, Allocator>::MoveVector(SizeType size, AllocatorType allocator)
    : size_(size), capacity_(size * capacityMultiplier_), allocator_(allocator),
      data_(nullptr) {
  data_ = static_cast<PointerType>(allocator_.Allocate(capacity_ * sizeOfType));
  for (SizeType i = 0; i < size_; ++i) {
    allocator_.Construct(data_ + i);
  }
}

template <class T, class Allocator> MoveVector<T, Allocator>::~MoveVector() {
  for (SizeType i = 0; i < size_; ++i) {
    allocator_.Destroy(data_ + i);
  }
  allocator_.Deallocate(data_);
}

template <class T, class Allocator>
MoveVector<T, Allocator>::MoveVector(MoveVector &&other) noexcept {
  size_ = other.size_;
  capacity_ = other.capacity_;
  data_ = other.data_;
  allocator_ = support::utils::Move(other.allocator_);
  other.size_ = 0;
  other.capacity_ = 0;
  other.data_ = nullptr;
  other.allocator_ = {};
}

template <class T, class Allocator>
MoveVector<T, Allocator> &
MoveVector<T, Allocator>::operator=(MoveVector &&other) noexcept {
  if (this == &other)
    return *this;

  for (SizeType i = 0; i < size_; ++i) {
    allocator_.Destroy(data_ + i);
  }
  allocator_.Deallocate(data_);

  size_ = other.size_;
  capacity_ = other.capacity_;
  data_ = other.data_;
  allocator_ = support::utils::Move(other.allocator_);
  other.size_ = 0;
  other.capacity_ = 0;
  other.allocator_ = {};
  other.data_ = nullptr;
  return *this;
}

template <class T, class Allocator>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::PushBack(
    RValueType value) noexcept(std::is_nothrow_move_assignable_v<T>) {
  EmplaceBack(support::utils::Move(value));
}

template <class T, class Allocator>
template <class... Args>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::EmplaceBack(
    Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
  ValueType value{support::utils::Forward<Args>(args)...};
  if (size_ == capacity_ && !TryResize()) {
    return;
  }
  data_[size_] = support::utils::Move(value);
  ++size_;
}

template <class T, class Allocator>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Insert(
    Iterator pos,
    RValueType value) noexcept(std::is_nothrow_move_assignable_v<T>) {
  Insert(pos, support::utils::Move(value));
}

template <class T, class Allocator>
template <class... Args>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Insert(
    Iterator pos,
    Args &&...args) noexcept(std::is_nothrow_move_assignable_v<T>) {
  const auto diff = pos - Begin();
  if (size_ == capacity_ && !TryResize()) {
    return;
  }
  ValueType created{support::utils::Forward<Args>(args)...};
  auto stored = support::utils::Move(*(data_ + diff));
  *(data_ + diff) = support::utils::Move(created);
  for (SizeType i = diff + 1; i < size_ + 1; ++i) {
    auto tmp = support::utils::Move(*(data_ + i));
    *(data_ + i) = support::utils::Move(stored);
    stored = support::utils::Move(tmp);
  }
  ++size_;
}

template <class T, class Allocator>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::EraseBack() {
  if (size_ == 0)
    return;

  allocator_.Destroy(data_ + size_);
  *(data_ + size_) = support::utils::Move(ValueType{});
  size_--;
}

template <class T, class Allocator>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Erase(Iterator pos) {
  if (size_ == 0)
    return;
  auto diff = pos - Begin();
  allocator_.Destroy(data_ + diff);
  ValueType tmp{};
  for (SizeType i = diff; i < size_ - 1; ++i) {
    tmp = *(data_ + i + 1);
    *(data_ + i) = support::utils::Move(tmp);
  }
  size_--;
}

template <class T, class Allocator>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Erase(Iterator pos,
                                                           Iterator end) {
  for (; pos != end; ++pos) {
    Erase(pos);
  }
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::RValueType MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::At(SizeType idx) {
  if (idx >= size_) {
    throw OUT_OF_RANGE;
  }
  return data_[idx];
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::RValueType MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::operator[](SizeType idx) noexcept {
  return support::utils::Move(data_[idx]);
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::SizeType MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Size() const noexcept {
  return size_;
}

template <class T, class Allocator>
void MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Reserve(SizeType size) {
  TryResize(size);
}

template <class T, class Allocator>
bool MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::IsEmpty() const noexcept {
  return size_ == 0;
}

template <class T, class Allocator>
void MoveVector<T, Allocator>::Clear() noexcept {
  for (SizeType i = 0; i < size_; ++i) {
    allocator_.Destroy(data_ + i);
  }
  size_ = 0;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Begin() noexcept {
  return IteratorType(data_);
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::End() noexcept {
  return IteratorType(data_ + size_, size_);
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::RBegin() noexcept {
  return ReversIterator(data_ + size_, size_);
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::REnd() noexcept {
  return ReversIterator(data_, 0);
}

template <class T, class Allocator>
bool MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::TryResize() noexcept {
  try {
    if (capacity_ == 0) {
      capacity_ = 1;
      PointerType ptr = static_cast<PointerType>(
        allocator_.Allocate(capacity_ * sizeOfType));
      allocator_.Construct(ptr);
      data_ = ptr;
      return true;
    }
    PointerType ptr = static_cast<PointerType>(
        allocator_.Allocate(capacity_ * capacityMultiplier_ * sizeOfType));
    for (SizeType i = 0; i < size_; ++i) {
      allocator_.Construct(ptr + i, support::utils::Move(data_[i]));
      allocator_.Destroy(data_ + i);
    }

    allocator_.Deallocate(data_);
    data_ = ptr;
    capacity_ *= capacityMultiplier_;
    return true;
  } catch (...) {
    return false;
  }
}

template <class T, class Allocator>
bool MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::TryResize(SizeType size) noexcept {
  try {
    if (capacity_ >= size)
      return false;
    void *ptr = allocator_.Allocate(size * sizeOfType);
    if (data_ == nullptr) {
      data_ = ptr;
      capacity_ = size;
      return true;
    }

    for (SizeType i = 0; i < size_; ++i) {
      allocator_.Construct(ptr + i, support::utils::Move(*(data_ + i)));
      allocator_.Destroy(data_ + i);
    }

    allocator_.Deallocate(data_);
    data_ = ptr;
    capacity_ = size;
    return true;
  } catch (...) {
    return false;
  }
}

template <class T, class Allocator>
MoveVector<T, Allocator>::Iterator::Iterator() : ptr_(nullptr), shift_(0) {}

template <class T, class Allocator>
MoveVector<T, Allocator>::Iterator::Iterator(T *ptr) : ptr_(ptr), shift_(0) {}

template <class T, class Allocator>
MoveVector<T, Allocator>::Iterator::~Iterator() {
  ptr_ = nullptr;
  shift_ = 0;
}

template<class T, class Allocator>
MoveVector<T,Allocator>::Iterator::Iterator(T* ptr, size_t shift) : ptr_(ptr), shift_(shift)  {

}

template <class T, class Allocator>
MoveVector<T, Allocator>::Iterator::Iterator(const Iterator &other) noexcept {
  ptr_ = other.ptr_;
  shift_ = other.shift_;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator=(const Iterator &other) noexcept {
  if (this == &other)
    return *this;

  ptr_ = other.ptr_;
  shift_ = other.shift_;

  return *this;
}

template <class T, class Allocator>
MoveVector<T, Allocator>::Iterator::Iterator(Iterator &&other) noexcept {
  ptr_ = other.ptr_;
  shift_ = other.shift_;
  other.ptr_ = nullptr;
  other.shift_ = 0;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator=(Iterator &&other) noexcept {
  if (this == &other)
    return *this;

  ptr_ = other.ptr_;
  shift_ = other.shift_;
  other.ptr_ = nullptr;
  other.shift_ = 0;

  return *this;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator++() noexcept {
  ++shift_;
  return *this;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator++(int) noexcept {
  Iterator it{*this};
  ++shift_;
  return it;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator--() noexcept {
  --shift_;
  return *this;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator--(int) noexcept {
  Iterator it{*this};
  --shift_;
  return it;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator+(size_t shift) noexcept {
  Iterator it{*this};
  it.shift_ += shift;
  return it;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::Iterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator-(size_t shift) noexcept {
  Iterator it{*this};
  it.shift_ -= shift;
  return it;
}

template <class T, class Allocator>
size_t MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Iterator::operator+(
    const Iterator &other) const noexcept {
  return shift_ + other.shift_;
}

template <class T, class Allocator>
size_t MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Iterator::operator-(
    const Iterator &other) const noexcept {
  return algorithms::Max(shift_, other.shift_) -
         algorithms::Min(shift_, other.shift_);
}

template <class T, class Allocator>
bool MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Iterator::operator==(
    const Iterator &other) const noexcept {
  return shift_ == other.shift_;
}

template <class T, class Allocator>
bool MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Iterator::operator!=(
    const Iterator &other) const noexcept {
  return !(*this == other);
}

template <class T, class Allocator>
T &&MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::Iterator::operator*() {
  return support::utils::Move(ptr_[shift_]);
}

template <class T, class Allocator>
const T &&MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator*() const {
  return support::utils::Move(ptr_[shift_]);
}

template <class T, class Allocator>
T *MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::Iterator::operator->() const noexcept {
  return &ptr_[shift_];
}

template <class T, class Allocator>
MoveVector<T, Allocator>::ReversIterator::ReversIterator() {
  ptr_ = nullptr;
  shift_ = 0;
}

template <class T, class Allocator>
MoveVector<T, Allocator>::ReversIterator::ReversIterator(T *ptr) {
  ptr_ = ptr;
  shift_ = 0;
}

template <class T, class Allocator>
MoveVector<T, Allocator>::ReversIterator::~ReversIterator() {
  ptr_ = nullptr;
  shift_ = 0;
}

template <class T, class Allocator>
MoveVector<T, Allocator>::ReversIterator::ReversIterator(
    const ReversIterator &other) noexcept {
  ptr_ = other.ptr_;
  shift_ = other.shift_;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator=(
    const ReversIterator &other) noexcept {
  if (this == &other)
    return *this;

  ptr_ = other.ptr_;
  shift_ = other.shift_;
  return *this;
}

template <class T, class Allocator>
MoveVector<T, Allocator>::ReversIterator::ReversIterator(
    ReversIterator &&other) noexcept {
  ptr_ = other.ptr_;
  shift_ = other.shift_;

  other.ptr_ = nullptr;
  other.shift_ = 0;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator=(
    ReversIterator &&other) noexcept {
  if (this == &other)
    return *this;

  ptr_ = other.ptr_;
  shift_ = other.shift_;

  other.ptr_ = nullptr;
  other.shift_ = 0;

  return *this;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator++() noexcept {
  --shift_;
  return *this;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator++(int) noexcept {
  ReversIterator it(*this);
  --shift_;
  return it;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator &MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator--() noexcept {
  ++shift_;
  return *this;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator--(int) noexcept {
  ReversIterator it(*this);
  ++shift_;
  return it;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator
MoveVector<T, Allocator>::ReversIterator::operator+(size_t shift) noexcept {
  ReversIterator it{*this};
  it.shift_ -= shift;
  return it;
}

template <class T, class Allocator>
typename MoveVector<T, Allocator>::ReversIterator MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator-(size_t shift) noexcept {
  ReversIterator it{*this};
  it.shift_ += shift;
  return it;
}

template <class T, class Allocator>
size_t MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator+(
    const ReversIterator &other) noexcept {
  return shift_ + other.shift_;
}

template <class T, class Allocator>
size_t MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator-(
    const ReversIterator &other) noexcept {
  return algorithms::Max(shift_, other.shift_) -
         algorithms::Min(shift_, other.shift_);
}

template <class T, class Allocator>
bool MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::ReversIterator::operator==(
    const ReversIterator &other) const noexcept {
  return shift_ == other.shift_ && ptr_ == other.ptr_;
}

template <class T, class Allocator>
bool MYTHIC_ENGINE_WIN_API MoveVector<T, Allocator>::ReversIterator::operator!=(
    const ReversIterator &other) const noexcept {
  return !(*this == other);
}

template <class T, class Allocator>
T &&MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator*() {
  return std::move(ptr_[shift_]);
}

template <class T, class Allocator>
const T &&MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator*() const {
  return std::move(ptr_[shift_]);
}

template <class T, class Allocator>
T *MYTHIC_ENGINE_WIN_API
MoveVector<T, Allocator>::ReversIterator::operator->() const noexcept {
  return &ptr_[shift_];
}

template <class T, class Allocator>
MoveVector<T, Allocator>::ReversIterator::ReversIterator(T *ptr, size_t shift)
    : ptr_(ptr), shift_(shift) {}

} // namespace MythicEngine::core::containers