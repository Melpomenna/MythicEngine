#pragma once

#include <Support/Utils/TypeTraits.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::core::algorithms {
template <support::utils::GreaterComparable T>
__CONSTEXPR__ __NODISCARD__ T MYTHIC_ENGINE_WIN_API Max(T &&lhs, T &&rhs) {
  const auto ftrue = (lhs > rhs);
  const auto ffalse = ftrue ^ 1;
  return ftrue * support::utils::Forward<T>(lhs) +
         ffalse * support::utils::Forward<T>(rhs);
}

template <support::utils::GreaterComparable T>
__CONSTEXPR__ __NODISCARD__ bool MYTHIC_ENGINE_WIN_API Max(const T &lhs,
                                                           const T &rhs) {
  const auto ftrue = (lhs > rhs);
  const auto ffalse = ftrue ^ 1;
  return ftrue * lhs + ffalse * rhs;
}

template <support::utils::LessComparable T>
__CONSTEXPR__ __NODISCARD__ T MYTHIC_ENGINE_WIN_API Min(T &&lhs, T &&rhs) {
  const auto ftrue = (lhs < rhs);
  const auto ffalse = ftrue ^ 1;
  return ftrue * support::utils::Forward<T>(lhs) +
         ffalse * support::utils::Forward<T>(rhs);
}

template <support::utils::LessComparable T>
__CONSTEXPR__ __NODISCARD__ bool MYTHIC_ENGINE_WIN_API Min(const T &lhs,
                                                           const T &rhs) {
  const auto ftrue = (lhs < rhs);
  const auto ffalse = ftrue ^ 1;
  return ftrue * lhs + ffalse * rhs;
}

} // namespace MythicEngine::core::algorithms