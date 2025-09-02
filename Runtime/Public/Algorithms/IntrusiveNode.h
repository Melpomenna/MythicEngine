#pragma once

#include "Memory/MallocHelper.h"
#include <atomic>

namespace Runtime::Algrotihms::Intrusive
{
    struct IntrusiveLinkedNode
    {
        IntrusiveLinkedNode* next_{nullptr};
    };

    struct IntrusiveTwoLinkedNode
    {
        IntrusiveTwoLinkedNode* next_{nullptr};
        IntrusiveTwoLinkedNode* back_{nullptr};
    };

    template <class = void>
    struct MultiIntrusiveLinkedNode : IntrusiveLinkedNode
    {
    };

    template <class = void>
    struct MultiIntrusiveLinkedTwoNode : IntrusiveTwoLinkedNode
    {
    };

    struct ThreadSafeIntrusiveLinkedNode
    {
        std::atomic<ThreadSafeIntrusiveLinkedNode*> next_{nullptr};
    };

    struct ThreadSafeIntrusiveTwoLinkedNode
    {
        alignas(Memory::CacheLine) std::atomic<ThreadSafeIntrusiveTwoLinkedNode*> next_{nullptr};
        alignas(Memory::CacheLine) std::atomic<ThreadSafeIntrusiveTwoLinkedNode*> back_{nullptr};
    };

    template <class = void>
    struct ThreadSafeMultiIntrusiveLinkedNode : ThreadSafeIntrusiveLinkedNode
    {
    };

    template <class = void>
    struct ThreadSafeMultiIntrusiveLinkedTwoNode : ThreadSafeIntrusiveTwoLinkedNode
    {
    };
} // namespace Runtime::Algrotihms::Intrusive
