#include "render-context-open-gl.hpp"

#include <GL/gl3w.h>

namespace jade {

  RenderContextOpenGL::RenderContextOpenGL(SDL_Window* window)
    : RenderContext(window)
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
      return;
    }

    // Enable VSYNC
    SDL_GL_SetSwapInterval(1);

    if (gl3wInit()) {
      return;
    }
  }

  RenderContextOpenGL::~RenderContextOpenGL()
  {
    SDL_GL_DeleteContext(gl_context);
  }

  auto RenderContextOpenGL::get_drawable_size() -> std::pair<int, int>
  {
    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    return { w, h };
  }

  void RenderContextOpenGL::viewport(int x, int y, int w, int h)
  {
    glViewport(x, y, w, h);
  }

  void RenderContextOpenGL::clear(float r, float g, float b, float a)
  {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void RenderContextOpenGL::draw_scene(const std::function<void()>& callback)
  {
    callback();
    SDL_GL_SwapWindow(window);
  }

} // namespace jade
