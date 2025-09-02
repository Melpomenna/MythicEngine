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

#include "Types.h"

namespace Runtime::StaticString
{

    /**
     * @brief Message indicating successful initialization of the Runtime.
     */
    static inline constexpr Symbol RuntimeSuccessfullInitializedMessage[] = "Runtime initialized successfully.";

    /**
     * @brief Message indicating successful shutdown of the Runtime.
     */
    static inline constexpr Symbol RuntimeSuccessfullShutdownMessage[] = "Runtime shutdown successfully.";

    /**
     * @brief Message indicating failure to initialize the Runtime.
     */
    static inline constexpr Symbol FailedToInitializeRuntimeMessage[] = "Failed to initialize Runtime.";

    /**
     * @brief Message indicating failure to shutdown the Runtime.
     */
    static inline constexpr Symbol FailedToShutdownRuntimeMessage[] = "Failed to shutdown Runtime.";

    /**
     * @brief Name of the main thread for the RuntimeQuill system.
     */
    static inline constexpr Symbol RuntimeQuillMainThreadName[] = "RuntimeQuillMain";

    /**
     * @brief Message for unhandled exceptions, including a placeholder for stack trace.
     */
    static inline constexpr Symbol SystemHandleExceptionMessage[] =
        "Unhandled exception in Runtime. Please check the logs for more details. stack trace:\n{}";

    /**
     * @brief Message indicating failure to create a dump file.
     */
    static inline constexpr Symbol FailedToCreateDumpFileMessage[] =
        "Failed to create dump file. Please check the logs for more details.";

    /**
     * @brief Format string for naming dump files.
     */
    static inline constexpr Symbol DumpFileFormatName[] = "runtime_crash_%04d%02d%02d_%02d%02d%02d_%d.dmp";

    /**
     * @brief Name of the DbgHelp library.
     */
    static inline constexpr Symbol DbgHelpLibName[] = "DbgHelp.dll";

    /**
     * @brief Name of the MiniDumpWriteDump function.
     */
    static inline constexpr Symbol MiniDumpWriteDumpFunctionName[] = "MiniDumpWriteDump";

    /**
     * @brief Message indicating failure to load the DbgHelp library.
     */
    static inline constexpr Symbol CannotLoadDbgHelpLibMessage[] = "Cannot load dbghelp.dll";

    /**
     * @brief Message indicating failure to get the MiniDumpWriteDump function.
     */
    static inline constexpr Symbol CannotGetMiniDumpWriteDumpFunctionMessage[] =
        "Cannot get MiniDumpWriteDump function";

    /**
     * @brief Message for pure virtual function call errors.
     */
    static inline constexpr Symbol PureVirtualCallMessage[] = "Pure virtual function called.";

    /**
     * @brief Message indicating failure to switch threads.
     */
    static inline constexpr Symbol FailedToSwitchThreadMessage[] =
        "Failed to switch thread. Please check the logs for more details.";

    /**
     * @brief Message for assertion failures, with a placeholder for details.
     */
    static inline constexpr Symbol AssertionFailedMessage[] = "Assertion failed:{}";

    /**
     * @brief Message indicating failure to join a thread, with a placeholder for thread id.
     */
    static inline constexpr Symbol FailedToJoinThreadMessage[] =
        "Destroying thread object while thread is still joinable. Thread id: {}";

    /**
     * @brief Message indicating that a thread is not joinable.
     */
    static inline constexpr Symbol ThreadIsNotJoinableMessage[] = "Thread is not joinable";

    /**
     * @brief Message indicating successful thread creation, with a placeholder for thread id.
     */
    static inline constexpr Symbol ThreadCreateSuccessMessage[] = "Create thread with id {}";

    /**
     * @brief Message indicating failure to create a thread.
     */
    static inline constexpr Symbol CannotCreateThreadMessage[] = "Cannot create thread";

    /**
     * @brief Message for exceptions thrown in thread functions.
     */
    static inline constexpr Symbol ExceptionInThreadFunctionMessage[] = "Exception in thread function";

    /**
     * @brief Message indicating failure to suspend a thread, with a placeholder for thread id.
     *
     * This message is used when the system fails to suspend a thread.
     * The placeholder '{}' should be replaced with the thread pointer.
     */
    static inline constexpr Symbol FailedToSuspendThreadMessage[] = "Cannot suspend thread {}";

    /**
     * @brief Message indicating failure to resume a thread, with a placeholder for thread id.
     *
     * This message is used when the system fails to resume a thread.
     * The placeholder '{}' should be replaced with the thread pointer.
     */
    static inline constexpr Symbol FailedToResumeThreadMessage[] = "Cannot resume thread {}";
} // namespace Runtime::StaticString
