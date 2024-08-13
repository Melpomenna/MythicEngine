#pragma once

#include <Support/Utils/Utils.h>

namespace MythicEngine::render::interfaces {

class MYTHIC_ENGINE_EXPORT IRender {
public:
  IRender() = default;
  virtual ~IRender() = default;

  IRender(const IRender &) = default;
  IRender &operator=(const IRender &) = default;

  IRender(IRender &&) noexcept = default;
  IRender &operator=(IRender &&) noexcept = default;

public:
  virtual void EndFrame() = 0;
};

} // namespace MythicEngine::render::interfaces