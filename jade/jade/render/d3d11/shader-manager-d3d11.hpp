#pragma once

#ifdef JADE_D3D11_SUPPORT

#include <jade/render/shader-manager.hpp>

namespace jade {

  class ShaderManagerD3D11 final : public ShaderManager
  {
  public:
    ~ShaderManagerD3D11() override = default;

    auto create_shader(ShaderType type, const char* buffer) -> ShaderId override;
    void delete_shader(ShaderId shader_id) override;
  };

} // namespace jade

#endif
