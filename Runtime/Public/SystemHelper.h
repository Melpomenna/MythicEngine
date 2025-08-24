#pragma once

#include "Config.h"

namespace Runtime::Parallel
{
    struct ThreadOptionsHelper;
}

namespace Runtime::System
{
    RUNTIME_API void SetUnhandledExceptionHandler(void (*unhandledExceptionHandler)(void*));
    RUNTIME_API void GenerateDump(void* exceptionInfo);
    RUNTIME_API void SetPureVirtualCallHandler(void (*pureVirtualCallHandler)());
    RUNTIME_API void SetThreadAffinityMask(void* thread, unsigned int mask);
    RUNTIME_NODISCARD RUNTIME_API void* CreateThread(Runtime::Parallel::ThreadOptionsHelper& threadOptions);
    RUNTIME_API void JoinThread(void* thread);
    RUNTIME_API void DetachThread(void* thread);
    RUNTIME_API void SuspendThread(void* thread);
    RUNTIME_API void ResumeThread(void* thread);
    RUNTIME_API void EnableLFHHeap();
    RUNTIME_API void YieldThread();
    RUNTIME_NODISCARD RUNTIME_API unsigned long GetCurrentThreadId();

} // namespace Runtime::System
