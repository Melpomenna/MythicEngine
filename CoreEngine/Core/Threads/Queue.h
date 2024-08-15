#pragma once

#include <Support/Utils/NonCopy.h>
#include <Support/Utils/TypeTraits.h>
#include <Support/Utils/Utils.h>

#include <condition_variable>
#include <queue>
#include <shared_mutex>

namespace MythicEngine::core::threads {

template <class T>
class UnboundingMPMCQueue final : public support::utils::NonCopy {
public:
  using ValueType = T;
  using Reference = support::utils::RemoveReference_t<ValueType>;
  using ConstReference = const support::utils::RemoveReference_t<ValueType> &;
  using RValue = support::utils::RemoveReference_t<ValueType> &&;
  using ContainerType = std::queue<ValueType>;
  using MutexType = std::shared_mutex;
  using ConditionalVariableType = std::condition_variable_any;

public:
  MYTHIC_ENGINE_EXPORT UnboundingMPMCQueue();

  MYTHIC_ENGINE_EXPORT UnboundingMPMCQueue(UnboundingMPMCQueue &&) noexcept;
  MYTHIC_ENGINE_EXPORT UnboundingMPMCQueue &
  operator=(UnboundingMPMCQueue &&) noexcept;

  MYTHIC_ENGINE_EXPORT void Push(ConstReference);
  template <class... Args> void Emplace(Args &&...);

  MYTHIC_ENGINE_EXPORT Reference Top();
  MYTHIC_ENGINE_EXPORT ConstReference Top() const;

  MYTHIC_ENGINE_EXPORT void Pop();
  MYTHIC_ENGINE_EXPORT ValueType TopAndRemove();

  MYTHIC_ENGINE_EXPORT bool IsEmpty() const noexcept;
  MYTHIC_ENGINE_EXPORT size_t Size() const noexcept;

private:
  ContainerType container_;
  mutable MutexType mt_;
  mutable ConditionalVariableType notEmpty_;
};

template <class T> UnboundingMPMCQueue<T>::UnboundingMPMCQueue() = default;

template <class T>
UnboundingMPMCQueue<T>::UnboundingMPMCQueue(
    UnboundingMPMCQueue &&other) noexcept {
  container_ = support::utils::Move(other.container_);
}

template <class T>
UnboundingMPMCQueue<T> &
UnboundingMPMCQueue<T>::operator=(UnboundingMPMCQueue &&other) noexcept {
  if (this == &other)
    return *this;

  std::scoped_lock lock{mt_, other.mt_};

  container_ = support::utils::Move(other.container_);

  return *this;
}

template <class T> void UnboundingMPMCQueue<T>::Push(ConstReference value) {
  Emplace(support::utils::Forward<ConstReference>(value));
}

template <class T>
template <class... Args>
void UnboundingMPMCQueue<T>::Emplace(Args &&...args) {
  static_assert(std::is_constructible_v<ValueType, Args...>);
  std::unique_lock lock{mt_};
  ValueType value{support::utils::Forward<Args>(args)...};
  container_.push(value);
  notEmpty_.notify_one();
}

template <class T>
typename UnboundingMPMCQueue<T>::Reference UnboundingMPMCQueue<T>::Top() {
  std::unique_lock lock{mt_};
  while (IsEmpty()) {
    notEmpty_.wait(lock);
  }
  return container_.front();
}

template <class T>
typename UnboundingMPMCQueue<T>::ConstReference
UnboundingMPMCQueue<T>::Top() const {
  std::unique_lock lock{mt_};
  while (IsEmpty()) {
    notEmpty_.wait(lock);
  }
  return container_.front();
}

template <class T> void UnboundingMPMCQueue<T>::Pop() {
  std::unique_lock lock{mt_};
  while (IsEmpty()) {
    notEmpty_.wait(lock);
  }
  container_.pop();
}

template <class T>
typename UnboundingMPMCQueue<T>::ValueType
UnboundingMPMCQueue<T>::TopAndRemove() {
  std::unique_lock lock{mt_};
  while (IsEmpty()) {
    notEmpty_.wait(lock);
  }
  ValueType value = container_.front();
  container_.pop();
  return support::utils::MoveIfNoexcept(value);
}

template <class T> bool UnboundingMPMCQueue<T>::IsEmpty() const noexcept {
  return Size() == 0;
}

template <class T> size_t UnboundingMPMCQueue<T>::Size() const noexcept {
  return container_.size();
}

template <class T> using Queue = UnboundingMPMCQueue<T>;

} // namespace MythicEngine::core::threads