#include <pch.h>
#include "Exceptions.h"

#include <Support/Utils/Utils.h>

#include <sstream>

namespace MythicEngine::support::utils {
Exception::Exception(int line, const char *file,
                     const char *exceptionMessage) noexcept
    : line_(line), file_(file),
      exceptionBuffer_(exceptionMessage ? exceptionMessage : "") {}

__NODISCARD__ const char *MYTHIC_ENGINE_WIN_API
Exception::what() const noexcept {
  std::ostringstream oss;
  oss << GetType() << "\n" << GetOriginString();
  if (exceptionBuffer_.empty()) {
    exceptionBuffer_ = oss.str();
  } else {
    exceptionBuffer_ += " " + oss.str();
  }

  return exceptionBuffer_.c_str();
}

__NODISCARD__ const char *MYTHIC_ENGINE_WIN_API
Exception::GetType() const noexcept {
  return "[Error]";
}

__NODISCARD__ const std::string &MYTHIC_ENGINE_WIN_API
Exception::GetFile() const noexcept {
  return file_;
}

__NODISCARD__ unsigned Exception::GetLine() const noexcept { return line_; }

__NODISCARD__ const std::string &MYTHIC_ENGINE_WIN_API
Exception::GetOriginString() const noexcept {
  return exceptionBuffer_;
}
} // namespace MythicEngine::support::utils
