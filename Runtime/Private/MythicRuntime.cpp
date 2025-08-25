#include "MallocHelper.h"
#include <stacktrace> // TODO: Create own stacktrace helper
#include <string_view>
#include "CpuMaskHelper.h"
#include "LogWrapper.h"
#include "MythicRuntime.h"
#include "QuillLoggerHelper.h"
#include "Singletone.h"
#include "StaticString.h"
#include "SystemHelper.h"


namespace Runtime
{
    bool InitRuntime()
    {
        // clang-format off
        std::initializer_list<mi_option_t> options = {
#if defined (_DEBUG)
                                                      mi_option_show_stats,
                                                      mi_option_show_errors,
                                                      mi_option_verbose,
#endif
                                                      mi_option_reserve_huge_os_pages
        };
        // clang-format on
        for (const auto& option : options)
        {
            mi_option_set_enabled(option, true);
        }
        System::EnableLFHHeap();

        quill::BackendOptions backendOptions;
        // Awailable log cores: 0,1
        int logCpuAffinity = 0;
        SingletoneHelper<Runtime::Parallel::CpuMaskHelper>::Instance()
            ->UseCore(0, logCpuAffinity)
            .UseCore(1, logCpuAffinity);
        backendOptions.cpu_affinity = static_cast<decltype(backendOptions.cpu_affinity)>(logCpuAffinity);
        backendOptions.thread_name = StaticString::RuntimeQuillMainThreadName;
        quill::Backend::start(backendOptions);

        quill::Logger* consoleLogger = quill::Frontend::create_or_get_logger(
            LogHelper::ConsoleLoggerName,
            quill::Frontend::create_or_get_sink<quill::ConsoleSink>("runtime_console_sink_1"));
        quill::Logger* fileLogger = quill::Frontend::create_or_get_logger(
            LogHelper::FileLoggerName, quill::Frontend::create_or_get_sink<quill::FileSink>("runtime_file_sink_1"));

        consoleLogger->set_log_level(quill::v10::LogLevel::Debug);
        fileLogger->set_log_level(quill::v10::LogLevel::TraceL3);

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
        RUNTIME_LOG_TRACE_L3(LogHelper::FileLoggerName, StaticString::RuntimeSuccessfullShutdownMessage);
        RUNTIME_CONSOLE_LOG_DEBUG(StaticString::RuntimeSuccessfullShutdownMessage);
        quill::Backend::stop();
        return true;
    }
} // namespace Runtime
