#include "Parallel/Spinlock.h"
#include <atomic>
#include "System/SystemHelper.h"

namespace Runtime::Parallel
{
    class Spinlock::SpinlockImpl final
    {
    public:
        SpinlockImpl() = default;

        SpinlockImpl(const SpinlockImpl& other) = default;
        SpinlockImpl& operator=(const SpinlockImpl& other) = default;

        SpinlockImpl(SpinlockImpl&& other) noexcept = default;
        SpinlockImpl& operator=(SpinlockImpl&& other) noexcept = default;

        ~SpinlockImpl() = default;

        void lock()
        {
            while (true)
            {
                while (locker_.load(std::memory_order_acquire))
                {
                    System::YieldCurrentProcessor();
                }

                bool expected = false;
                if (locker_.compare_exchange_weak(expected, true, std::memory_order_acquire, std::memory_order_relaxed))
                {
                    System::YieldThread();
                    break;
                }
            }
        }

        void unlock()
        {
            locker_.store(false, std::memory_order_release);
        }

    private:
        std::atomic<bool> locker_{false};
    };

    Spinlock::Spinlock()
    {
        impl_ = new (mem_) SpinlockImpl();
    }

    Spinlock::~Spinlock()
    {
        impl_->~SpinlockImpl();
    }

    void Spinlock::lock()
    {
        impl_->lock();
    }

    void Spinlock::unlock()
    {
        impl_->unlock();
    }
} // namespace Runtime::Parallel
