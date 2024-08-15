#pragma once

#include <ThirdPaty/DirectX11/Include/D3D11.h>
#include <Windows.h>
#include <wrl.h>

#include <Core/Memory/OwnPtr.h>
#include <Core/Time/ITimeDispatcher.h>
#include <Render/Interfaces/IRender.h>
#include <Support/Utils/IUpdateHandler.h>
#include <Support/Utils/NonCopy.h>
#include <Support/Utils/Subscription.h>
#include <Support/Utils/Utils.h>

namespace MythicEngine::render::directx11 {

class DirectX11Render final : public interfaces::IRender,
                              public support::utils::NonCopy,
                              public support::utils::IUpdateHandler {
public:
  using UpdateSubscription = support::utils::Subscription<
      core::memory::OwnPtr<support::utils::IUpdateHandler>,
      core::memory::OwnPtr<core::time::ITimeDispatcher>>;
  using ITimeDispatcherPtr = core::memory::OwnPtr<core::time::ITimeDispatcher>;
  using DevicePtr = Microsoft::WRL::ComPtr<ID3D11Device>;
  using SwapChainPtr = Microsoft::WRL::ComPtr<IDXGISwapChain>;
  using DeviceContextPtr = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
  using ResourcesPtr = Microsoft::WRL::ComPtr<ID3D11Resource>;
  using TargetViewPtr = Microsoft::WRL::ComPtr<ID3D11RenderTargetView>;

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

  MYTHIC_ENGINE_EXPORT static core::memory::OwnPtr<interfaces::IRender> &
  ResolveDirectX11Render(const ITimeDispatcherPtr &dispatcher = {},
                         HWND hWnd = nullptr) noexcept;

private:
  MYTHIC_ENGINE_EXPORT void Init(HWND);
  MYTHIC_ENGINE_EXPORT void Release() noexcept;
  MYTHIC_ENGINE_EXPORT void ClearBuffer(float, float, float,
                                        float) const noexcept;

private:
  DevicePtr device_;
  SwapChainPtr swapChain_;
  DeviceContextPtr context_;
  TargetViewPtr targetView_;
  UpdateSubscription subscription_;
  static __INLINE__ core::memory::OwnPtr<interfaces::IRender> render_;
};

} // namespace MythicEngine::render::directx11