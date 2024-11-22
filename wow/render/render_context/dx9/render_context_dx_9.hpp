#pragma once

#ifdef WOW_ENABLE_D3D9

#include "render/render_context/render_context.hpp"

#include <d3d9.h>

namespace wow {

   class RenderContextDX9 final : public IRenderContext
   {
    public:
      auto get_window_flags() -> SDL_WindowFlags override;

      auto init(SDL_Window* window) -> bool override;
      void term(SDL_Window* window) override;

      void draw_scene(SDL_Window* window, const std::function<void()>& callback) override;

      auto get_drawable_size(SDL_Window* window) -> std::pair<int, int> override;
      void viewport(int x, int y, int w, int h) override;

      void clear(float r, float g, float b, float a) override;

    private:
      IDirect3D9* d3d = nullptr;
      IDirect3DDevice9* d3d_device = nullptr;
   };

} // namespace wow

#endif
