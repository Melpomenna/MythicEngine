#include "Parallel/CpuMaskHelper.h"

#include "Log/LogWrapper.h"
#include "System/SystemHelper.h"

namespace Runtime::Parallel
{

    CpuMaskHelper& CpuMaskHelper::SetCore(UI8 core) & noexcept
    {
        RUNTIME_ASSERT((core < static_cast<int>(sizeof(mask_) * 8) &&
                        core <= static_cast<int>(Runtime::System::HardwareConcurent())));
        mask_ |= (1 << core);
        return *this;
    }

    CpuMaskHelper& CpuMaskHelper::UseCore(UI8 core, UI8& res) & noexcept
    {
        if (HasCore(core))
            RUNTIME_UNLIKELY
            {
                RUNTIME_ASSERT(((mask_ & (1 << core)) != 0));
                res = 0;
                return *this;
            }
        else
        {
            res |= 1 << core;
            SetCore(core);
            return *this;
        }
    }

    bool CpuMaskHelper::HasCore(UI8 core) const& noexcept
    {
        RUNTIME_ASSERT((core < static_cast<UI8>(sizeof(mask_) * 8) &&
                        core <= static_cast<UI8>(Runtime::System::HardwareConcurent())));
        return (mask_ & (1 << core)) != 0;
    }
} // namespace Runtime::Parallel
