#include "shader-manager.hpp"

#include <jade/utils/assert.hpp>

auto
jade::ShaderManager::is_valid(ShaderHandle shader_handle) const -> bool
{
  return shaders.contains(shader_handle.id);
}

auto
jade::ShaderManager::create_handle(Shader* shader) -> ShaderHandle
{
  ShaderHandle::IdType id = next_index++;
  shaders.insert({ id, shader });

  ShaderHandle shader_handle;
  shader_handle.id = id;

  return shader_handle;
}

void
jade::ShaderManager::delete_handle(ShaderHandle shader_handle)
{
  JADE_ASSERT(shader_handle.id != 0);
  auto it = shaders.find(shader_handle.id);
  JADE_ASSERT(it != shaders.end());
  JADE_ASSERT(it->second != nullptr);
  delete it->second;
  shaders.erase(it);
}
