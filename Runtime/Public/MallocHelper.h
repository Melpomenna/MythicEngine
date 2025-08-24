/**
 * @file MallocHelper.h
 * @brief Provides integration of mimalloc memory allocator with custom new/delete operators.
 *
 * This header sets up the mimalloc allocator for use in the project, overriding
 * the default memory allocation routines. It includes the necessary mimalloc headers
 * and ensures that macro expansion for `_expand` does not interfere with mimalloc usage.
 *
 * @note
 * - Uses mimalloc for efficient memory management.
 * - Pushes and pops the `_expand` macro to avoid conflicts.
 * - Intended for use in projects requiring custom memory allocation strategies.
 */

#pragma once

#pragma push_macro("_expand")
#undef _expand

#include <mimalloc-new-delete.h>
#include <mimalloc-override.h>
#include <mimalloc.h>

#pragma pop_macro("_expand")
