#pragma once

#include <string_view>
#include "QuillLoggerHelper.h"
#include "StaticString.h"

#define RUNTIME_LOG_INFO(logger, fmt, ...) LOG_INFO(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_TRACE_L3(logger, fmt, ...) LOG_TRACE_L3(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_TRACE_L2(logger, fmt, ...) LOG_TRACE_L2(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_TRACE_L1(logger, fmt, ...) LOG_TRACE_L1(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_DEBUG(logger, fmt, ...) LOG_DEBUG(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_WARNING(logger, fmt, ...) LOG_WARNING(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_ERROR(logger, fmt, ...) LOG_ERROR(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)
#define RUNTIME_LOG_CRITICAL(logger, fmt, ...) LOG_CRITICAL(quill::Frontend::get_logger(logger), fmt, __VA_ARGS__)

namespace Runtime::LogHelper
{
    static inline constexpr char ConsoleLoggerName[] = "console";
    static inline constexpr char FileLoggerName[] = "file";

} // namespace Runtime::LogHelper

#if defined(_DEBUG)
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
