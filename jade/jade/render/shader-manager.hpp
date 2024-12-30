#pragma once

#include <unordered_map>

namespace jade {

  enum class ShaderType
  {
    Vertex,
    Fragment,
  };

  class ShaderManager;

  class ShaderHandle
  {
    friend class ShaderManager;

  public:
    using IdType = std::size_t;

  private:
    IdType id = 0;
  };

  class Shader
  {
    friend class ShaderManager;

  public:
    explicit Shader(ShaderType type)
      : type(type)
    {
    }

    virtual ~Shader() = default;

  protected:
    ShaderType type;
  };

  class ShaderManager
  {
  public:
    virtual ~ShaderManager() = default;

    virtual auto create_shader(ShaderType type, const char* buffer) -> ShaderHandle = 0;
    virtual void delete_shader(ShaderHandle shader_handle) = 0;

    auto is_valid(ShaderHandle shader_handle) const -> bool;

  protected:
    auto create_handle(Shader* shader) -> ShaderHandle;
    void delete_handle(ShaderHandle shader_handle);

  private:
    // No smart pointers on purpose, because the pointers
    // are always stored inside the system
    std::unordered_map<ShaderHandle::IdType, Shader*> shaders;

    // Shader counter
    ShaderHandle::IdType next_index = 1;
  };

} // namespace jade
