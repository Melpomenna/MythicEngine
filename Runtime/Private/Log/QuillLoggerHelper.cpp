#include "Log/QuillLoggerHelper.h"
#include "Log/Loggers.h"
#include "Parallel/CpuMaskHelper.h"
#include "Singletone.h"
#include "StaticString.h"

namespace Runtime::LogHelper::Quill
{
    void initLogger()
    {
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
    }

    void stopLogger()
    {
        quill::Backend::stop();
    }
} // namespace Runtime::LogHelper::Quill
