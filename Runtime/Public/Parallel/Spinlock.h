#pragma once

namespace Runtime::Parallel
{
    class Spinlock final
    {
    public:
        Spinlock();

        Spinlock(const Spinlock& other) = delete;
        Spinlock& operator=(const Spinlock& other) = delete;

        Spinlock(Spinlock&& other) noexcept = default;
        Spinlock& operator=(Spinlock&& other) noexcept = default;

        ~Spinlock();

        void lock();

        void unlock();

    private:
        class SpinlockImpl;
        alignas(1) char mem_[1];
        SpinlockImpl* impl_{nullptr};
    };
} // namespace Runtime::Parallel
