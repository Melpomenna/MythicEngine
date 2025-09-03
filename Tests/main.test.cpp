#pragma warning(disable : 4389 4245 4081 4389)
#include "Memory/MallocHelper.h"
#include <gtest/gtest.h>
#include "CoreRuntime.h"
#include "Parallel/ThreadHelper.h"

TEST(ThreadTest, StartAndJoinOneThreadSuccess)
{
    UI64 mainThreadId = Runtime::System::GetCurrentThreadId();
    Runtime::Parallel::Thread thread(
        [](int start)
        {
            int sum = start;
            for (int i = 0; i < 100; ++i)
            {
                sum += i;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(250));
            return sum;
        },
        Runtime::Parallel::ThreadOptionsHelper{}, 500);

    EXPECT_NE(thread.GetThreadId(), mainThreadId);
    EXPECT_TRUE(thread.GetHandle() != nullptr);
    thread.Join();
    EXPECT_EQ(thread.GetThreadId(), 0);
    EXPECT_TRUE(thread.GetHandle() == nullptr);
    EXPECT_TRUE(thread.CurrentException() == nullptr);
    EXPECT_FALSE(thread.HasException());
    EXPECT_TRUE(thread.GetValue<int>() != nullptr);
    EXPECT_EQ(*thread.GetValue<int>(), 4950 + 500);
}

TEST(ThreadTest, StartAndJoinWithException)
{
    UI64 mainThreadId = Runtime::System::GetCurrentThreadId();
    Runtime::Parallel::Thread worker{[]()
                                     {
                                         std::this_thread::sleep_for(std::chrono::microseconds(250));
                                         throw std::runtime_error{"Something"};
                                     },
                                     Runtime::Parallel::ThreadOptionsHelper{}};
    EXPECT_NE(worker.GetThreadId(), mainThreadId);
    EXPECT_TRUE(worker.GetHandle() != nullptr);
    worker.Join();
    EXPECT_EQ(worker.GetThreadId(), 0);
    EXPECT_TRUE(worker.GetHandle() == nullptr);
    EXPECT_TRUE(worker.CurrentException() != nullptr);
    EXPECT_TRUE(worker.HasException());

    try
    {
        std::rethrow_exception(worker.CurrentException());
    }
    catch (const std::runtime_error&)
    {
        EXPECT_TRUE(true);
    }
    catch (...)
    {
        EXPECT_TRUE(false);
    }
}

TEST(ThreadTest, StartAndDetachThread)
{
    UI64 mainThreadId = Runtime::System::GetCurrentThreadId();
    Runtime::Parallel::Thread worker{[]()
                                     {
                                         std::this_thread::sleep_for(std::chrono::seconds(2));
                                         return 0;
                                     },
                                     Runtime::Parallel::ThreadOptionsHelper{}};
    EXPECT_NE(worker.GetThreadId(), mainThreadId);
    EXPECT_TRUE(worker.GetHandle() != nullptr);
    worker.Detach();
    EXPECT_EQ(worker.GetThreadId(), 0);
    EXPECT_TRUE(worker.GetHandle() == nullptr);
    EXPECT_TRUE(worker.CurrentException() == nullptr);
    EXPECT_FALSE(worker.HasException());
    while (worker.GetValue<int>() == nullptr)
    {
        Runtime::System::YieldCurrentProcessor();
    }
    EXPECT_TRUE(worker.GetValue<int>() != nullptr);
    EXPECT_EQ(*worker.GetValue<int>(), 0);
}

TEST(ThreadTest, StartAndDetachThreadAndSetValueByReference)
{
    UI64 mainThreadId = Runtime::System::GetCurrentThreadId();
    int result = 1;
    Runtime::Parallel::Thread worker{[](int& result)
                                     {
                                         std::this_thread::sleep_for(std::chrono::seconds(2));
                                         result = 0;
                                         return 0;
                                     },
                                     Runtime::Parallel::ThreadOptionsHelper{}, std::ref(result)};
    EXPECT_NE(worker.GetThreadId(), mainThreadId);
    EXPECT_TRUE(worker.GetHandle() != nullptr);
    worker.Detach();
    EXPECT_EQ(worker.GetThreadId(), 0);
    EXPECT_TRUE(worker.GetHandle() == nullptr);
    EXPECT_TRUE(worker.CurrentException() == nullptr);
    EXPECT_FALSE(worker.HasException());
    while (worker.GetValue<int>() == nullptr)
    {
        Runtime::System::YieldCurrentProcessor();
    }
    EXPECT_TRUE(worker.GetValue<int>() != nullptr);
    EXPECT_EQ(*worker.GetValue<int>(), 0);
    EXPECT_NE(result, 1);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(*worker.GetValue<int>(), result);
}

TEST(ThreadTest, StartThreadAndAutoJoin)
{
    UI64 mainThreadId = Runtime::System::GetCurrentThreadId();
    int result = 1;
    Runtime::Parallel::Thread worker{[](int& result)
                                     {
                                         std::this_thread::sleep_for(std::chrono::seconds(2));
                                         result = 0;
                                         return 0;
                                     },
                                     Runtime::Parallel::ThreadOptionsHelper{.enableAutoJoin = true}, std::ref(result)};
    EXPECT_NE(worker.GetThreadId(), mainThreadId);
    EXPECT_TRUE(worker.GetHandle() != nullptr);
    EXPECT_TRUE(worker.CurrentException() == nullptr);
    EXPECT_FALSE(worker.HasException());
    while (worker.GetValue<int>() == nullptr)
    {
        Runtime::System::YieldCurrentProcessor();
    }
    EXPECT_TRUE(worker.GetValue<int>() != nullptr);
    EXPECT_EQ(*worker.GetValue<int>(), 0);
    EXPECT_NE(result, 1);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(*worker.GetValue<int>(), result);
}

int main(int argc, char** argv)
{
    Runtime::CoreRuntime runtime;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
