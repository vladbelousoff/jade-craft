#include "render_context_dx_9.hpp"

#include <SDL_syswm.h>

namespace wow {

   auto RenderContextDX9::get_window_flags() -> SDL_WindowFlags
   {
      return static_cast<SDL_WindowFlags>(0); // No specific SDL flags needed for DX9
   }

   auto RenderContextDX9::init(SDL_Window* window) -> bool
   {
      d3d = Direct3DCreate9(D3D_SDK_VERSION);
      if (!d3d) {
         return false;
      }

      SDL_SysWMinfo wm_info;
      SDL_VERSION(&wm_info.version)
      SDL_GetWindowWMInfo(window, &wm_info);
      HWND hwnd = wm_info.info.win.window;

      auto [width, height] = get_drawable_size(window);

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
         return false;
      }

      return true;
   }

   void RenderContextDX9::term(SDL_Window* window)
   {
      d3d_device->Release();
      d3d_device = nullptr;

      d3d->Release();
      d3d = nullptr;
   }

   void RenderContextDX9::swap(SDL_Window* window)
   {
      d3d_device->Present(nullptr, nullptr, nullptr, nullptr);
   }

   auto RenderContextDX9::get_drawable_size(SDL_Window* window) -> std::pair<int, int>
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
      d3d_device->SetViewport(&vp);
   }

   void RenderContextDX9::clear(float r, float g, float b, float a)
   {
      d3d_device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(r, g, b, a), 1.0f, 0);
   }

   void RenderContextDX9::init_scene()
   {
      d3d_device->BeginScene();
   }

   void RenderContextDX9::term_scene()
   {
      d3d_device->EndScene();
   }

} // namespace wow
