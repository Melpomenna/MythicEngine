/**
 * @file LogWrapper.h
 * @brief Logging macros and helpers for the Runtime module.
 *
 * This header provides a set of macros for logging at various levels (info, trace, debug, warning, error, critical)
 * using the Quill logging library. It also defines logger names and assertion/verification macros for debug builds.
 *
 * @copyright (C) Melpomenna - All rights reserved.
 */

#pragma once

#include <string_view>
#include "QuillLoggerHelper.h"
#include "StaticString.h"

/**
 * @def RUNTIME_LOG_INFO(logger, fmt, ...)
 * @brief Logs an info message to the specified logger.
 *
 * @def RUNTIME_LOG_TRACE_L3(logger, fmt, ...)
 * @brief Logs a level 3 trace message to the specified logger.
 *
 * @def RUNTIME_LOG_TRACE_L2(logger, fmt, ...)
 * @brief Logs a level 2 trace message to the specified logger.
 *
 * @def RUNTIME_LOG_TRACE_L1(logger, fmt, ...)
 * @brief Logs a level 1 trace message to the specified logger.
 *
 * @def RUNTIME_LOG_DEBUG(logger, fmt, ...)
 * @brief Logs a debug message to the specified logger.
 *
 * @def RUNTIME_LOG_WARNING(logger, fmt, ...)
 * @brief Logs a warning message to the specified logger.
 *
 * @def RUNTIME_LOG_ERROR(logger, fmt, ...)
 * @brief Logs an error message to the specified logger.
 *
 * @def RUNTIME_LOG_CRITICAL(logger, fmt, ...)
 * @brief Logs a critical message to the specified logger.
 */

#define RUNTIME_LOG_INFO(logger, fmt, ...) LOG_INFO(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_TRACE_L3(logger, fmt, ...) LOG_TRACE_L3(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_TRACE_L2(logger, fmt, ...) LOG_TRACE_L2(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_TRACE_L1(logger, fmt, ...) LOG_TRACE_L1(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_DEBUG(logger, fmt, ...) LOG_DEBUG(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_WARNING(logger, fmt, ...) LOG_WARNING(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_ERROR(logger, fmt, ...) LOG_ERROR(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_CRITICAL(logger, fmt, ...) LOG_CRITICAL(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)

/**
 * @namespace Runtime::LogHelper
 * @brief Contains logger name constants for the Runtime module.
 */
namespace Runtime::LogHelper
{
    /**
     * @brief Name of the console logger.
     */
    static inline constexpr char ConsoleLoggerName[] = "console";
    /**
     * @brief Name of the file logger.
     */
    static inline constexpr char FileLoggerName[] = "file";

} // namespace Runtime::LogHelper

#if defined(_DEBUG)
/**
 * @def RUNTIME_CONSOLE_LOG_INFO(fmt, ...)
 * @brief Logs an info message to the console logger in debug builds.
 *
 * @def RUNTIME_CONSOLE_LOG_TRACE_L3(fmt, ...)
 * @brief Logs a level 3 trace message to the console logger in debug builds.
 *
 * @def RUNTIME_CONSOLE_LOG_TRACE_L2(fmt, ...)
 * @brief Logs a level 2 trace message to the console logger in debug builds.
 *
 * @def RUNTIME_CONSOLE_LOG_TRACE_L1(fmt, ...)
 * @brief Logs a level 1 trace message to the console logger in debug builds.
 *
 * @def RUNTIME_CONSOLE_LOG_DEBUG(fmt, ...)
 * @brief Logs a debug message to the console logger in debug builds.
 *
 * @def RUNTIME_CONSOLE_LOG_WARNING(fmt, ...)
 * @brief Logs a warning message to the console logger in debug builds.
 *
 * @def RUNTIME_CONSOLE_LOG_ERROR(fmt, ...)
 * @brief Logs an error message to the console logger in debug builds.
 *
 * @def RUNTIME_CONSOLE_LOG_CRITICAL(fmt, ...)
 * @brief Logs a critical message to the console logger in debug builds.
 */

