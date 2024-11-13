#pragma once

#include "render/render_context/render_context.hpp"

namespace wow {

   class RenderContextOpenGL : IRenderContext
   {
    public:
      auto get_window_flags() -> SDL_WindowFlags override;

      auto init(SDL_Window* window) -> bool override;
      void term(SDL_Window* window) override;
      void swap(SDL_Window* window) override;

      void get_drawable_size(SDL_Window* window, int& w, int& h) override;

    private:
      SDL_GLContext gl_context = nullptr;
   };

} // namespace wow
