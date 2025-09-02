#pragma once

#include "MimallocHelper.h"
#include "Types.h"

/**
 * @file MallocHelper.h
 * @brief Memory allocation helper functions for the Runtime module.
 */

namespace Runtime::Memory
{
    /**
     * @brief Initializes memory allocation options for the Runtime module.
     *
     * This function sets up any required options or configurations
     * for memory allocation routines used within the Runtime module.
     */
    void initOptions();

    template <class T>
    using DefaultAllocator = Mimalloc::MiAllocator<T>;

    static constexpr inline UI64 CacheLine = 64;
} // namespace Runtime::Memory