#define RUNTIME_CONSOLE_LOG_INFO(fmt, ...) RUNTIME_LOG_INFO(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)
#define RUNTIME_CONSOLE_LOG_TRACE_L3(fmt, ...)                                                                         \
    RUNTIME_LOG_TRACE_L3(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)
#define RUNTIME_CONSOLE_LOG_TRACE_L2(fmt, ...)                                                                         \
    RUNTIME_LOG_TRACE_L2(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)
#define RUNTIME_CONSOLE_LOG_TRACE_L1(fmt, ...)                                                                         \
    RUNTIME_LOG_TRACE_L1(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)
#define RUNTIME_CONSOLE_LOG_DEBUG(fmt, ...) RUNTIME_LOG_DEBUG(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)
#define RUNTIME_CONSOLE_LOG_WARNING(fmt, ...)                                                                          \
    RUNTIME_LOG_WARNING(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)
#define RUNTIME_CONSOLE_LOG_ERROR(fmt, ...) RUNTIME_LOG_ERROR(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)
#define RUNTIME_CONSOLE_LOG_CRITICAL(fmt, ...)                                                                         \
    RUNTIME_LOG_CRITICAL(Runtime::LogHelper::ConsoleLoggerName, fmt, __VA_ARGS__)

/**
 * @def RUNTIME_ASSERT_IMPL(condition, logger)
 * @brief Asserts a condition and logs a critical message if the assertion fails (debug only).
 *
 * @def RUNTIME_VERIFY_IMPL(condition)
 * @brief Verifies a condition and logs a critical message if the verification fails (debug only).
 *
 * @def RUNTIME_ASSERT(condition)
 * @brief Asserts a condition and logs to the file logger (debug only).
 *
 * @def RUNTIME_VERIFY(condition)
 * @brief Verifies a condition and logs to the file logger (debug only).
 */

#define RUNTIME_ASSERT_IMPL(condition, logger)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            assert(false);                                                                                             \
            RUNTIME_LOG_CRITICAL(logger, Runtime::StaticString::AssertionFailedMessage, std::string_view{#condition}); \
        }                                                                                                              \
    }                                                                                                                  \
    while (false)

#define RUNTIME_VERIFY_IMPL(condition)                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            assert(false);                                                                                             \
            RUNTIME_LOG_CRITICAL(logger, Runtime::StaticString::AssertionFailedMessage, std::string_view{#condition}); \
        }                                                                                                              \
    }                                                                                                                  \
    while (true)

#define RUNTIME_ASSERT(condition) RUNTIME_ASSERT_IMPL(condition, Runtime::LogHelper::FileLoggerName)
#define RUNTIME_VERIFY(condition) RUNTIME_VERIFY_IMPL(condition, Runtime::LogHelper::FileLoggerName)
#else
/**
 * @def RUNTIME_CONSOLE_LOG_INFO(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_CONSOLE_LOG_TRACE_L3(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_CONSOLE_LOG_TRACE_L2(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_CONSOLE_LOG_TRACE_L1(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_CONSOLE_LOG_DEBUG(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_CONSOLE_LOG_WARNING(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_CONSOLE_LOG_ERROR(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_CONSOLE_LOG_CRITICAL(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_ASSERT(...)
 * @brief No-op in release builds.
 *
 * @def RUNTIME_VERIFY(...)
 * @brief No-op in release builds.
 */
#define RUNTIME_CONSOLE_LOG_INFO(...)
#define RUNTIME_CONSOLE_LOG_TRACE_L3(...)
#define RUNTIME_CONSOLE_LOG_TRACE_L2(...)
#define RUNTIME_CONSOLE_LOG_TRACE_L1(...)
#define RUNTIME_CONSOLE_LOG_DEBUG(...)
#define RUNTIME_CONSOLE_LOG_WARNING(...)
#define RUNTIME_CONSOLE_LOG_ERROR(...)
#define RUNTIME_CONSOLE_LOG_CRITICAL(...)
#define RUNTIME_ASSERT(...)
#define RUNTIME_VERIFY(...)
#endif
