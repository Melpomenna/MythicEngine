#pragma once

#include "Config.h"

namespace Runtime
{

    class RUNTIME_API CoreRuntime final
    {
    public:
        CoreRuntime();

        CoreRuntime(const CoreRuntime&) = delete;
        CoreRuntime& operator=(const CoreRuntime&) = delete;
        CoreRuntime(CoreRuntime&&) = delete;
        CoreRuntime& operator=(CoreRuntime&&) = delete;

        ~CoreRuntime();
    };

} // namespace Runtime
