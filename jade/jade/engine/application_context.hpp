#pragma once

#include <jade/render/render_context/render_context.hpp>
#include <jade/utils/string_manager.hpp>

namespace jade {

  struct IApplicationContext
  {
    virtual void on_init() = 0;
    virtual void on_term() = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_render(RenderContext* context) = 0;
  };

  int run_game(IApplicationContext* application_context, RenderInterface render_interface, StringView title);

} // namespace jade
