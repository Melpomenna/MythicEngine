#include "CpuMaskHelper.h"

#include <thread>
#include "LogWrapper.h"

namespace Runtime::Parallel
{

    CpuMaskHelper& CpuMaskHelper::SetCore(int core) & noexcept
    {
        RUNTIME_ASSERT((core < static_cast<int>(sizeof(mask_) * 8) &&
                        core <= static_cast<int>(std::thread::hardware_concurrency())));
        mask_ |= (1 << core);
        return *this;
    }

    CpuMaskHelper& CpuMaskHelper::UseCore(int core, int& res) & noexcept
    {
        if (HasCore(core))
            RUNTIME_UNLIKELY
            {
                RUNTIME_ASSERT(((mask_ & (1 << core)) != 0));
                res = -1;
                return *this;
            }
        else
        {
            res |= 1 << core;
            SetCore(core);
            return *this;
        }
    }

    bool CpuMaskHelper::HasCore(int core) const& noexcept
    {
        RUNTIME_ASSERT((core < static_cast<int>(sizeof(mask_) * 8) &&
                        core <= static_cast<int>(std::thread::hardware_concurrency())));
        return (mask_ & (1 << core)) != 0;
    }
} // namespace Runtime::Parallel
