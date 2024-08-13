#pragma once

#include <Support/Utils/Utils.h>

namespace MythicEngine::core::threads {

class MYTHIC_ENGINE_EXPORT IRunnable {
public:
  IRunnable() = default;
  virtual ~IRunnable() = default;

  IRunnable(const IRunnable &) = default;
  IRunnable &operator=(const IRunnable &) = default;

  IRunnable(IRunnable &&) noexcept = default;
  IRunnable &operator=(IRunnable &&) noexcept = default;

public:
  virtual void Run() = 0;
  virtual void operator()() { Run(); }
};

} // namespace MythicEngine::core::threads