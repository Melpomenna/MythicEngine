#include "Memory/MallocHelper.h"
#include "Memory/MimallocHelper.h"


namespace Runtime::Memory
{
    void initOptions()
    {
        Runtime::Memory::Mimalloc::initOptions();
    }
} // namespace Runtime::Memory
