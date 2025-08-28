/**
 * @file QuillLoggerHelper.h
 * @brief Helper header for including Quill logging components.
 *
 * This header provides convenient includes for Quill logging library components,
 * including backend, frontend, log macros, logger, and common sinks.
 *
 * @author Mellpomenna
 * @date 25.08.2025
 *
 * @copyright Melpomenna
 */

#pragma once

#include <quill/Backend.h> /**< Quill backend logging functionality */
#include <quill/Frontend.h> /**< Quill frontend logging functionality */
#include <quill/LogMacros.h> /**< Quill logging macros for easy logging */
#include <quill/Logger.h> /**< Quill logger class */
#include <quill/sinks/ConsoleSink.h> /**< Quill sink for console output */
#include <quill/sinks/FileSink.h> /**< Quill sink for file output */

namespace Runtime::LogHelper::Quill
{
    void initLogger();
    void stopLogger();

} // namespace Runtime::LogHelper::Quill
