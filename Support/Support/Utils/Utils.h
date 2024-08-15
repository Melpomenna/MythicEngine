#pragma once

#define MYTHIC_ENGINE_VERSION 010

#define MYTHIC_ENGINE_MAJOR_VERSION MYTHIC_ENGINE_VERSION >> 3

#define MYTHIC_ENGINE_MINOR_VERSION (MYTHIC_ENGINE_VERSION >> 2) & 1

#define MYTHIC_ENGINE_PATCH_VERSION MYTHIC_ENGINE_VERSION & 1

#define MYTHIC_ENGINE_WIN_API __stdcall

#define __NODISCARD__ [[nodiscard]]

#define __UNUSED__ [[maybe_unused]]

#define MYTHIC_ENGINE_EXPORT __declspec(dllexport)

#define MYTHIC_ENGINE_IMPORT __declspec(dllimport)

#define __CONSTEXPR__ constexpr

#define __INLINE__ inline

#define MYTHIC_ENGINE_DEFAULT_CONSTRUCT_AND_OPERATORS(__CLASS__)               \
  __CLASS__() = default;                                                       \
  __CLASS__(const __CLASS__ &) = default;                                      \
  __CLASS__ &operator=(const __CLASS__ &) = default;                           \
  __CLASS__(__CLASS__ &&) noexcept = default;                                  \
  __CLASS__ &operator=(__CLASS__ &&) noexcept = default;