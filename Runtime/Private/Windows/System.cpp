#include "Windows/System.h"
#include "Log/LogWrapper.h"
#include "Parallel/ThreadHelper.h"
#include "StaticString.h"
// clang-format off
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DbgHelp.h>
// clang-format on

#if defined(Yield)
#undef Yield
#endif

namespace Runtime::System::Windows
{
    void SetUnhandledExceptionHandler(void (*pUnhandledExceptionHandler)(void*))
    {
        ::SetUnhandledExceptionFilter(reinterpret_cast<LPTOP_LEVEL_EXCEPTION_FILTER>(pUnhandledExceptionHandler));
    }

    void GenerateDump(void* pExceptionInfo)
    {
        HMODULE hDbgLib = LoadLibrary(StaticString::DbgHelpLibName);

        if (!hDbgLib)
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::CannotLoadDbgHelpLibMessage);
            return;
        }

        typedef BOOL(WINAPI * MiniDumpWriteDumpPtr)(HANDLE hProcess, DWORD ProcessId, HANDLE hFile,
                                                    MINIDUMP_TYPE DumpType,
                                                    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
                                                    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
                                                    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

        auto hMiniDumpWriteDumpFunc = reinterpret_cast<MiniDumpWriteDumpPtr>(
            GetProcAddress(hDbgLib, StaticString::MiniDumpWriteDumpFunctionName));
        if (!hMiniDumpWriteDumpFunc)
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::CannotGetMiniDumpWriteDumpFunctionMessage);
            return;
        }

        SYSTEMTIME st;
        GetSystemTime(&st);

        char dumpFileName[512]{};
        _snprintf_s(dumpFileName, sizeof(dumpFileName), StaticString::DumpFileFormatName, st.wYear, st.wMonth, st.wDay,
                    st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId());

        HANDLE hDumpFile = CreateFile(dumpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);

        if (hDumpFile == INVALID_HANDLE_VALUE)
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToCreateDumpFileMessage);
            return;
        }

        MINIDUMP_EXCEPTION_INFORMATION exceptionParam;
        exceptionParam.ThreadId = GetCurrentThreadId();
        exceptionParam.ExceptionPointers = reinterpret_cast<PEXCEPTION_POINTERS>(pExceptionInfo);
        exceptionParam.ClientPointers = FALSE;
        BOOL result = hMiniDumpWriteDumpFunc(
            GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
            static_cast<MINIDUMP_TYPE>(
                (static_cast<int>(MiniDumpWithIndirectlyReferencedMemory) | static_cast<int>(MiniDumpWithDataSegs))),
            &exceptionParam, nullptr, nullptr);

        CloseHandle(hDumpFile);
        FreeLibrary(hDbgLib);

        if (!result)
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToCreateDumpFileMessage);
        }
    }

    void SetPureVirtualCallHandler(void (*pPureCallHandler)())
    {
        _set_purecall_handler(pPureCallHandler);
    }

    void SetThreadAffinityMask(void* pThread, unsigned int nMask)
    {
        ::SetThreadAffinityMask(pThread, nMask);
    }

    void EnableLFHHeap()
    {
        ULONG HeapInformation = 2; // LFH heap mode
        HeapSetInformation(GetProcessHeap(), HeapEnableTerminationOnCorruption, &HeapInformation,
                           sizeof(HeapInformation));
    }

    void* CreateThread(Runtime::Parallel::ThreadOptionsHelper& refThreadOptions)
    {
        return ::CreateThread(nullptr, static_cast<SIZE_T>(refThreadOptions.stackSize), refThreadOptions.startAddress,
                              refThreadOptions.paramsAddress,
                              refThreadOptions.isSuspendedOnStart ? CREATE_SUSPENDED : 0, &refThreadOptions.threadId);
    }

    void JoinThread(void* thread)
    {
        ::WaitForSingleObject(thread, INFINITE);
        ::CloseHandle(thread);
    }

    void DetachThread(void* thread)
    {
        ::CloseHandle(thread);
    }

    void SuspendThread(void* thread)
    {
        if (::SuspendThread(thread) == -1)
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToSuspendThreadMessage, thread);
            RUNTIME_CONSOLE_LOG_CRITICAL(StaticString::FailedToSuspendThreadMessage, thread);
        }
    }

    void ResumeThread(void* thread)
    {
        if (::ResumeThread(thread) == -1)
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToResumeThreadMessage, thread);
            RUNTIME_CONSOLE_LOG_CRITICAL(StaticString::FailedToResumeThreadMessage, thread);
        }
    }

    void YieldThread()
    {
        if (::SwitchToThread() == 0)
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToSwitchThreadMessage);
            RUNTIME_CONSOLE_LOG_CRITICAL(StaticString::FailedToSwitchThreadMessage);
        }
    }

    unsigned long GetCurrentThreadId()
    {
        return ::GetCurrentThreadId();
    }

    void YieldCurrentProcessor()
    {
        YieldProcessor();
    }

    void InitProcessInfoHelper(ProcessInfoHelper* processInfoHelper)
    {
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuffer = nullptr;
        DWORD nLength = 0;
        if (!GetLogicalProcessorInformation(pBuffer, &nLength))
        {
            pBuffer = static_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>(malloc(nLength));
            if (!GetLogicalProcessorInformation(pBuffer, &nLength))
            {
                return;
            }
        }
        if (!pBuffer)
        {
            return;
        }
        DWORD byteOffset = 0;
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = pBuffer;
        while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= nLength)
        {
            switch (pBuffer->Relationship)
            {
            case RelationProcessorCore:
                {
                    processInfoHelper->physicalCoresProcessCount++;
                    processInfoHelper->logicalCoresProcessCount +=
                        static_cast<int>(_mm_popcnt_u64(static_cast<unsigned long long>(pBuffer->ProcessorMask)));
                    break;
                }
            case RelationNumaNode:
                {
                    processInfoHelper->numaNodesCount++;
                    break;
                }
            case RelationCache:
                {
                    processInfoHelper->caches = reinterpret_cast<System::ProcessInfoHelper::Cache*>(
                        realloc(processInfoHelper->caches,
                                sizeof(System::ProcessInfoHelper::Cache) * (++processInfoHelper->cachesCount)));
                    if (!processInfoHelper->caches)
                        return;
                    PCACHE_DESCRIPTOR Cache = &pBuffer->Cache;
                    processInfoHelper->caches[processInfoHelper->cachesCount - 1].level =
                        static_cast<int>(Cache->Level);
                    processInfoHelper->caches[processInfoHelper->cachesCount - 1].associativity = Cache->Associativity;
                    processInfoHelper->caches[processInfoHelper->cachesCount - 1].lineSize = Cache->LineSize;
                    processInfoHelper->caches[processInfoHelper->cachesCount - 1].size = Cache->Size;
                    processInfoHelper->caches[processInfoHelper->cachesCount - 1].type = Cache->Type;
                    processInfoHelper->l1CachesCount += Cache->Level == 1;
                    processInfoHelper->l2CachesCount += Cache->Level == 2;
                    processInfoHelper->l3CachesCount += Cache->Level == 3;
                    break;
                }
            case RelationProcessorPackage:
                {
                    processInfoHelper->processorCorePackageCount++;
                    break;
                }
            case RelationGroup:
                {
                    break;
                }
            case RelationProcessorDie:
                {
                    break;
                }
            case RelationNumaNodeEx:
                {
                    break;
                }
            case RelationProcessorModule:
                {
                    break;
                }
            case RelationAll:
                {
                    break;
                }
            default:
                {
                    break;
                }
            }
            byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
            pBuffer++;
        }
        free(ptr);
    }


    RUNTIME_API void SetThreadPriority(void* handle, ThreadPriority priority)
    {
        int nPriority = 0;
        switch (priority)
        {
        case ThreadPriority::ModeBackgroundBegin:
            nPriority = THREAD_MODE_BACKGROUND_BEGIN;
            break;
        case ThreadPriority::ModeBackgroundEnd:
            nPriority = THREAD_MODE_BACKGROUND_END;
            break;
        case ThreadPriority::AboveNormal:
            nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case ThreadPriority::BelowNormal:
            nPriority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case ThreadPriority::High:
            nPriority = THREAD_PRIORITY_HIGHEST;
            break;
        case ThreadPriority::Idle:
            nPriority = THREAD_PRIORITY_IDLE;
            break;
        case ThreadPriority::Lowest:
            nPriority = THREAD_PRIORITY_LOWEST;
            break;
        case ThreadPriority::Normal:
            nPriority = THREAD_PRIORITY_NORMAL;
            break;
        case ThreadPriority::RealTime:
            nPriority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
        }
        if (!::SetThreadPriority(handle, nPriority))
        {
        }
    }
} // namespace Runtime::System::Windows
