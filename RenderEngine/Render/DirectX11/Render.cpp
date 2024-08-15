#include <pch.h>
#include "Render.h"

#include <Render/DirectX11/Exceptions.h>

namespace MythicEngine::render::directx11 {

DirectX11Render::DirectX11Render(const ITimeDispatcherPtr &dispatcher,
                                 HWND hWnd)
    : device_(nullptr), swapChain_(nullptr), context_(nullptr),
      subscription_(
          core::memory::OwnPtr(this, core::memory::MemoryOrderType::Parent),
          dispatcher) {
  Init(hWnd);
}

core::memory::OwnPtr<interfaces::IRender> &MYTHIC_ENGINE_WIN_API
DirectX11Render::ResolveDirectX11Render(const ITimeDispatcherPtr &dispatcher,
                                        HWND hWnd) noexcept {
  if (!render_) {
    render_ = support::utils::Move(
        core::memory::MakeOwn<DirectX11Render>(dispatcher, hWnd));
  }
  return render_;
}

void MYTHIC_ENGINE_WIN_API DirectX11Render::Init(HWND hWnd) {
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
    throw Exception(__LINE__, __FILE__, "Cannot create device and swap chain");
  }

  ResourcesPtr pBackBuffer;
  if (swapChain_->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer) !=
      S_OK) {
    throw Exception(__LINE__, __FILE__, "Cannot get buffer");
  }

  if (device_->CreateRenderTargetView(pBackBuffer.Get(), nullptr,
                                      &targetView_) != S_OK) {
    throw Exception(__LINE__, __FILE__, "Cannot create render target view");
  }
}

void MYTHIC_ENGINE_WIN_API DirectX11Render::Release() noexcept {}

DirectX11Render::~DirectX11Render() { Release(); }

void MYTHIC_ENGINE_WIN_API DirectX11Render::EndFrame() {
  if (auto hr = swapChain_->Present(1, 0); hr != S_OK) {
    throw Exception(__LINE__, __FILE__,
                    "Failed on present: error code:" + std::to_string(hr));
  }
}

void MYTHIC_ENGINE_WIN_API DirectX11Render::ClearBuffer(
    float r, float g, float b, float a) const noexcept {
  const float color[] = {r, g, b, a};
  context_->ClearRenderTargetView(targetView_.Get(), color);
}

void MYTHIC_ENGINE_WIN_API DirectX11Render::OnUpdate(float dt) { EndFrame(); }

} // namespace MythicEngine::render::directx11