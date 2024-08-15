#include <pch.h>
#include "Thread.h"

#include <Core/Threads/IRunnable.h>
#include <Support/Utils/Exceptions.h>
#include <Support/Utils/TypeTraits.h>

namespace MythicEngine::core::threads {

Thread::Thread() : task_(nullptr), id_(reinterpret_cast<uint64_t>(this)) {
  state_ = ThreadState::NoWork;
}

Thread::Thread(RunnableType &&runnable)
    : task_(support::utils::Move(runnable)),
      id_(reinterpret_cast<uint64_t>(this)) {
  worker_ = std::thread{&Thread::Run, this};
  state_ = ThreadState::Joinable;
}

Thread::Thread(const RunnableType &runnable)
    : task_(runnable), id_(reinterpret_cast<uint64_t>(this)) {
  worker_ = std::thread{&Thread::Run, this};
  state_ = ThreadState::Joinable;
}

Thread::Thread(Thread &&other) noexcept {
  task_ = support::utils::Move(other.task_);
  worker_ = support::utils::Move(other.worker_);
  id_ = other.id_;
  currentException_ = support::utils::Move(other.currentException_);
  isTaskProcessed_.store(other.isTaskProcessed_.load());
  state_ = support::utils::Move(other.state_);

  other.worker_ = {};
  other.id_ = 0;
  other.currentException_ = std::nullopt;
  other.isTaskProcessed_.store(false);
  other.state_ = ThreadState::NoWork;
}

Thread &MYTHIC_ENGINE_WIN_API Thread::operator=(Thread &&other) noexcept {
  if (this == &other)
    return *this;

  task_ = support::utils::Move(other.task_);
  worker_ = support::utils::Move(other.worker_);
  id_ = other.id_;
  currentException_ = support::utils::Move(other.currentException_);
  isTaskProcessed_.store(other.isTaskProcessed_.load());
  state_ = support::utils::Move(other.state_);

  other.worker_ = {};
  other.id_ = 0;
  other.currentException_ = std::nullopt;
  other.isTaskProcessed_.store(false);
  other.state_ = ThreadState::NoWork;

  return *this;
}

Thread::~Thread() {
  SafeJoin();
  id_ = 0;
}

void MYTHIC_ENGINE_WIN_API Thread::RunTask(RunnableType &&task) {
  if (state_ == ThreadState::Detach)
    return;
  Join();
  task_ = support::utils::Move(task);
  worker_ = std::thread{&Thread::Run, this};
  state_ = ThreadState::Joinable;
}

void MYTHIC_ENGINE_WIN_API Thread::TryJoin() { worker_.join(); }

void Thread::SafeJoin() {
  if (state_ == ThreadState::Detach ||
      state_ == ThreadState::NoWork)
    return;

  std::unique_lock lock{mt_};

  while (IsRunning()) {
    isRunning_.wait(lock);
  }

  if (worker_.joinable()) {
    worker_.join();
  }
}

void MYTHIC_ENGINE_WIN_API Thread::Join() {
  if (state_ == ThreadState::Detach || state_ == ThreadState::NoWork)
    return;

  if (HasException()) {
    std::throw_with_nested(GetException());
  }

  SafeJoin();
}

void Thread::Detach() {
  worker_.detach();
  state_ = ThreadState::Detach;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API Thread::IsJoinable() const noexcept {
  return worker_.joinable();
}

__NODISCARD__ uint64_t MYTHIC_ENGINE_WIN_API Thread::GetId() const noexcept {
  return id_;
}

__NODISCARD__ bool MYTHIC_ENGINE_WIN_API Thread::HasException() const noexcept {
  return currentException_.has_value();
}

__NODISCARD__ std::exception_ptr MYTHIC_ENGINE_WIN_API
Thread::GetException() const noexcept {
  return currentException_.value_or(std::exception_ptr());
}

bool MYTHIC_ENGINE_WIN_API Thread::IsRunning() const noexcept {
  return isTaskProcessed_;
}

void MYTHIC_ENGINE_WIN_API Thread::Run() {
  isTaskProcessed_.store(true);
  currentException_ = std::nullopt;
  try {
    if (task_) {
      task_->Run();
    }
  } catch (...) {
    currentException_ = std::current_exception();
  }
  isTaskProcessed_.store(false);
  isRunning_.notify_one();
}

} // namespace MythicEngine::core::threads