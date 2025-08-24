#include "Windows/System.h"
#include "LogWrapper.h"
#include "StaticString.h"
#include "ThreadHelper.h"
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
                              refThreadOptions.paramsAddress, refThreadOptions.isSuspendedOnStart ? CREATE_SUSPENDED : 0,
                              &refThreadOptions.threadId);
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
        ::SuspendThread(thread);
    }

    void ResumeThread(void* thread)
    {
        ::ResumeThread(thread);
    }

    void YieldThread()
    {
        if (!::SwitchToThread())
        {
            RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToSwitchThreadMessage);
            RUNTIME_CONSOLE_LOG_CRITICAL(StaticString::FailedToSwitchThreadMessage);
        }
    }

    unsigned long GetCurrentThreadId()
    {
        return ::GetCurrentThreadId();
    }
} // namespace Runtime::System::Windows
