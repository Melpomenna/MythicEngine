#pragma once

namespace Runtime::System
{
    enum class ThreadPriority
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
