#include "SystemHelper.h"
#include "Windows/System.h"

namespace Runtime::System
{
    void SetUnhandledExceptionHandler(void (*unhandledExceptionHandler)(void*))
    {
        Windows::SetUnhandledExceptionHandler(unhandledExceptionHandler);
    }

    void GenerateDump(void* exceptionInfo)
    {
        Windows::GenerateDump(exceptionInfo);
    }

    void SetPureVirtualCallHandler(void (*pureVirtualCallHandler)())
    {
        Windows::SetPureVirtualCallHandler(pureVirtualCallHandler);
    }

    void SetThreadAffinityMask(void* thread, unsigned int mask)
    {
        Windows::SetThreadAffinityMask(thread, mask);
    }

    void EnableLFHHeap()
    {
        Windows::EnableLFHHeap();
    }

    void* CreateThread(Runtime::Parallel::ThreadOptionsHelper& threadOptions)
    {
        return Windows::CreateThread(threadOptions);
    }

    void JoinThread(void* thread)
    {
        Windows::JoinThread(thread);
    }

    void DetachThread(void* thread)
    {
        Windows::DetachThread(thread);
    }

    void SuspendThread(void* thread)
    {
        Windows::SuspendThread(thread);
    }

    void ResumeThread(void* thread)
    {
        Windows::ResumeThread(thread);
    }

    void YieldThread()
    {
        Windows::YieldThread();
    }

    unsigned long GetCurrentThreadId()
    {
        return Windows::GetCurrentThreadId();
    }

    void YieldCurrentProcessor()
    {
        Windows::YieldCurrentProcessor();
    }
} // namespace Runtime::System
