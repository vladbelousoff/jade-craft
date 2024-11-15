#pragma once

#include "render/render_context/render_context.hpp"

namespace wow {

   class RenderContextOpenGL : public IRenderContext
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
      SDL_GLContext gl_context = nullptr;
   };

} // namespace wow
