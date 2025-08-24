#pragma once

#include "Config.h"

/**
 * @file MythicRuntime.h
 * @brief Provides initialization and shutdown functions for the Mythic Runtime.
 */

namespace Runtime
{

    /**
     * @brief Initializes the Mythic Runtime.
     *
     * This function sets up all necessary resources and state required for the runtime to operate.
     * It should be called before using any other runtime functionality.
     *
     * @return true if initialization was successful, false otherwise.
     */
    RUNTIME_NODISCARD RUNTIME_API bool InitRuntime();

    /**
     * @brief Shuts down the Mythic Runtime.
     *
     * This function releases all resources and performs cleanup for the runtime.
     * It should be called when runtime functionality is no longer needed.
     *
     * @return true if shutdown was successful, false otherwise.
     */
    RUNTIME_NODISCARD RUNTIME_API bool ShutdownRuntime();

} // namespace Runtime
