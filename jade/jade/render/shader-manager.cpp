#include "shader-manager.hpp"

#include <spdlog/spdlog.h>

auto
jade::ShaderManager::create_shader(ShaderType, const char*) -> ShaderId
{
  generations.push_back(0);
  return ShaderId(next_id++, 0);
}

void
jade::ShaderManager::delete_shader(ShaderId shader_id)
{
  if (is_valid(shader_id)) {
    generations[shader_id.unique_id.id - 1]++;
  } else {
    spdlog::warn("Attempted to delete a shader with id {}, which is not valid", shader_id.unique_id.id);
  }
}

auto
jade::ShaderManager::is_valid(ShaderId shader_id) const -> bool
{
  if (shader_id.unique_id.id > 0 && shader_id.unique_id.id <= generations.size()) {
    return generations[shader_id.unique_id.id - 1] == shader_id.unique_id.generation;
  }

  return false;
}
