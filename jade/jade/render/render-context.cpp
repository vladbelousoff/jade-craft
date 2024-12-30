#pragma once

#include "render-context.hpp"

auto
jade::RenderContext::create_shader(ShaderType type, const char* buffer) const -> ShaderHandle
{
  return shader_manager->create_shader(type, buffer);
}

void
jade::RenderContext::delete_shader(ShaderHandle shader_handle) const
{
  shader_manager->delete_shader(shader_handle);
}
