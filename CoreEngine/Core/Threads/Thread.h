#pragma once

#include <Core/Memory/OwnPtr.h>
#include <Core/Threads/IRunnable.h>
#include <Support/Utils/NonCopy.h>
#include <Support/Utils/Utils.h>
#include <atomic>
#include <condition_variable>
#include <optional>
#include <shared_mutex>
#include <thread>

namespace MythicEngine::core::threads {

class Thread final : public support::utils::NonCopy {
public:
  using RunnableType = memory::OwnPtr<IRunnable>;
  enum class ThreadState { Joinable, Detach, NoWork };

public:
  MYTHIC_ENGINE_EXPORT Thread();
  MYTHIC_ENGINE_EXPORT explicit Thread(RunnableType &&);
  MYTHIC_ENGINE_EXPORT explicit Thread(const RunnableType &);
  MYTHIC_ENGINE_EXPORT ~Thread() override;

  MYTHIC_ENGINE_EXPORT Thread(Thread &&) noexcept;
  MYTHIC_ENGINE_EXPORT Thread &operator=(Thread &&) noexcept;

public:
  MYTHIC_ENGINE_EXPORT void RunTask(RunnableType &&worker);
  MYTHIC_ENGINE_EXPORT void TryJoin();
  MYTHIC_ENGINE_EXPORT void Join();
  MYTHIC_ENGINE_EXPORT void Detach();
  MYTHIC_ENGINE_EXPORT void SafeJoin();
  MYTHIC_ENGINE_EXPORT bool IsJoinable() const noexcept;
  MYTHIC_ENGINE_EXPORT uint64_t GetId() const noexcept;
  MYTHIC_ENGINE_EXPORT bool HasException() const noexcept;
  MYTHIC_ENGINE_EXPORT std::exception_ptr GetException() const noexcept;
  MYTHIC_ENGINE_EXPORT bool IsRunning() const noexcept;

private:
  void Run();

private:
  RunnableType task_;
  std::thread worker_;
  uint64_t id_;
  std::optional<std::exception_ptr> currentException_;
  std::atomic_bool isTaskProcessed_;
  std::shared_mutex mt_;
  std::condition_variable_any isRunning_;
  ThreadState state_;
};
} // namespace MythicEngine::core::threads