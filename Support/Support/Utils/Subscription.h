#pragma once

#include <Support/Utils/TypeTraits.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::support::utils {

template <class Handler, class Dispatcher> class Subscription final {
  using HandlerPtrType = Handler;
  using DispatcherPtrType = Dispatcher;

public:
  MYTHIC_ENGINE_EXPORT Subscription();
  MYTHIC_ENGINE_EXPORT explicit Subscription(const HandlerPtrType &,
                                             const DispatcherPtrType &);
  MYTHIC_ENGINE_EXPORT explicit Subscription(HandlerPtrType &&,
                                             DispatcherPtrType &&);

  MYTHIC_ENGINE_EXPORT Subscription(const Subscription &) = default;
  MYTHIC_ENGINE_EXPORT Subscription &operator=(const Subscription &) = default;

  MYTHIC_ENGINE_EXPORT Subscription(Subscription &&) noexcept = default;
  MYTHIC_ENGINE_EXPORT Subscription &
  operator=(Subscription &&) noexcept = default;

  MYTHIC_ENGINE_EXPORT ~Subscription();

  MYTHIC_ENGINE_EXPORT void Subscribe(const HandlerPtrType &,
                                      const DispatcherPtrType &);
  MYTHIC_ENGINE_EXPORT void Subscribe(HandlerPtrType &&, DispatcherPtrType &&);
  MYTHIC_ENGINE_EXPORT void Subscribe();
  MYTHIC_ENGINE_EXPORT void Unsubscribe();

private:
  bool isSubscribed_{false};
  HandlerPtrType handler_;
  DispatcherPtrType dispatcher_;
};

template <class Handler, class Dispatcher>
Subscription<Handler, Dispatcher>::Subscription()
    : handler_(nullptr), dispatcher_(nullptr) {}

template <class Handler, class Dispatcher>
Subscription<Handler, Dispatcher>::Subscription(
    const HandlerPtrType &handler, const DispatcherPtrType &dispatcher)
    : handler_(handler), dispatcher_(dispatcher) {
  Subscribe();
}

template <class Handler, class Dispatcher>
Subscription<Handler, Dispatcher>::Subscription(HandlerPtrType &&handler,
                                                DispatcherPtrType &&dispatcher)
    : handler_(Move(handler)), dispatcher_(Move(dispatcher)) {
  Subscribe();
}

template <class Handler, class Dispatcher>
Subscription<Handler, Dispatcher>::~Subscription() {
  Unsubscribe();
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API Subscription<Handler, Dispatcher>::Subscribe(
    HandlerPtrType &&handler, DispatcherPtrType &&dispatcher) {
  handler_ = Move(handler);
  dispatcher_ = Move(dispatcher);
  Subscribe();
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API Subscription<Handler, Dispatcher>::Subscribe(
    const HandlerPtrType &handler, const DispatcherPtrType &dispatcher) {
  handler_ = handler;
  dispatcher_ = dispatcher;
  Subscribe();
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API Subscription<Handler, Dispatcher>::Subscribe() {
  if (isSubscribed_)
    return;
  dispatcher_->AddHandler(handler_);
  isSubscribed_ = true;
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API Subscription<Handler, Dispatcher>::Unsubscribe() {
  if (!isSubscribed_)
    return;
  if (dispatcher_)
    dispatcher_->RemoveHandler(handler_);
  isSubscribed_ = false;
}

} // namespace MythicEngine::support::utils