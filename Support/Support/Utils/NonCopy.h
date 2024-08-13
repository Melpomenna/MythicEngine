#pragma once

#include <Support/Utils/Utils.h>

namespace MythicEngine::support::utils {
class MYTHIC_ENGINE_EXPORT NonCopy {
public:
  NonCopy() = default;
  virtual ~NonCopy() = default;

  NonCopy(const NonCopy &) = delete;
  NonCopy &operator=(const NonCopy &) = delete;

  NonCopy(NonCopy &&) noexcept = default;
  NonCopy &operator=(NonCopy &&) noexcept = default;
};
} // namespace MythicEngine::support::utils