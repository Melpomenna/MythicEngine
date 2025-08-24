#pragma once

#include "Config.h"

namespace Runtime::Parallel
{
    struct ThreadOptionsHelper;
}

namespace Runtime::System::Windows
{
    RUNTIME_API void SetUnhandledExceptionHandler(void (*pUnhandledExceptionHandler)(void*));
    RUNTIME_API void GenerateDump(void* pExceptionInfo);
    RUNTIME_API void SetPureVirtualCallHandler(void (*pPureCallHandler)());
    RUNTIME_API void SetThreadAffinityMask(void* pThread, unsigned int mask);
    RUNTIME_NODISCARD RUNTIME_API void* CreateThread(Runtime::Parallel::ThreadOptionsHelper& refThreadOptions);
    RUNTIME_API void JoinThread(void* thread);
    RUNTIME_API void DetachThread(void* thread);
    RUNTIME_API void SuspendThread(void* thread);
    RUNTIME_API void ResumeThread(void* thread);
    RUNTIME_API void EnableLFHHeap();
    RUNTIME_API void YieldThread();
    RUNTIME_NODISCARD RUNTIME_API unsigned long GetCurrentThreadId();
} // namespace Runtime::System::Windows
