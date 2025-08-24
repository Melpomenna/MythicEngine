#pragma once

#if !defined(RUNTIME_EXPORT)
#define RUNTIME_API __declspec(dllimport)
#else
#define RUNTIME_API __declspec(dllexport)
#endif

#define RUNTIME_ETW_ALLOCATOR __declspec(allocator)
#define RUNTIME_NODISCARD [[nodiscard]]
#define RUNTIME_LIKELY [[likely]]
#define RUNTIME_UNLIKELY [[unlikely]]
