/**
 * @file MimallocHelper.h
 * @brief Helper functions for configuring mimalloc memory allocator options.
 *
 * This header provides an interface for initializing mimalloc-specific options
 * for memory management within the Runtime::Memory::Mimalloc namespace.
 *
 * @author Melpomenna
 * @date 27.08.2025
 */

namespace Runtime::Memory::Mimalloc
{
    /**
     * @brief Initializes mimalloc allocator options.
     *
     * Call this function at program startup to configure mimalloc-specific
     * options before any allocations are performed.
     */
    void initOptions();
} // namespace Runtime::Memory::Mimalloc
