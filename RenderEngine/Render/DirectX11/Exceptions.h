#pragma once

#include <Support/Utils/Exceptions.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::render::directx11 {
class Exception final : public support::utils::Exception {
public:
  MYTHIC_ENGINE_DEFAULT_CONSTRUCT_AND_OPERATORS(Exception)
  explicit Exception(size_t, const char *, std::string_view);
  ~Exception() override = default;

  const char *what() const noexcept override;
  const char *GetType() const noexcept override;

private:
  const std::string message_;
};
} // namespace MythicEngine::render::directx11