#include "render-context-d3d9.hpp"

#ifdef JADE_D3D9_SUPPORT

#include <SDL_syswm.h>

namespace jade {

  RenderContextDX9::RenderContextDX9(SDL_Window* window)
    : RenderContext(window)
    , d3d(Direct3DCreate9(D3D_SDK_VERSION))
  {
    if (!d3d) {
      return;
    }

    SDL_SysWMinfo wm_info;
    SDL_VERSION(&wm_info.version)
    SDL_GetWindowWMInfo(window, &wm_info);
    auto* hwnd = wm_info.info.win.window;

    auto [width, height] = get_drawable_size();

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Enable VSYNC

    if (FAILED(d3d->CreateDevice(
          D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3d_device))) {
      d3d->Release();
      d3d = nullptr;
      return;
    }
  }

  RenderContextDX9::~RenderContextDX9()
  {
    d3d_device->Release();
    d3d->Release();
  }

  auto RenderContextDX9::get_drawable_size() -> std::pair<int, int>
  {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return { w, h };
  }

  void RenderContextDX9::viewport(int x, int y, int w, int h)
  {
    D3DVIEWPORT9 vp;
    vp.X = x;
    vp.Y = y;
    vp.Width = w;
    vp.Height = h;
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;
    (void)d3d_device->SetViewport(&vp);
  }

  void RenderContextDX9::clear(float r, float g, float b, float a)
  {
    (void)d3d_device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(r, g, b, a), 1.0f, 0);
  }

  void RenderContextDX9::draw_scene(const std::function<void()>& callback)
  {
    if (FAILED(d3d_device->BeginScene())) {
      return;
    }

    callback();

    if (FAILED(d3d_device->EndScene())) {
      return;
    }

    (void)d3d_device->Present(nullptr, nullptr, nullptr, nullptr);
  }

} // namespace jade

#endif
