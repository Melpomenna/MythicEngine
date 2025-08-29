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

    using Handle = void*;
} // namespace Runtime::System
