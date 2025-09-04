#include <signal.h>

#if defined(_WIN32) || defined(_WIN64)
    using UnhandledHandler = void(*)(void*);
#else
    using UnhandledHandler = void(*)(int, siginfo_t*, void*);
#endif