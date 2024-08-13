#pragma once

#include <ThirdPaty/DirectX11/Include/D3D11.h>
#include <Windows.h>

#include <Render/Interfaces/IRender.h>
#include <Support/Utils/ITimerDispatcher.h>
#include <Support/Utils/IUpdateHandler.h>
#include <Support/Utils/NonCopy.h>
#include <Support/Utils/Subscription.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::render::directx11 {

class DirectX11Render final : public interfaces::IRender,
                              public support::utils::NonCopy,
                              public support::utils::IUpdateHandler {
public:
  using UpdateSubscription =
      support::utils::Subscription<support::utils::IUpdateHandler,
                                   support::utils::ITimerDispatcher>;
  using ITimeDispatcherPtr = std::weak_ptr<support::utils::ITimerDispatcher>;
  using DevicePtr = ID3D11Device *;
  using SwapChainPtr = IDXGISwapChain *;
  using DeviceContextPtr = ID3D11DeviceContext *;

public:
  MYTHIC_ENGINE_EXPORT DirectX11Render() = delete;
  MYTHIC_ENGINE_EXPORT explicit DirectX11Render(const ITimeDispatcherPtr &,
                                                HWND);

  MYTHIC_ENGINE_EXPORT DirectX11Render(DirectX11Render &&) noexcept = default;
  MYTHIC_ENGINE_EXPORT DirectX11Render &
  operator=(DirectX11Render &&) noexcept = default;

  MYTHIC_ENGINE_EXPORT ~DirectX11Render() override;

  MYTHIC_ENGINE_EXPORT void EndFrame() override;

  MYTHIC_ENGINE_EXPORT void OnUpdate(float) override;

private:
  MYTHIC_ENGINE_EXPORT void Init(HWND) noexcept;
  MYTHIC_ENGINE_EXPORT void Release() noexcept;

private:
  UpdateSubscription subscription_;
  DevicePtr device_;
  SwapChainPtr swapChain_;
  DeviceContextPtr context_;
};

} // namespace MythicEngine::render::directx11