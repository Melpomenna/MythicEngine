#pragma once

#include <Core/Threads/IRunnable.h>
#include <Support/Utils/Utils.h>
#include <functional>
#include <type_traits>

namespace MythicEngine::core::functions {

template <class Func, class... Args>
class FunctionRunnable final : public threads::IRunnable {
public:
  using FunctionType = Func;

public:
  MYTHIC_ENGINE_EXPORT FunctionRunnable() = default;
  MYTHIC_ENGINE_EXPORT ~FunctionRunnable() override = default;

  MYTHIC_ENGINE_EXPORT FunctionRunnable(const FunctionRunnable &) = default;
  MYTHIC_ENGINE_EXPORT FunctionRunnable &
  operator=(const FunctionRunnable &) = default;

  MYTHIC_ENGINE_EXPORT FunctionRunnable(FunctionRunnable &&) noexcept = default;
  MYTHIC_ENGINE_EXPORT FunctionRunnable &
  operator=(FunctionRunnable &&) noexcept = default;

  MYTHIC_ENGINE_EXPORT explicit FunctionRunnable(FunctionType &&, Args &&...);

public:
  MYTHIC_ENGINE_EXPORT void Run() override;

private:
  FunctionType function_;
  std::tuple<Args...> args_;
};

template <class Func, class... Args>
FunctionRunnable<Func, Args...>::FunctionRunnable(FunctionType &&func,
                                                  Args &&...args)
    : function_(std::forward<Func>(func)),
      args_(std::make_tuple(std::forward<Args>(args)...)) {}

template <class Func, class... Args>
void MYTHIC_ENGINE_WIN_API FunctionRunnable<Func, Args...>::Run() {
  std::apply(function_, args_);
}

template <class Func>
class MYTHIC_ENGINE_EXPORT FunctionRunnable<Func, void> final
    : public threads::IRunnable {
public:
  using FunctionType = Func;

public:
  FunctionRunnable() = default;
  ~FunctionRunnable() override = default;

  FunctionRunnable(const FunctionRunnable &) = default;
  FunctionRunnable &operator=(const FunctionRunnable &) = default;

  FunctionRunnable(FunctionRunnable &&) noexcept = default;
  FunctionRunnable &operator=(FunctionRunnable &&) noexcept = default;

  explicit FunctionRunnable(FunctionType &&);

public:
  void Run() override;

private:
  FunctionType function_;
};

template <class Func>
FunctionRunnable<Func, void>::FunctionRunnable(FunctionType &&func)
    : function_(std::forward<Func>(func)) {}

template <class Func>
void MYTHIC_ENGINE_WIN_API FunctionRunnable<Func, void>::Run() {
  function_();
}

template <class T> using Function = FunctionRunnable<std::function<T>, void>;

} // namespace MythicEngine::core::functions
