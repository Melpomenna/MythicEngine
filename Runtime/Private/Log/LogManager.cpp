#include "Log/LogManager.h"
#include "Log/QuillLoggerHelper.h"

namespace Runtime::LogHelper
{
    void initLog()
    {
        Quill::initLogger();
    }

    void stopLog()
    {
        Quill::stopLogger();
    }
} // namespace Runtime::LogHelper
