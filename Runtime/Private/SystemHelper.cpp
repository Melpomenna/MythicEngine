#include "SystemHelper.h"
#include "Windows/System.h"
#include <MacOS/System.h>

namespace Runtime::System
{
    void SetUnhandledExceptionHandler(UnhandledHandler handler)
    {
        Windows::SetUnhandledExceptionHandler(handler);
    }

    void GenerateDump(void* exceptionInfo)
    {
        #if defined(_WIN32) || defined(_WIN64)
        Windows::GenerateDump(exceptionInfo);
        #endif
    }

    void SetPureVirtualCallHandler(void (*pureVirtualCallHandler)())
    {
        #if defined(_WIN32) || defined(_WIN64)
        Windows::SetPureVirtualCallHandler(pureVirtualCallHandler);
        #else
        MacOS::SetPureVirtualCallHandler(pureVirtualCallHandler);
        #endif
    }

    void SetThreadAffinityMask(void* thread, unsigned int mask)
    {
        #if defined(_WIN32) || defined(_WIN64)
        Windows::SetThreadAffinityMask(thread, mask);
        #endif
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
