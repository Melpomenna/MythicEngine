#pragma once

#include "Memory/MallocHelper.h"
#include <vector>
#include "Config.h"
#include "Parallel/ThreadHelper.h"
#include "Types.h"

namespace Runtime::Parallel
{
    class DynamicThreadPool final
    {
    public:
        DynamicThreadPool();

        DynamicThreadPool(const DynamicThreadPool& other) = delete;
        DynamicThreadPool& operator=(const DynamicThreadPool& other) = delete;

        DynamicThreadPool(DynamicThreadPool&& other) = default;
        DynamicThreadPool& operator=(DynamicThreadPool&& other) = default;

        ~DynamicThreadPool();

        DynamicThreadPool& CreateThread(const ThreadOptionsHelper& options, UI8 mask = 0x0);

        Int32 SendTask();

        void WaitTask(Int32 id);

        void WaitAll();

    private:
        void Join();

        void Schedule();

        std::vector<Thread, Memory::DefaultAllocator<Thread>> workers_;
    };
} // namespace Runtime::Parallel
