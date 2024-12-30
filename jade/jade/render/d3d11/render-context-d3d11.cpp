#include "render-context-d3d11.hpp"

#ifdef JADE_D3D11_SUPPORT

#include "shader-manager-d3d11.hpp"

#include <jade/utils/assert.hpp>

#include <SDL_syswm.h>
#include <d3d11.h>
#include <dxgi.h>

jade::RenderContextD3D11::RenderContextD3D11(SDL_Window* window)
  : RenderContext(window)
{
  SDL_SysWMinfo wm_info;
  SDL_VERSION(&wm_info.version);
  SDL_GetWindowWMInfo(window, &wm_info);
  auto* hwnd = wm_info.info.win.window;

  auto [width, height] = get_drawable_size();

  // Setup Direct3D 11 device and swap chain
  DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
  swap_chain_desc.BufferCount = 1;
  swap_chain_desc.BufferDesc.Width = width;
  swap_chain_desc.BufferDesc.Height = height;
  swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
  swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.OutputWindow = hwnd;
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.SampleDesc.Quality = 0;
  swap_chain_desc.Windowed = TRUE;

  UINT create_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
  create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL feature_level;
  HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_flags, nullptr, 0,
    D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &d3d_device, &feature_level, &d3d_device_context);
  JADE_ASSERT(!FAILED(hr));

  // Create render target view
  ID3D11Texture2D* back_buffer;
  hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
  JADE_ASSERT(!FAILED(hr));

  hr = d3d_device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
  back_buffer->Release();
  JADE_ASSERT(!FAILED(hr));

  d3d_device_context->OMSetRenderTargets(1, &render_target_view, nullptr);
  shader_manager = std::make_unique<ShaderManagerD3D11>(d3d_device);
}

jade::RenderContextD3D11::~RenderContextD3D11()
{
  shader_manager.reset();

  if (render_target_view) {
    render_target_view->Release();
  }

  if (swap_chain) {
    swap_chain->Release();
  }

  if (d3d_device_context) {
    d3d_device_context->Release();
  }

  if (d3d_device) {
    d3d_device->Release();
  }
}

void
jade::RenderContextD3D11::draw_scene(const std::function<void()>& callback)
{
  callback();

  JADE_ASSERT(swap_chain);

  const HRESULT hr = swap_chain->Present(1, 0); // Enable V-Sync
  JADE_ASSERT(!FAILED(hr));
}

auto
jade::RenderContextD3D11::get_drawable_size() -> std::pair<int, int>
{
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  return { w, h };
}

void
jade::RenderContextD3D11::viewport(int x, int y, int w, int h)
{
  D3D11_VIEWPORT vp;
  vp.TopLeftX = static_cast<FLOAT>(x);
  vp.TopLeftY = static_cast<FLOAT>(y);
  vp.Width = static_cast<FLOAT>(w);
  vp.Height = static_cast<FLOAT>(h);
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  d3d_device_context->RSSetViewports(1, &vp);
}

void
jade::RenderContextD3D11::clear(float r, float g, float b, float a)
{
  const float color[4] = { r, g, b, a };
  d3d_device_context->ClearRenderTargetView(render_target_view, color);
}

#endif
