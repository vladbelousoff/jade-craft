#include "shader-manager-d3d11.hpp"

#ifdef JADE_D3D11_SUPPORT

#include <d3dcommon.h>

class ShaderD3D11 final : public jade::Shader
{
  friend class jade::ShaderManagerD3D11;

public:
  explicit ShaderD3D11(jade::ShaderType type, ID3D10Blob* blob)
    : Shader(type)
    , blob(blob)
  {
  }

  ~ShaderD3D11() override = default;

private:
  ID3DBlob* blob = nullptr;
};

auto
jade::ShaderManagerD3D11::create_shader(ShaderType type, const char* buffer) -> ShaderId
{
  return create_id(new ShaderD3D11(type, nullptr));
}

void
jade::ShaderManagerD3D11::delete_shader(ShaderId shader_id)
{
  delete_id(shader_id);
}

#endif
