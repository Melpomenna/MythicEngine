#include <pch.h>

#include "ContainerExceptions.h"

#include <source_location>

namespace MythicEngine::core::containers {

OutOfRangeException::OutOfRangeException()
    : Exception(std::source_location::current().line(),
                std::source_location::current().file_name(), "Out of range"),
      message_("Out of range") {}

OutOfRangeException::OutOfRangeException(size_t line, const char *filename)
    : Exception(static_cast<int>(line), filename, "Out of range"),
      message_("Out of range") {}

const char *MYTHIC_ENGINE_WIN_API OutOfRangeException::what() const noexcept {
  return message_.c_str();
}

const char *MYTHIC_ENGINE_WIN_API
OutOfRangeException::GetType() const noexcept {
  return message_.c_str();
}

} // namespace MythicEngine::core::containers