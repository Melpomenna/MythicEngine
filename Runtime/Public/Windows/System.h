#pragma once

/**
 * @file System.h
 * @brief Windows-specific system utilities and threading API for the Runtime library.
 */

#include "Config.h"

/**
 * @namespace Runtime::Parallel
 * @brief Parallelism utilities and helpers.
 */
namespace Runtime::Parallel
{
    /**
     * @struct ThreadOptionsHelper
     * @brief Helper structure for configuring thread options.
     */
    struct ThreadOptionsHelper;
}

/**
 * @namespace Runtime::System::Windows
 * @brief Windows-specific system and threading functions.
 */
namespace Runtime::System::Windows
{
    /**
     * @brief Sets a handler for unhandled exceptions.
     * @param pUnhandledExceptionHandler Pointer to the exception handler function.
     */
    RUNTIME_API void SetUnhandledExceptionHandler(void (*pUnhandledExceptionHandler)(void*));

    /**
     * @brief Generates a dump file for the given exception information.
     * @param pExceptionInfo Pointer to exception information.
     */
    RUNTIME_API void GenerateDump(void* pExceptionInfo);

    /**
     * @brief Sets a handler for pure virtual function calls.
     * @param pPureCallHandler Pointer to the pure call handler function.
     */
    RUNTIME_API void SetPureVirtualCallHandler(void (*pPureCallHandler)());

    /**
     * @brief Sets the thread affinity mask for a given thread.
     * @param pThread Pointer to the thread object.
     * @param mask Affinity mask to set.
     */
    RUNTIME_API void SetThreadAffinityMask(void* pThread, unsigned int mask);

    /**
     * @brief Creates a new thread with the specified options.
     * @param refThreadOptions Reference to thread options helper.
     * @return Pointer to the created thread object.
     */
    RUNTIME_NODISCARD RUNTIME_API void* CreateThread(Runtime::Parallel::ThreadOptionsHelper& refThreadOptions);

    /**
     * @brief Joins the specified thread, blocking until it finishes execution.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void JoinThread(void* thread);

    /**
     * @brief Detaches the specified thread, allowing it to run independently.
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
     * @brief Enables the Low Fragmentation Heap (LFH) for improved memory allocation performance.
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
} // namespace Runtime::System::Windows
