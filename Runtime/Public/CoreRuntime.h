#pragma once

#include "Config.h"

/**
 * @file CoreRuntime.h
 * @brief Defines the CoreRuntime class for the Runtime module.
 */

namespace Runtime
{

    /**
     * @class CoreRuntime
     * @brief Main entry point for the core runtime system.
     *
     * This class is non-copyable and non-movable.
     * It provides basic lifecycle management for the runtime.
     */
    class RUNTIME_API CoreRuntime final
    {
    public:
        /**
         * @brief Constructs a CoreRuntime instance.
         */
        CoreRuntime();

        /// Deleted copy constructor.
        CoreRuntime(const CoreRuntime&) = delete;
        /// Deleted copy assignment operator.
        CoreRuntime& operator=(const CoreRuntime&) = delete;
        /// Deleted move constructor.
        CoreRuntime(CoreRuntime&&) = delete;
        /// Deleted move assignment operator.
        CoreRuntime& operator=(CoreRuntime&&) = delete;

        /**
         * @brief Destroys the CoreRuntime instance.
         */
        ~CoreRuntime();
    };

} // namespace Runtime
