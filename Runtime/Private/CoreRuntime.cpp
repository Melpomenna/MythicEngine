#include "CoreRuntime.h"
#include "Log/LogWrapper.h"
#include "MythicRuntime.h"
#include "StaticString.h"

namespace Runtime
{
    CoreRuntime::CoreRuntime()
    {
        if (!InitRuntime())
            RUNTIME_UNLIKELY
            {
                RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::FailedToInitializeRuntimeMessage);
            }
    }

    CoreRuntime ::~CoreRuntime()
    {
        (void)ShutdownRuntime();
    }
} // namespace Runtime
