#pragma once

#include <Support/Utils/NonCopy.h>
#include <Support/Utils/Utils.h>

#include <Core/Containers/MoveVector.h>
#include <Core/Memory/OwnPtr.h>
#include <Core/Threads/IRunnable.h>
#include <Core/Threads/Queue.h>
#include <Core/Threads/Thread.h>

#include <atomic>

namespace MythicEngine::core::threads {

class StaticThreadPool final : public support::utils::NonCopy {

public:
  using RunnableType = memory::OwnPtr<IRunnable>;

  enum class RunningTag { DeferRun, RunAtInit };

public:
  MYTHIC_ENGINE_EXPORT StaticThreadPool();
  MYTHIC_ENGINE_EXPORT explicit StaticThreadPool(
      size_t, RunningTag tag = RunningTag::RunAtInit);
  MYTHIC_ENGINE_EXPORT ~StaticThreadPool();

public:
  MYTHIC_ENGINE_EXPORT void Join();
  MYTHIC_ENGINE_EXPORT void SafeJoin();
  MYTHIC_ENGINE_EXPORT void ShutDown();
  MYTHIC_ENGINE_EXPORT void AddTask(RunnableType &&) noexcept;
  MYTHIC_ENGINE_EXPORT bool IsRunning() const noexcept;
  MYTHIC_ENGINE_EXPORT void Run();

private:
  MYTHIC_ENGINE_EXPORT void MainThreadWork() noexcept;
  MYTHIC_ENGINE_EXPORT void CreateWorkers();

public:
  Queue<RunnableType> queue_;
  containers::MoveVector<Thread> workers_;
  std::exception_ptr currentException_;
  std::atomic<size_t> workersCount_;
  std::atomic_bool isRunning_;
};

} // namespace MythicEngine::core::threads