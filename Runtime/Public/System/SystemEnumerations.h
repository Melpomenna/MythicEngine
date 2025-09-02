#pragma once

#include "Types.h"

namespace Runtime::System
{
    enum class ThreadPriority : UI8
    {
        ModeBackgroundBegin,
        ModeBackgroundEnd,
        AboveNormal,
        BelowNormal,
        High,
        Idle,
        Lowest,
        Normal,
        RealTime
    };
} // namespace Runtime::System
