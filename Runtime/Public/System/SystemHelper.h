/**
 * @file SystemHelper.h
 * @brief Provides system-level helper functions for thread management, exception handling, and heap configuration.
 *
 * @author Melpomenna
 * @date 25.08.2025
 */

#pragma once

#include "Config.h"
#include "System/SystemEnumerations.h"

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
     * @struct ProcessInfoHelper
     * @brief Holds information about the current process's hardware and cache configuration.
     *
     * This structure is used to store details about processor caches, NUMA nodes, core counts,
     * and other relevant system information for the current process. It is typically populated
     * by calling InitProcessInfoHelper().
     */
    struct ProcessInfoHelper final
    {
        ProcessInfoHelper() = default;
        ~ProcessInfoHelper()
        {
            delete caches;
        }

        /**
         * @struct Cache
         * @brief Represents a processor cache.
         *
         * Contains information about a specific cache level, including its type, size, associativity, and line size.
         */
        struct Cache final
        {
            /**
             * @brief Cache level (e.g., 1 for L1, 2 for L2, 3 for L3).
             */
            int level = 0;

            /**
             * @brief Cache associativity (number of ways).
             */
            int associativity = 0;

            /**
             * @brief Cache line size in bytes.
             */
            int lineSize = 0;

            /**
             * @brief Cache size in bytes.
             */
            int size = 0;

            /**
             * @brief Cache type.
             *
             * Possible values:
             *   - 1: CacheUnified
             *   - 2: CacheInstruction
             *   - 3: CacheData
             *   - 4: CacheTrace
             *   - 0: CacheUnknown
             * These correspond to the WinAPI cache type constants.
             */
            int type = 0;
        };

        /**
         * @brief Pointer to an array of Cache structures describing processor caches.
         */
        Cache* caches = nullptr;

        /**
         * @brief Number of L3 caches detected.
         */
        int l3CachesCount = 0;

        /**
         * @brief Number of L2 caches detected.
         */
        int l2CachesCount = 0;

        /**
         * @brief Number of L1 caches detected.
         */
        int l1CachesCount = 0;

        /**
         * @brief Total number of caches detected.
         */
        int cachesCount = 0;

        /**
         * @brief Number of NUMA nodes available to the process.
         */
        int numaNodesCount = 0;

        /**
         * @brief Number of logical processor cores available to the process.
         */
        int logicalCoresProcessCount = 0;

        /**
         * @brief Number of physical processor cores available to the process.
         */
        int physicalCoresProcessCount = 0;

        /**
         * @brief Number of processor core packages available to the process.
         */
        int processorCorePackageCount = 0;
    };

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

    /**
     * @brief Returns the number of concurrent hardware threads supported by the system.
     *
     * This function queries the system for the number of hardware threads (logical processors)
     * that are available for parallel execution. It is typically used to determine the optimal
     * number of threads to use for parallel algorithms or thread pools.
     *
     * @return The number of concurrent hardware threads supported by the system.
     */
    RUNTIME_NODISCARD RUNTIME_API unsigned int HardwareConcurent();

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

    RUNTIME_API void SetThreadPriority(void* handle, ThreadPriority priority);

} // namespace Runtime::System
