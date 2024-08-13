#pragma once

#include <Support/Utils/Utils.h>

namespace MythicEngine::support::utils {
class MYTHIC_ENGINE_EXPORT IUpdateHandler {
public:
  virtual ~IUpdateHandler() = default;

  virtual void OnUpdate(float) = 0;
};
} // namespace MythicEngine::support::utils
