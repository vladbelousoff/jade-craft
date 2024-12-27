#pragma once

#include <objidl.h>
#include <unordered_map>

namespace jade {

  enum class ShaderType
  {
    Vertex,
    Fragment,
  };

  class ShaderManager;

  class ShaderId
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

    virtual auto create_shader(ShaderType type, const char* buffer) -> ShaderId = 0;
    virtual void delete_shader(ShaderId shader_id) = 0;

    auto is_valid(ShaderId shader_id) const -> bool;

  protected:
    auto create_id(Shader* shader) -> ShaderId;
    void delete_id(ShaderId shader_id);

  private:
    // No smart pointers on purpose, because the pointers
    // are always stored inside the system
    std::unordered_map<ShaderId::IdType, Shader*> shaders;

    // Shader counter
    ShaderId::IdType next_index = 1;
  };

} // namespace jade
