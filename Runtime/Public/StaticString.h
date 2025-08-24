#pragma once

namespace Runtime::StaticString
{

    static inline constexpr char RuntimeSuccessfullInitializedMessage[] = "Runtime initialized successfully.";
    static inline constexpr char RuntimeSuccessfullShutdownMessage[] = "Runtime shutdown successfully.";
    static inline constexpr char FailedToInitializeRuntimeMessage[] = "Failed to initialize Runtime.";
    static inline constexpr char FailedToShutdownRuntimeMessage[] = "Failed to shutdown Runtime.";
    static inline constexpr char RuntimeQuillMainThreadName[] = "RuntimeQuillMain";
    static inline constexpr char SystemHandleExceptionMessage[] =
        "Unhandled exception in Runtime. Please check the logs for more details. stack trace:\n{}";
    static inline constexpr char FailedToCreateDumpFileMessage[] =
        "Failed to create dump file. Please check the logs for more details.";
    static inline constexpr char DumpFileFormatName[] = "runtime_crash_%04d%02d%02d_%02d%02d%02d_%d.dmp";
    static inline constexpr char DbgHelpLibName[] = "DbgHelp.dll";
    static inline constexpr char MiniDumpWriteDumpFunctionName[] = "MiniDumpWriteDump";
    static inline constexpr char CannotLoadDbgHelpLibMessage[] = "Cannot load dbghelp.dll";
    static inline constexpr char CannotGetMiniDumpWriteDumpFunctionMessage[] = "Cannot get MiniDumpWriteDump function";
    static inline constexpr char PureVirtualCallMessage[] = "Pure virtual function called.";
    static inline constexpr char FailedToSwitchThreadMessage[] =
        "Failed to switch thread. Please check the logs for more details.";
    static inline constexpr char AssertionFailedMessage[] = "Assertion failed:{}";
    static inline constexpr char FailedToJoinThreadMessage[] =
        "Destroying thread object while thread is still joinable. Thread id: {}";
    static inline constexpr char ThreadIsNotJoinableMessage[] = "Thread is not joinable";
    static inline constexpr char ThreadCreateSuccessMessage[] = "Create thread with id {}";
    static inline constexpr char CannotCreateThreadMessage[] = "Cannot create thread";
    static inline constexpr char ExceptionInThreadFunctionMessage[] = "Exception in thread function";

} // namespace Runtime::StaticString
