#include <pch.h>
#include "Exceptions.h"

namespace MythicEngine::render::directx11 {

Exception::Exception(size_t line, const char *fileName,
                     std::string_view message)
    : support::utils::Exception(static_cast<int>(line), fileName,
                                message.data()),
      message_(message) {}

const char *Exception::what() const noexcept { return message_.c_str(); }

const char *Exception::GetType() const noexcept { return "DirectX11"; }

} // namespace MythicEngine::render::directx11