#include <pch.h>
#include "Render.h"

namespace MythicEngine::render::directx11 {

DirectX11Render::DirectX11Render(const ITimeDispatcherPtr &dispatcher,
                                 HWND hWnd)
    : subscription_(this, dispatcher.lock().get()), device_(nullptr),
      swapChain_(nullptr), context_(nullptr) {
  Init(hWnd);
}

void MYTHIC_ENGINE_WIN_API DirectX11Render::Init(HWND hWnd) noexcept {
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 0;
  sd.BufferDesc.RefreshRate.Denominator = 0;
  sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount = 1;
  sd.OutputWindow = hWnd;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags = 0;

  if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                    0, nullptr, 0, D3D11_SDK_VERSION, &sd,
                                    &swapChain_, &device_, nullptr,
                                    &context_) != S_OK) {
  }
}

void MYTHIC_ENGINE_WIN_API DirectX11Render::Release() noexcept {
  if (swapChain_) {
    swapChain_->Release();
    swapChain_ = nullptr;
  }

  if (context_) {
    context_->Release();
    context_ = nullptr;
  }

  if (device_) {
    device_->Release();
    device_ = nullptr;
  }
}

DirectX11Render::~DirectX11Render() { Release(); }

void MYTHIC_ENGINE_WIN_API DirectX11Render::EndFrame() {
  swapChain_->Present(1, 0);
}

void MYTHIC_ENGINE_WIN_API DirectX11Render::OnUpdate(float) { EndFrame(); }

} // namespace MythicEngine::render::directx11