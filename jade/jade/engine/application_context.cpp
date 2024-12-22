#include "application_context.hpp"

#include <jade/render/render_context/dx9/render_context_dx9.hpp>
#include <jade/render/render_context/gl/render_context_open_gl.hpp>
#include <jade/render/render_context/render_context.hpp>
#include <jade/utils/scope_exit.hpp>
#include <jade/utils/string_manager.hpp>

#include <SDL_video.h>
#include <spdlog/spdlog.h>

namespace jade {

  int run_game(IApplicationContext* application_context, RenderInterface render_interface, StringView title)
  {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      spdlog::error("Failed to initialize SDL2: {}", SDL_GetError());
      return -1;
    }

    ScopeExit terminate_sdl([] {
      spdlog::info("Terminating SDL2...");
      SDL_Quit();
    });

    int window_flags = SDL_WINDOW_SHOWN;
    if (render_interface == RenderInterface::OpenGL) {
      window_flags |= SDL_WINDOW_OPENGL;
    }

    constexpr int x = SDL_WINDOWPOS_CENTERED;
    constexpr int y = SDL_WINDOWPOS_CENTERED;

    SDL_Window* window = SDL_CreateWindow(title.data(), x, y, 1280, 720, window_flags);
    if (!window) {
      spdlog::error("Failed to create an SDL2 window: {}", SDL_GetError());
      return -1;
    }

    ScopeExit terminate_window([window] {
      spdlog::info("Destroying window...");
      SDL_DestroyWindow(window);
    });

    RenderContext* render_context = nullptr;
    switch (render_interface) {
      case RenderInterface::OpenGL:
        render_context = new RenderContextOpenGL(window);
        break;
      case RenderInterface::Direct3D9:
#ifdef JADE_D3D9_SUPPORT
        render_context = new RenderContextDX9(window);
#endif
        break;
    }

    ScopeExit terminate_render_context([&] {
      if (render_context) {
        spdlog::info("Terminating render context...");
        delete render_context;
        render_context = nullptr;
      }
    });

    application_context->on_init();

    bool running = true;
    SDL_Event event;

    while (running) {
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          running = false;
        }
      }

      application_context->on_update();

      if (!render_context) {
        // Headless mode
        continue;
      }

      auto [display_w, display_h] = render_context->get_drawable_size();
      render_context->viewport(0, 0, display_w, display_h);
      application_context->on_render(render_context);
    }

    return 0;
  }

} // namespace jade
