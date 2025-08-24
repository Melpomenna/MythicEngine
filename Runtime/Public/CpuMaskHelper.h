#pragma once

#include "Config.h"

namespace Runtime::Parallel
{
    class CpuMaskHelper final
    {
    public:
        CpuMaskHelper() noexcept = default;

        CpuMaskHelper(const CpuMaskHelper&) noexcept = default;

        CpuMaskHelper& operator=(const CpuMaskHelper&) noexcept = default;

        CpuMaskHelper(CpuMaskHelper&&) noexcept = default;

        CpuMaskHelper& operator=(CpuMaskHelper&& other) noexcept = default;

        ~CpuMaskHelper() noexcept = default;

        CpuMaskHelper& UseCore(int core, int& res) & noexcept;

        bool HasCore(int core) const& noexcept;

    private:
        CpuMaskHelper& SetCore(int core) & noexcept;

        int mask_{0};
    };
} // namespace Runtime::Parallel
