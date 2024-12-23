#pragma once

#ifdef JADE_D3D11_SUPPORT

#include <jade/render/render-context.hpp>

#include <d3d11.h>

namespace jade {

  class RenderContextD3D11 final : public RenderContext
  {
  public:
    explicit RenderContextD3D11(SDL_Window* window);
    ~RenderContextD3D11() override;

    void draw_scene(const std::function<void()>& callback) override;
    auto get_drawable_size() -> std::pair<int, int> override;
    void viewport(int x, int y, int w, int h) override;
    void clear(float r, float g, float b, float a) override;

  private:
    ID3D11Device* d3d_device = nullptr;
    ID3D11DeviceContext* d3d_device_context = nullptr;
    IDXGISwapChain* swap_chain = nullptr;
    ID3D11RenderTargetView* render_target_view = nullptr;
  };

} // namespace jade

#endif
