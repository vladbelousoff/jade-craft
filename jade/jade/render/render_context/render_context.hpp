#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include <utility>

namespace jade {

  enum class RenderInterface
  {
    OpenGL,
    Direct3D9,
  };

  class RenderContext
  {
  public:
    explicit RenderContext(SDL_Window* window)
      : window(window)
    {
    }

    virtual ~RenderContext() = default;

    virtual void draw_scene(const std::function<void()>& callback) = 0;
    virtual auto get_drawable_size() -> std::pair<int, int> = 0;
    virtual void viewport(int x, int y, int w, int h) = 0;
    virtual void clear(float r, float g, float b, float a) = 0;

  protected:
    SDL_Window* window = nullptr;
  };

} // namespace jade
