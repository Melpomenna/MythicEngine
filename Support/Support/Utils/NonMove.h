#pragma once

#include <Support/Utils/Utils.h>

namespace MythicEngine::support::utils {
class MYTHIC_ENGINE_EXPORT NonMove {
public:
  NonMove() = default;
  virtual ~NonMove() = default;

  NonMove(const NonMove &) = default;
  NonMove &operator=(const NonMove &) = default;

  NonMove(NonMove &&) noexcept = delete;
  NonMove &operator=(NonMove &&) noexcept = delete;
};
} // namespace MythicEngine::support::utils