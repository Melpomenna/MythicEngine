#pragma once

#include <Support/Utils/Utils.h>

namespace MythicEngine::support::utils {
class MYTHIC_ENGINE_EXPORT LPWStringWrapper final {
public:
  constexpr LPWStringWrapper() = default;
  explicit LPWStringWrapper(const char *buffer);

  constexpr LPWStringWrapper(const LPWStringWrapper &) = default;
  constexpr LPWStringWrapper &operator=(const LPWStringWrapper &) = default;

  constexpr LPWStringWrapper(LPWStringWrapper &&) noexcept = default;
  constexpr LPWStringWrapper &operator=(LPWStringWrapper &&) noexcept = default;

  operator const wchar_t *() const noexcept;
  operator const char *() const noexcept;

private:
  const char *buffer_;
};
} // namespace MythicEngine::support::utils