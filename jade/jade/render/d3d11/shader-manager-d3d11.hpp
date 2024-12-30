#pragma once

#ifdef JADE_D3D11_SUPPORT

#include <SDL_system.h>

#include <jade/render/shader-manager.hpp>

namespace jade {

  class ShaderManagerD3D11 final : public ShaderManager
  {
  public:
    explicit ShaderManagerD3D11(ID3D11Device* device)
      : device(device)
    {
    }

    ~ShaderManagerD3D11() override = default;

    auto create_shader(ShaderType type, const char* buffer) -> ShaderHandle override;
    void delete_shader(ShaderHandle shader_handle) override;

  private:
    ID3D11Device* device;
  };

} // namespace jade

#endif
