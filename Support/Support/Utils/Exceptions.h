#pragma once

#include <Support/Utils/Utils.h>
#include <exception>
#include <string>

namespace MythicEngine::support::utils {
class Exception : public std::exception {
public:
  MYTHIC_ENGINE_EXPORT ~Exception() override = default;

  MYTHIC_ENGINE_EXPORT Exception(const Exception &) = default;
  MYTHIC_ENGINE_EXPORT Exception &operator=(const Exception &) = default;

  MYTHIC_ENGINE_EXPORT Exception(Exception &&) noexcept = default;
  MYTHIC_ENGINE_EXPORT Exception &operator=(Exception &&) noexcept = default;

  MYTHIC_ENGINE_EXPORT explicit Exception(
      int line, const char *file,
      const char *exceptionMessage = nullptr) noexcept;
  MYTHIC_ENGINE_EXPORT const char *what() const noexcept override;
  MYTHIC_ENGINE_EXPORT virtual const char *GetType() const noexcept;
  MYTHIC_ENGINE_EXPORT const std::string &GetFile() const noexcept;
  MYTHIC_ENGINE_EXPORT unsigned GetLine() const noexcept;
  MYTHIC_ENGINE_EXPORT const std::string &GetOriginString() const noexcept;

private:
  unsigned line_;
  std::string file_;

protected:
  mutable std::string exceptionBuffer_;
};
} // namespace MythicEngine::support::utils