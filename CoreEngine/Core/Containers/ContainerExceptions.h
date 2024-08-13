#pragma once

#include <Support/Utils/Exceptions.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::core::containers {
class OutOfRangeException final : public support::utils::Exception {
public:
  MYTHIC_ENGINE_EXPORT OutOfRangeException();
  MYTHIC_ENGINE_EXPORT explicit OutOfRangeException(size_t, const char *);

  MYTHIC_ENGINE_EXPORT const char *what() const noexcept override;
  MYTHIC_ENGINE_EXPORT const char *GetType() const noexcept override;

private:
  const std::string message_;
};
} // namespace MythicEngine::core::containers

#ifdef _SOURCE_LOCATION_
#define OUT_OF_RANGE                                                           \
  MythicEngine::core::containers::OutOfRangeException(                         \
      std::source_location::current().line(),                                  \
      std::source_location::current().file_name())

#else
#define OUT_OF_RANGE                                                           \
  MythicEngine::core::containers::OutOfRangeException(__LINE__, __FILE__)

#endif