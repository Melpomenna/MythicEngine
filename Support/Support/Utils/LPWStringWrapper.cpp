#include <pch.h>
#include "LPWStringWrapper.h"

#include <Support/Utils/Utils.h>

namespace MythicEngine::support::utils {
LPWStringWrapper::LPWStringWrapper(const char *buffer) : buffer_(buffer) {}

__NODISCARD__ MYTHIC_ENGINE_WIN_API
    LPWStringWrapper::operator const wchar_t *() const noexcept {
  if (nullptr == buffer_)
    return nullptr;
  return reinterpret_cast<const wchar_t *>(buffer_);
}

__NODISCARD__ MYTHIC_ENGINE_WIN_API
    LPWStringWrapper::operator const char *() const noexcept {
  return buffer_;
}

} // namespace MythicEngine::support::utils
