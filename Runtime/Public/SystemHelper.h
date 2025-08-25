/**
 * @file SystemHelper.h
 * @brief Provides system-level helper functions for thread management, exception handling, and heap configuration.
 *
 * @author Melpomenna
 * @date 25.08.2025
 */

#pragma once

#include "Config.h"

/**
 * @namespace Runtime::Parallel
 * @brief Contains parallelism-related helpers and types.
 */
namespace Runtime::Parallel
{
    /**
     * @struct ThreadOptionsHelper
     * @brief Helper structure for thread creation options.
     */
    struct ThreadOptionsHelper;
} // namespace Runtime::Parallel

/**
 * @namespace Runtime::System
 * @brief Provides system-level utilities for thread and exception management.
 */
namespace Runtime::System
{
    /**
     * @brief Sets the handler for unhandled exceptions.
     * @param unhandledExceptionHandler Pointer to the exception handler function.
     */
    RUNTIME_API void SetUnhandledExceptionHandler(void (*unhandledExceptionHandler)(void*));

    /**
     * @brief Generates a dump file for the given exception information.
     * @param exceptionInfo Pointer to exception information.
     */
    RUNTIME_API void GenerateDump(void* exceptionInfo);

    /**
     * @brief Sets the handler for pure virtual function calls.
     * @param pureVirtualCallHandler Pointer to the handler function.
     */
    RUNTIME_API void SetPureVirtualCallHandler(void (*pureVirtualCallHandler)());

    /**
     * @brief Sets the thread affinity mask for the specified thread.
     * @param thread Pointer to the thread object.
     * @param mask Affinity mask value.
     */
    RUNTIME_API void SetThreadAffinityMask(void* thread, unsigned int mask);

    /**
     * @brief Creates a new thread with the specified options.
     * @param threadOptions Reference to thread options helper.
     * @return Pointer to the created thread object.
     */
    RUNTIME_NODISCARD RUNTIME_API void* CreateThread(Runtime::Parallel::ThreadOptionsHelper& threadOptions);

    /**
     * @brief Waits for the specified thread to finish execution.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void JoinThread(void* thread);

    /**
     * @brief Detaches the specified thread.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void DetachThread(void* thread);

    /**
     * @brief Suspends the specified thread.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void SuspendThread(void* thread);

    /**
     * @brief Resumes the specified suspended thread.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void ResumeThread(void* thread);

    /**
     * @brief Enables the Low Fragmentation Heap (LFH) for the process.
     */
    RUNTIME_API void EnableLFHHeap();

    /**
     * @brief Yields execution of the current thread.
     */
    RUNTIME_API void YieldThread();

    /**
     * @brief Gets the identifier of the current thread.
     * @return The thread ID as an unsigned long.
     */
    RUNTIME_NODISCARD RUNTIME_API unsigned long GetCurrentThreadId();

    /**
     * @brief Yields execution of the current processor.
     *
     * Causes the calling thread to yield its execution on the current processor,
     * typically by issuing a processor-specific pause instruction. This is useful
     * in spin-wait loops to improve performance and reduce power consumption.
     *
     * On Windows, this uses the YieldProcessor intrinsic from the WinAPI.
     */
    RUNTIME_API void YieldCurrentProcessor();

} // namespace Runtime::System
