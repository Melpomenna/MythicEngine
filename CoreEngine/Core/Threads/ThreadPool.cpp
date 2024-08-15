#include <pch.h>
#include "ThreadPool.h"

#include <Core/Containers/Utils.h>
#include <Core/Functions/FunctionRunnable.h>
#include <Support/Utils/TypeTraits.h>

namespace MythicEngine::core::threads {

StaticThreadPool::StaticThreadPool()
    : StaticThreadPool(std::thread::hardware_concurrency()) {}

StaticThreadPool::StaticThreadPool(size_t workersCount, RunningTag tag)
    : workers_(workersCount), workersCount_(workersCount), isRunning_(false) {
  if (tag == RunningTag::RunAtInit) {
    CreateWorkers();
  }
}

StaticThreadPool::~StaticThreadPool() {
  SafeJoin();
  workers_.Clear();
}

void MYTHIC_ENGINE_WIN_API StaticThreadPool::Join() {
  if (workers_.IsEmpty())
    return;

  ShutDown();

  for (auto i = 0UL; i < workersCount_; ++i) {
    AddTask({});
  }

  FOR_EACH_BEGIN(workers_)
  it->Join();
  FOR_EACH_END
}

void StaticThreadPool::SafeJoin() {
  if (workers_.IsEmpty())
    return;
  ShutDown();

  for (auto i = 0UL; i < workersCount_; ++i) {
    AddTask({});
  }

  FOR_EACH_BEGIN(workers_)
  it->SafeJoin();
  FOR_EACH_END
}

void MYTHIC_ENGINE_WIN_API StaticThreadPool::ShutDown() { isRunning_ = false; }

bool MYTHIC_ENGINE_WIN_API StaticThreadPool::IsRunning() const noexcept {
  return isRunning_;
}

void MYTHIC_ENGINE_WIN_API StaticThreadPool::Run() {
  if (IsRunning())
    return;
  Join();
  CreateWorkers();
}

void MYTHIC_ENGINE_WIN_API
StaticThreadPool::AddTask(RunnableType &&task) noexcept {
  queue_.Push(support::utils::Move(task));
}

void MYTHIC_ENGINE_WIN_API StaticThreadPool::MainThreadWork() noexcept {
  while (isRunning_) {
    auto task = queue_.TopAndRemove();
    if (!task)
      break;
    task->Run();
  }
}

void MYTHIC_ENGINE_WIN_API StaticThreadPool::CreateWorkers() {
  if (isRunning_)
    return;

  isRunning_.store(true);

  for (auto i = 0UL; i < workersCount_; ++i) {
    std::function handler{[this]() noexcept { MainThreadWork(); }};
    using FunctionType = functions::Function<void()>;
    memory::OwnPtr function =
        memory::MakeOwn<IRunnable, FunctionType>(support::utils::Move(handler));
    workers_[i].RunTask(support::utils::Move(function));
  }
}

} // namespace MythicEngine::core::threads