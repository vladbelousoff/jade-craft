#pragma once

#include <vector>

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
    ShaderId() = default;

    explicit ShaderId(std::uint32_t id, std::uint32_t generation)
      : unique_id(id, generation)
    {
    }

    bool operator==(const ShaderId& other) const
    {
      return unique_id.id == other.unique_id.id && unique_id.generation == other.unique_id.generation;
    }

    bool operator!=(const ShaderId& other) const
    {
      return !(*this == other);
    }

  private:
    struct UniqueId
    {
      std::uint32_t id = 0;
      std::uint32_t generation = 0;

      UniqueId() = default;

      UniqueId(std::uint32_t id, std::uint32_t generation)
        : id(id)
        , generation(generation)
      {
      }
    };

    UniqueId unique_id;
  };

  class ShaderManager
  {
  public:
    virtual ~ShaderManager() = default;

    virtual auto create_shader(ShaderType type, const char* buffer) -> ShaderId;
    virtual void delete_shader(ShaderId shader_id);

    auto is_valid(ShaderId shader_id) const -> bool;

  private:
    std::vector<std::uint32_t> generations;
    std::uint32_t next_id = 1;
  };

} // namespace jade
