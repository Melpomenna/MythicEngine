/**
 * @file Config.h
 * @brief Configuration macros for runtime DLL export/import and attributes.
 *
 * This header defines macros for controlling symbol visibility and attributes
 * used throughout the runtime library. It is intended for use in both
 * dynamic and static linking scenarios, and provides annotations for
 * compiler optimizations and diagnostics.
 *
 * @note
 * - RUNTIME_API: Controls DLL import/export based on RUNTIME_EXPORT definition.
 * - RUNTIME_ETW_ALLOCATOR: Marks functions as allocator for ETW (Event Tracing for Windows).
 * - RUNTIME_NODISCARD: Marks functions whose return value should not be discarded.
 * - RUNTIME_LIKELY / RUNTIME_UNLIKELY: Provides branch prediction hints.
 */

#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define RUNTIME_PLATFORM_WINDOWS 1
#else
    #define RUNTIME_PLATFORM_WINDOWS 0
#endif

#if RUNTIME_PLATFORM_WINDOWS
    #if !defined(RUNTIME_EXPORT)
    /**

    * @brief Marks symbols for import * @def RUNTIME_API from a DLL.
    *
    * If RUNTIME_EXPORT is not defined, symbols are imported from a DLL.
    */
    #define RUNTIME_API __declspec(dllimport)
    #else
    /**
     * @def RUNTIME_API
     * @brief Marks symbols for export to a DLL.
     *
     * If RUNTIME_EXPORT is defined, symbols are exported to a DLL.
     */
    #define RUNTIME_API __declspec(dllexport)
    #endif
#else
    #define RUNTIME_API __attribute__((visibility("default")))
#endif

/**
 * @def RUNTIME_ETW_ALLOCATOR
 * @brief Marks functions as allocators for ETW (Event Tracing for Windows).
 */
#if RUNTIME_PLATFORM_WINDOWS
    #define RUNTIME_ETW_ALLOCATOR __declspec(allocator)
#else
    #define RUNTIME_ETW_ALLOCATOR
#endif

/**
 * @def RUNTIME_NODISCARD
 * @brief Indicates that the return value of a function should not be discarded.
 */

#define RUNTIME_NODISCARD [[nodiscard]]

/**
 * @def RUNTIME_LIKELY
 * @brief Provides a branch prediction hint that a condition is likely to be true.
 */
#define RUNTIME_LIKELY [[likely]]

/**
 * @def RUNTIME_UNLIKELY
 * @brief Provides a branch prediction hint that a condition is unlikely to be true.
 */
#define RUNTIME_UNLIKELY [[unlikely]]
