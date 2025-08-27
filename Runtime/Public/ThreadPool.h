#pragma once


#include "Config.h"
#include "ThreadHelper.h"

namespace Runtime::Parallel
{
    template <int ThreadsCount = Thread::HardwareConcurent()>
    class StaticThreadPool final
    {
    public:
        StaticThreadPool(ThreadOptionsHelper&& options = ThreadOptionsHelper{.enableAutoJoin = true})
        {
            for (int i = 0; i < ThreadCount; ++i)
            {
                workers = Thread(&StaticThreadPool::Schedule, options, this);
            }
        }

        StaticThreadPool(const StaticThreadPool& other) = delete;
        StaticThreadPool& operator=(const StaticThreadPool& other) = delete;

    private:
        void Schedule()
        {
        }
        Thread workers[ThreadCount];
    };

    class DynamicThreadPool
    {
    public:
    private:
    };
} // namespace Runtime::Parallel
