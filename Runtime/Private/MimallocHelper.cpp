#include "MimallocHelper.h"

#pragma push_macro("_expand")
#undef _expand

#include <mimalloc-new-delete.h>
#include <mimalloc-override.h>
#include <mimalloc.h>

#pragma pop_macro("_expand")

namespace Runtime::Memory::Mimalloc
{
    void initOptions()
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
    }
} // namespace Runtime::Memory::Mimalloc
