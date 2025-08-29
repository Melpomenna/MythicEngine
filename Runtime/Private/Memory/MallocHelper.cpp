#include "Memory/MallocHelper.h"

namespace Runtime::Memory
{
    void initOptions()
    {
        Runtime::Memory::Mimalloc::initOptions();
    }
} // namespace Runtime::Memory
