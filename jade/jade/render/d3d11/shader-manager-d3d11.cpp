#include "shader-manager-d3d11.hpp"

#include "jade/utils/assert.hpp"

#ifdef JADE_D3D11_SUPPORT

#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

#include <spdlog/spdlog.h>

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

  union
  {
    ID3D11PixelShader* pixel_shader = nullptr;
    ID3D11VertexShader* vertex_shader;
  };
};

auto
jade::ShaderManagerD3D11::create_shader(ShaderType type, const char* buffer) -> ShaderHandle
{
  const char* shader_target = type == ShaderType::Fragment ? "ps_5_0" : "vs_5.0";
  auto* shader = new ShaderD3D11(type, nullptr);

  {
    ID3DBlob* error_blob = nullptr;
    const HRESULT hr = D3DCompile(buffer, strlen(buffer),
      nullptr,       // source name
      nullptr,       // defines
      nullptr,       // include handler
      "main",        // entry point
      shader_target, // shader target
      D3DCOMPILE_ENABLE_STRICTNESS,
      0, // effect flags
      &shader->blob, &error_blob);

    if (FAILED(hr) && error_blob) {
      spdlog::error("%s", error_blob->GetBufferPointer());
      error_blob->Release();
    }
  }

  switch (type) {
    case ShaderType::Vertex: {
      const HRESULT hr = device->CreateVertexShader( //
        shader->blob->GetBufferPointer(),            //
        shader->blob->GetBufferSize(),               //
        nullptr,                                     //
        &shader->vertex_shader);
      JADE_ASSERT(!FAILED(hr));
    } break;
    case ShaderType::Fragment: {
      const HRESULT hr = device->CreatePixelShader( //
        shader->blob->GetBufferPointer(),           //
        shader->blob->GetBufferSize(),              //
        nullptr,                                    //
        &shader->pixel_shader);
      JADE_ASSERT(!FAILED(hr));
    } break;
    default:
      JADE_ASSERT(false);
  }

  return create_handle(shader);
}

void
jade::ShaderManagerD3D11::delete_shader(ShaderHandle shader_handle)
{
  delete_handle(shader_handle);
}

#endif
