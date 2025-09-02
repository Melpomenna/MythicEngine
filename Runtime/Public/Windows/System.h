#pragma once

/**
 * @file System.h
 * @brief Windows-specific system utilities and threading API for the Runtime library.
 */

#include "Config.h"
#include "System/SystemEnumerations.h"
#include "Types.h"

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
} // namespace Runtime::Parallel

namespace Runtime::System
{
    struct ProcessInfoHelper;
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
    RUNTIME_API void SetThreadAffinityMask(void* pThread, UI32 mask);

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
    RUNTIME_API void JoinThread(Handle thread);

    /**
     * @brief Detaches the specified thread, allowing it to run independently.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void DetachThread(Handle thread);

    /**
     * @brief Suspends the specified thread.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void SuspendThread(Handle thread);

    /**
     * @brief Resumes the specified suspended thread.
     * @param thread Pointer to the thread object.
     */
    RUNTIME_API void ResumeThread(Handle thread);

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
    RUNTIME_NODISCARD RUNTIME_API UI64 GetCurrentThreadId();

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

    /**
     * @brief Returns the number of concurrent hardware threads supported by the system.
     *
     * This function queries the system for the number of hardware threads (logical processors)
     * that are available for parallel execution. It is typically used to determine the optimal
     * number of threads to use for parallel algorithms or thread pools.
     *
     * @return The number of concurrent hardware threads supported by the system.
     */
    RUNTIME_NODISCARD RUNTIME_API UI16 HardwareConcurent();

    /**
     * @brief Initializes the specified ProcessInfoHelper structure with system information.
     *
     * This function populates the given ProcessInfoHelper instance with details about the
     * system's processor caches, NUMA nodes, logical and physical core counts, and other
     * relevant hardware information. It should be called before accessing any fields of
     * ProcessInfoHelper that depend on system-specific data.
     *
     * @param processInfoHelper Pointer to the ProcessInfoHelper structure to initialize.
     */
    RUNTIME_API void InitProcessInfoHelper(ProcessInfoHelper* processInfoHelper);

    RUNTIME_API void SetThreadPriority(Handle handle, ThreadPriority priority);
} // namespace Runtime::System::Windows
