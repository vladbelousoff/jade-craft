#pragma once

#ifdef WOW_ENABLE_D3D9

#include "render/render_context/render_context.hpp"

#include <d3d9.h>

namespace wow {

  class RenderContextDX9 final : public RenderContext
  {
  public:
    explicit RenderContextDX9(SDL_Window* window);
    ~RenderContextDX9() override;

    void draw_scene(const std::function<void()>& callback) override;
    auto get_drawable_size() -> std::pair<int, int> override;
    void viewport(int x, int y, int w, int h) override;
    void clear(float r, float g, float b, float a) override;

  private:
    IDirect3D9* d3d = nullptr;
    IDirect3DDevice9* d3d_device = nullptr;
  };

} // namespace wow

#endif
