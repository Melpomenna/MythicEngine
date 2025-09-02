#pragma once

#include "Config.h"

/**
 * @file CpuMaskHelper.h
 * @brief Defines the CpuMaskHelper class for managing CPU core masks.
 */

namespace Runtime::Parallel
{
    /**
     * @class CpuMaskHelper
     * @brief Helper class for managing a mask representing selected CPU cores.
     *
     * This class provides methods to set and query CPU core usage via a bitmask.
     */
    class CpuMaskHelper final
    {
    public:
        /**
         * @brief Default constructor.
         */
        CpuMaskHelper() noexcept = default;

        /**
         * @brief Copy constructor.
         * @param other The CpuMaskHelper to copy from.
         */
        CpuMaskHelper(const CpuMaskHelper&) noexcept = default;

        /**
         * @brief Copy assignment operator.
         * @param other The CpuMaskHelper to assign from.
         * @return Reference to this object.
         */
        CpuMaskHelper& operator=(const CpuMaskHelper&) noexcept = default;

        /**
         * @brief Move constructor.
         * @param other The CpuMaskHelper to move from.
         */
        CpuMaskHelper(CpuMaskHelper&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         * @param other The CpuMaskHelper to move from.
         * @return Reference to this object.
         */
        CpuMaskHelper& operator=(CpuMaskHelper&& other) noexcept = default;

        /**
         * @brief Destructor.
         */
        ~CpuMaskHelper() noexcept = default;

        /**
         * @brief Sets the usage of a specific CPU core.
         * @param core The core index to use.
         * @param res Reference to an int to store the result.
         * @return Reference to this object.
         */
        CpuMaskHelper& UseCore(UI8 core, UI8& res) & noexcept;

        /**
         * @brief Checks if a specific core is set in the mask.
         * @param core The core index to check.
         * @return True if the core is set, false otherwise.
         */
        bool HasCore(UI8 core) const& noexcept;

    private:
        /**
         * @brief Sets a specific core in the mask.
         * @param core The core index to set.
         * @return Reference to this object.
         */
        CpuMaskHelper& SetCore(UI8 core) & noexcept;

        /**
         * @brief Bitmask representing the selected CPU cores.
         */
        UI8 mask_{0};
    };
} // namespace Runtime::Parallel
