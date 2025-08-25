#include "ThreadHelper.h"
#include "LogWrapper.h"

namespace Runtime::Parallel
{

    Thread::~Thread()
    {
        if (IsJoinable() && options_.enableAutoJoin)
        {
            Join();
            return;
        }

        if (IsJoinable())
        {
            RUNTIME_LOG_CRITICAL(Runtime::LogHelper::FileLoggerName, Runtime::StaticString::FailedToJoinThreadMessage,
                                 options_.threadId);
            RUNTIME_ASSERT(!IsJoinable());
            std::terminate();
        }
    }

    void Thread::Join()
    {
        if (!IsJoinable())
        {
            RUNTIME_ASSERT(IsJoinable());
            RUNTIME_LOG_CRITICAL(Runtime::LogHelper::FileLoggerName, Runtime::StaticString::ThreadIsNotJoinableMessage);
            std::terminate();
        }
        System::JoinThread(handle_);
        handle_ = nullptr;
        options_ = {};
    }

    void Thread::Start()
    {
        if (!IsJoinable())
        {
            RUNTIME_LOG_CRITICAL(Runtime::LogHelper::FileLoggerName, Runtime::StaticString::ThreadIsNotJoinableMessage);
            RUNTIME_ASSERT(IsJoinable());
            std::terminate();
        }
        if (options_.isSuspendedOnStart)
        {
            System::ResumeThread(handle_);
        }
    }

    void Thread::Detach()
    {
        if (!IsJoinable())
        {
            RUNTIME_LOG_CRITICAL(Runtime::LogHelper::FileLoggerName, Runtime::StaticString::ThreadIsNotJoinableMessage);
            RUNTIME_ASSERT(IsJoinable());
            std::terminate();
        }
        System::DetachThread(handle_);
        handle_ = nullptr;
        options_ = {};
    }

    std::exception_ptr Thread::CurrentException() noexcept
    {
        std::shared_lock lock{valueProvaider_.mt, std::try_to_lock_t{}};
        return valueProvaider_.provaider.error_or(nullptr);
    }

    bool Thread::HasException() noexcept
    {
        std::shared_lock lock{valueProvaider_.mt, std::try_to_lock_t{}};
        return valueProvaider_.provaider.error_or(nullptr) != nullptr;
    }

    unsigned long Thread::GetThreadId() const noexcept
    {
        return options_.threadId;
    }

    bool Thread::IsJoinable() const noexcept
    {
        return options_.threadId != 0;
    }

    void* Thread::GetHandle() const noexcept
    {
        return handle_;
    }

    const ThreadOptionsHelper& Thread::GetOptions() const noexcept
    {
        return options_;
    }

    void Thread::MessageOnThreadCreate(int threadId)
    {

        RUNTIME_CONSOLE_LOG_INFO(Runtime::StaticString::ThreadCreateSuccessMessage, threadId);
        RUNTIME_LOG_INFO(Runtime::LogHelper::FileLoggerName, Runtime::StaticString::ThreadCreateSuccessMessage,
                         threadId);
    }

    void Thread::MessageOnThreadCreateFailed([[maybe_unused]] void* handle)
    {
        RUNTIME_ASSERT(handle);
        RUNTIME_CONSOLE_LOG_CRITICAL(Runtime::StaticString::CannotCreateThreadMessage);
        RUNTIME_LOG_CRITICAL(Runtime::LogHelper::FileLoggerName, Runtime::StaticString::CannotCreateThreadMessage);
    }

    void Thread::MessageOnThreadException()
    {
        RUNTIME_LOG_CRITICAL(Runtime::LogHelper::FileLoggerName,
                             Runtime::StaticString::ExceptionInThreadFunctionMessage);
        RUNTIME_CONSOLE_LOG_CRITICAL(Runtime::StaticString::ExceptionInThreadFunctionMessage);
    }

} // namespace Runtime::Parallel
