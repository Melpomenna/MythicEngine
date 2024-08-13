#pragma once

#include <Support/Utils/Utils.h>
#include <memory>

namespace MythicEngine::support::utils {

template <class Handler, class Dispatcher>
class MYTHIC_ENGINE_EXPORT Subscription final {
  using HandlerPtrType = Handler *;
  using DispatcherPtrType = Dispatcher *;

public:
  Subscription();
  explicit Subscription(HandlerPtrType, DispatcherPtrType);

  Subscription(const Subscription &) = default;
  Subscription &operator=(const Subscription &) = default;

  Subscription(Subscription &&) noexcept = default;
  Subscription &operator=(Subscription &) noexcept = default;

  ~Subscription();

  void Subscribe(HandlerPtrType, DispatcherPtrType);
  void Subscribe();
  void Unsubscribe();

private:
  bool isSubscribed_{false};
  HandlerPtrType handler_;
  DispatcherPtrType dispatcher_;
};

template <class Handler, class Dispatcher> class WeakSubscription final {
  using HandlerPtrType = std::weak_ptr<Handler>;
  using DispatcherPtrType = std::weak_ptr<Dispatcher>;

public:
  WeakSubscription();
  explicit WeakSubscription(const HandlerPtrType &, const DispatcherPtrType &);
  ~WeakSubscription();

  void Subscribe(HandlerPtrType, DispatcherPtrType);
  void Subscribe();
  void Unsubscribe();

private:
  bool isSubscribed_{false};
  HandlerPtrType handler_;
  DispatcherPtrType dispatcher_;
};

template <class Handler, class Dispatcher>
Subscription<Handler, Dispatcher>::Subscription()
    : handler_(nullptr), dispatcher_(nullptr) {}

template <class Handler, class Dispatcher>
Subscription<Handler, Dispatcher>::Subscription(HandlerPtrType handler,
                                                DispatcherPtrType dispatcher)
    : handler_(handler), dispatcher_(dispatcher) {
  Subscribe();
}

template <class Handler, class Dispatcher>
Subscription<Handler, Dispatcher>::~Subscription() {
  Unsubscribe();
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API Subscription<Handler, Dispatcher>::Subscribe(
    HandlerPtrType handler, DispatcherPtrType dispatcher) {
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
  dispatcher_->RemoveHandler(handler_);
  dispatcher_ = nullptr;
  handler_ = nullptr;
  isSubscribed_ = false;
}

template <class Handler, class Dispatcher>
WeakSubscription<Handler, Dispatcher>::WeakSubscription()
    : handler_(HandlerPtrType()), dispatcher_(DispatcherPtrType()) {}

template <class Handler, class Dispatcher>
WeakSubscription<Handler, Dispatcher>::WeakSubscription(
    const HandlerPtrType &handler, const DispatcherPtrType &dispatcher)
    : handler_(handler), dispatcher_(dispatcher), isSubscribed_(true) {
  Subscribe();
}

template <class Handler, class Dispatcher>
WeakSubscription<Handler, Dispatcher>::~WeakSubscription() {
  Unsubscribe();
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API WeakSubscription<Handler, Dispatcher>::Subscribe(
    HandlerPtrType handler, DispatcherPtrType dispatcher) {
  handler_ = handler;
  dispatcher_ = dispatcher;
  Subscribe();
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API WeakSubscription<Handler, Dispatcher>::Subscribe() {
  if (isSubscribed_ || dispatcher_.expired())
    return;
  dispatcher_.lock()->AddHandler(handler_);
  isSubscribed_ = true;
}

template <class Handler, class Dispatcher>
void MYTHIC_ENGINE_WIN_API
WeakSubscription<Handler, Dispatcher>::Unsubscribe() {
  if (!isSubscribed_ || dispatcher_.expired())
    return;
  isSubscribed_ = false;
  dispatcher_.lock()->RemoveHandler(handler_);
  dispatcher_.reset();
  handler_.reset();
}

} // namespace MythicEngine::support::utils