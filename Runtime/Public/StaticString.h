/**
 * @file StaticString.h
 * @brief Contains static constant string messages used throughout the Runtime library.
 *
 * This header defines a set of static, inline, constexpr string constants for common messages,
 * error descriptions, thread names, and other string literals used by the Runtime system.
 * These constants are intended to provide consistent messaging and reduce duplication.
 *
 * @copyright Copyright (c) 2025 Melpomenna
 */

#pragma once

namespace Runtime::StaticString
{

    /**
     * @brief Message indicating successful initialization of the Runtime.
     */
    static inline constexpr char RuntimeSuccessfullInitializedMessage[] = "Runtime initialized successfully.";

    /**
     * @brief Message indicating successful shutdown of the Runtime.
     */
    static inline constexpr char RuntimeSuccessfullShutdownMessage[] = "Runtime shutdown successfully.";

    /**
     * @brief Message indicating failure to initialize the Runtime.
     */
    static inline constexpr char FailedToInitializeRuntimeMessage[] = "Failed to initialize Runtime.";

    /**
     * @brief Message indicating failure to shutdown the Runtime.
     */
    static inline constexpr char FailedToShutdownRuntimeMessage[] = "Failed to shutdown Runtime.";

    /**
     * @brief Name of the main thread for the RuntimeQuill system.
     */
    static inline constexpr char RuntimeQuillMainThreadName[] = "RuntimeQuillMain";

    /**
     * @brief Message for unhandled exceptions, including a placeholder for stack trace.
     */
    static inline constexpr char SystemHandleExceptionMessage[] =
        "Unhandled exception in Runtime. Please check the logs for more details. stack trace:\n{}";

    /**
     * @brief Message indicating failure to create a dump file.
     */
    static inline constexpr char FailedToCreateDumpFileMessage[] =
        "Failed to create dump file. Please check the logs for more details.";

    /**
     * @brief Format string for naming dump files.
     */
    static inline constexpr char DumpFileFormatName[] = "runtime_crash_%04d%02d%02d_%02d%02d%02d_%d.dmp";

    /**
     * @brief Name of the DbgHelp library.
     */
    static inline constexpr char DbgHelpLibName[] = "DbgHelp.dll";

    /**
     * @brief Name of the MiniDumpWriteDump function.
     */
    static inline constexpr char MiniDumpWriteDumpFunctionName[] = "MiniDumpWriteDump";

    /**
     * @brief Message indicating failure to load the DbgHelp library.
     */
    static inline constexpr char CannotLoadDbgHelpLibMessage[] = "Cannot load dbghelp.dll";

    /**
     * @brief Message indicating failure to get the MiniDumpWriteDump function.
     */
    static inline constexpr char CannotGetMiniDumpWriteDumpFunctionMessage[] = "Cannot get MiniDumpWriteDump function";

    /**
     * @brief Message for pure virtual function call errors.
     */
    static inline constexpr char PureVirtualCallMessage[] = "Pure virtual function called.";

    /**
     * @brief Message indicating failure to switch threads.
     */
    static inline constexpr char FailedToSwitchThreadMessage[] =
        "Failed to switch thread. Please check the logs for more details.";

    /**
     * @brief Message for assertion failures, with a placeholder for details.
     */
    static inline constexpr char AssertionFailedMessage[] = "Assertion failed:{}";

    /**
     * @brief Message indicating failure to join a thread, with a placeholder for thread id.
     */
    static inline constexpr char FailedToJoinThreadMessage[] =
        "Destroying thread object while thread is still joinable. Thread id: {}";

    /**
     * @brief Message indicating that a thread is not joinable.
     */
    static inline constexpr char ThreadIsNotJoinableMessage[] = "Thread is not joinable";

    /**
     * @brief Message indicating successful thread creation, with a placeholder for thread id.
     */
    static inline constexpr char ThreadCreateSuccessMessage[] = "Create thread with id {}";

    /**
     * @brief Message indicating failure to create a thread.
     */
    static inline constexpr char CannotCreateThreadMessage[] = "Cannot create thread";

    /**
     * @brief Message for exceptions thrown in thread functions.
     */
    static inline constexpr char ExceptionInThreadFunctionMessage[] = "Exception in thread function";

} // namespace Runtime::StaticString
