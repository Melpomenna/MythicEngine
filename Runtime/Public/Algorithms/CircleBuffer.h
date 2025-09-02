#pragma once

#include "Memory/MallocHelper.h"
#include <atomic>
#include <type_traits>
#include "Config.h"
#include "IntrusiveNode.h"
#include "System/SystemHelper.h"

namespace Runtime::Algrotihms
{
    template <class T>
    concept SingleNodeLinkage = std::is_base_of_v<Intrusive::IntrusiveLinkedNode, T>;

    template <class T>
    concept ThreadSafeNodeLinkage = std::is_base_of_v<Intrusive::ThreadSafeIntrusiveLinkedNode, T>;

    template <SingleNodeLinkage T>
    class IntrusiveCircleBuffer
    {
    public:
        IntrusiveCircleBuffer() = default;

        IntrusiveCircleBuffer(const IntrusiveCircleBuffer& other) = delete;
        IntrusiveCircleBuffer& operator=(const IntrusiveCircleBuffer& other) = delete;

        IntrusiveCircleBuffer(IntrusiveCircleBuffer&&) noexcept = default;
        IntrusiveCircleBuffer& operator=(IntrusiveCircleBuffer&&) noexcept = default;

        virtual ~IntrusiveCircleBuffer() = default;

        void push_back(T& next)
        {
            if (!begin_ && !end_)
                RUNTIME_UNLIKELY
                {
                    begin_ = std::addressof(next);
                    end_ = std::addressof(next);
                    begin_->next_ = end_;
                    end_->next_ = begin_;
                }
            else
            {
                end_->next_ = std::addressof(next);
                end_ = end_->next_;
                end_->next_ = begin_;
            }
        }

        void Shift()
        {
            if (begin_)
                begin_ = begin_->next_;
        }

        T* pop_front()
        {
            T* result = nullptr;
            if (begin_)
            {
                result = begin_;
                begin_ = begin_->next_;
                end_->next_ = begin_;
            }
            return result;
        }

    private:
        T* begin_{nullptr};
        T* end_{nullptr};
    };

    template <ThreadSafeNodeLinkage T>
    class ThreadSafeIntrusiveCircleBuffer
    {
    public:
        ThreadSafeIntrusiveCircleBuffer() = default;

        ThreadSafeIntrusiveCircleBuffer(const ThreadSafeIntrusiveCircleBuffer& other) = delete;
        ThreadSafeIntrusiveCircleBuffer& operator=(const ThreadSafeIntrusiveCircleBuffer& other) = delete;

        ThreadSafeIntrusiveCircleBuffer(ThreadSafeIntrusiveCircleBuffer&&) noexcept = default;
        ThreadSafeIntrusiveCircleBuffer& operator=(ThreadSafeIntrusiveCircleBuffer&&) noexcept = default;

        virtual ~ThreadSafeIntrusiveCircleBuffer() = default;

        bool try_push(T& next)
        {
            T* ptr = std::addressof(next);
            T* current = begin_.load(std::memory_order_relaxed);
            if (!current)
                RUNTIME_UNLIKELY
                {
                    if (begin_.compare_exchange_strong(current, ptr) && end_.compare_exchange_strong(current, ptr))
                    {
                        T* end = end_.load(std::memory_order_relaxed);
                        current = begin_.load(std::memory_order_acquire);
                        current->next_.store(end_.load(std::memory_order_relaxed));
                        end->next_.store(current, std::memory_order_release);
                        return true;
                    }
                    return false;
                }

            current = end_.load(std::memory_order_relaxed);
            current->next_.store(ptr, std::memory_order_acquire);
            ptr->next_.store(begin_.load(std::memory_order_acquire));
            end_.store(ptr, std::memory_order_release);
            return true;
        }

        void push_back(T& next)
        {
            while (!try_push(next))
            {
                System::YieldCurrentProcessor();
            }
        }

        void shift()
        {
            auto current = begin_.load(std::memory_order_acquire);
            while (current &&
                   !begin_.compare_exchange_weak(current, current->next_.load(std::memory_order_acquire),
                                                 std::memory_order_release, std::memory_order_acquire))
            {
                if (!current)
                    RUNTIME_UNLIKELY
                    {
                        break;
                    }
                System::YieldCurrentProcessor();
            }
        }

        T* pop_front()
        {
            T* result = begin_.load(std::memory_order_relaxed);
            if (end_.compare_exchange_strong(result, nullptr))
            {
                begin_.store(nullptr, std::memory_order_release);
                return result;
            }
            shift();
            end_.load(std::memory_order_acquire)
                ->next_.store(begin_.load(std::memory_order_acquire), std::memory_order_release);
            return result;
        }

    private:
        alignas(Memory::CacheLine) std::atomic<T*> begin_{nullptr};
        alignas(Memory::CacheLine) std::atomic<T*> end_{nullptr};
    };

    template <class T, bool V>
    using CircleBase = std::conditional_t<V, ThreadSafeIntrusiveCircleBuffer<T>, IntrusiveCircleBuffer<T>>;

    template <class T, bool IsThreadSafe = false>
    class CircleBuffer : protected CircleBase<T, IsThreadSafe>
    {
    public:
        using Base = CircleBase<T, IsThreadSafe>;

        void push_back(T& next)
        {
            Base::push_back(next);
        }

        void Shift()
        {
            Base::Shift();
        }

        T* pop_front()
        {
            return Base::pop_front();
        }
    };
} // namespace Runtime::Algrotihms
