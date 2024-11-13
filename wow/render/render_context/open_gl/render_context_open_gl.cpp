#include "render_context_open_gl.hpp"

#include <GL/gl3w.h>

namespace wow {

   auto RenderContextOpenGL::get_window_flags() -> SDL_WindowFlags
   {
      return SDL_WINDOW_OPENGL;
   }

   auto RenderContextOpenGL::init(SDL_Window* window) -> bool
   {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

      gl_context = SDL_GL_CreateContext(window);
      if (!gl_context) {
         return false;
      }

      // Enable VSYNC
      SDL_GL_SetSwapInterval(1);

      if (gl3wInit()) {
         return false;
      }

      return true;
   }

   void RenderContextOpenGL::term(SDL_Window* window)
   {
      SDL_GL_DeleteContext(gl_context);
   }

   void RenderContextOpenGL::swap(SDL_Window* window)
   {
      SDL_GL_SwapWindow(window);
   }

   void RenderContextOpenGL::get_drawable_size(SDL_Window* window, int& w, int& h)
   {
      SDL_GL_GetDrawableSize(window, &w, &h);
   }

} // namespace wow
