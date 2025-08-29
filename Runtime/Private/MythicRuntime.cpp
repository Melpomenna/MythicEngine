#include "Memory/MallocHelper.h"
#include <stacktrace> // TODO: Create own stacktrace helper
#include <string_view>
#include "Log/LogManager.h"
#include "Log/LogWrapper.h"
#include "MythicRuntime.h"
#include "Parallel/CpuMaskHelper.h"
#include "Singletone.h"
#include "StaticString.h"
#include "System/SystemHelper.h"


namespace Runtime
{
    bool InitRuntime()
    {
        System::InitProcessInfoHelper(SingletoneHelper<System::ProcessInfoHelper>::Instance());
        Memory::initOptions();
        System::EnableLFHHeap();
        LogHelper::initLog();

        System::SetUnhandledExceptionHandler(
            [](void* handlerStruct)
            {
                auto stackTrace = std::to_string(std::stacktrace::current());
                RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::SystemHandleExceptionMessage,
                                     std::string_view{stackTrace});
                RUNTIME_CONSOLE_LOG_CRITICAL(StaticString::SystemHandleExceptionMessage, std::string_view{stackTrace});
                System::GenerateDump(handlerStruct);
            });

        System::SetPureVirtualCallHandler(
            []()
            {
                RUNTIME_LOG_CRITICAL(LogHelper::FileLoggerName, StaticString::PureVirtualCallMessage);
                RUNTIME_CONSOLE_LOG_CRITICAL(StaticString::PureVirtualCallMessage);
                std::exit(1);
            });

        RUNTIME_LOG_TRACE_L3(LogHelper::FileLoggerName, StaticString::RuntimeSuccessfullInitializedMessage);
        RUNTIME_CONSOLE_LOG_DEBUG(StaticString::RuntimeSuccessfullInitializedMessage);
        return true;
    }

    bool ShutdownRuntime()
    {
        SingletoneHelper<Runtime::Parallel::CpuMaskHelper>::Destroy();
        SingletoneHelper<System::ProcessInfoHelper>::Destroy();
        RUNTIME_LOG_TRACE_L3(LogHelper::FileLoggerName, StaticString::RuntimeSuccessfullShutdownMessage);
        RUNTIME_CONSOLE_LOG_DEBUG(StaticString::RuntimeSuccessfullShutdownMessage);
        LogHelper::stopLog();
        return true;
    }
} // namespace Runtime
