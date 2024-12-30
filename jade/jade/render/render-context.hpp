#pragma once

#include "shader-manager.hpp"

#include <SDL_video.h>
#include <functional>
#include <memory>
#include <utility>

namespace jade {

  enum class RenderInterface
  {
    OpenGL,
    Direct3D9,
    Direct3D11,
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

    auto create_shader(ShaderType type, const char* buffer) const -> ShaderHandle;
    void delete_shader(ShaderHandle shader_handle) const;

  protected:
    SDL_Window* window = nullptr;
    std::unique_ptr<ShaderManager> shader_manager;
  };

} // namespace jade
