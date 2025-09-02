#include "System/SystemHelper.h"
#include "Singletone.h"
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

    void SetThreadAffinityMask(Handle thread, UI8 mask)
    {
        Windows::SetThreadAffinityMask(thread, mask);
    }

    void EnableLFHHeap()
    {
        Windows::EnableLFHHeap();
    }

    Handle CreateThread(Runtime::Parallel::ThreadOptionsHelper& threadOptions)
    {
        return Windows::CreateThread(threadOptions);
    }

    void JoinThread(Handle thread)
    {
        Windows::JoinThread(thread);
    }

    void DetachThread(Handle thread)
    {
        Windows::DetachThread(thread);
    }

    void SuspendThread(Handle thread)
    {
        Windows::SuspendThread(thread);
    }

    void ResumeThread(Handle thread)
    {
        Windows::ResumeThread(thread);
    }

    void YieldThread()
    {
        Windows::YieldThread();
    }

    UI64 GetCurrentThreadId()
    {
        return Windows::GetCurrentThreadId();
    }

    void YieldCurrentProcessor()
    {
        Windows::YieldCurrentProcessor();
    }

    UI16 HardwareConcurent()
    {
        return static_cast<UI16>(Runtime::SingletoneHelper<ProcessInfoHelper>::Instance()->logicalCoresProcessCount);
    }

    void InitProcessInfoHelper(ProcessInfoHelper* processInfoHelper)
    {
        Windows::InitProcessInfoHelper(processInfoHelper);
    }

    void SetThreadPriority(Handle handle, ThreadPriority priority)
    {
        Windows::SetThreadPriority(handle, priority);
    }
} // namespace Runtime::System
