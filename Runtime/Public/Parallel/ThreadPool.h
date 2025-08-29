#pragma once


#include "Memory/MallocHelper.h"
#include <vector>
#include "Config.h"
#include "Parallel/ThreadHelper.h"

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

        DynamicThreadPool& CreateThread(const ThreadOptionsHelper& options, int mask = 0x0);

        int SendTask();

        void WaitTask(int id);

        void WaitAll();

    private:
        void Join();

        void Shcedule();

        std::vector<Thread, Memory::Mimalloc::MiAllocator<Thread>> workers_;
    };
} // namespace Runtime::Parallel
