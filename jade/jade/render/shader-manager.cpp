#include "shader-manager.hpp"

#include <jade/utils/assert.hpp>

auto
jade::ShaderManager::is_valid(ShaderId shader_id) const -> bool
{
  return shaders.contains(shader_id.id);
}

auto
jade::ShaderManager::create_id(Shader* shader) -> ShaderId
{
  ShaderId::IdType id = next_index++;
  shaders.insert({ id, shader });

  ShaderId shader_id;
  shader_id.id = id;

  return shader_id;
}

void
jade::ShaderManager::delete_id(ShaderId shader_id)
{
  JADE_ASSERT(shader_id.id != 0);
  auto it = shaders.find(shader_id.id);
  JADE_ASSERT(it != shaders.end());
  JADE_ASSERT(it->second != nullptr);
  delete it->second;
  shaders.erase(it);
}
