#include "application-context.hpp"

#include <jade/render/d3d11/render-context-d3d11.hpp>
#include <jade/render/open-gl/render-context-open-gl.hpp>
#include <jade/render/render-context.hpp>
#include <jade/strings/string-manager.hpp>
#include <jade/utils/assert.hpp>
#include <jade/utils/scope-exit.hpp>

#include <SDL.h>
#include <SDL_video.h>
#include <spdlog/spdlog.h>

namespace jade {

  int run_app(IApplicationContext* application_context, RenderInterface render_interface, StringView title)
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
#ifdef JADE_D3D11_SUPPORT
      case RenderInterface::Direct3D11:
        render_context = new RenderContextD3D11(window);
        break;
#endif
      default:
        JADE_ASSERT(false);
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

    auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = last_time;

    while (running) {
      current_time = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float> elapsed_time = current_time - last_time;
      const float delta_time = elapsed_time.count();
      last_time = current_time;

      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          running = false;
        }
      }

      application_context->on_update(delta_time);

      if (!render_context) {
        // Headless mode
        continue;
      }

      auto [display_w, display_h] = render_context->get_drawable_size();
      render_context->viewport(0, 0, display_w, display_h);

      render_context->draw_scene([application_context, render_context] {
        application_context->on_render(render_context);
      });
    }

    return 0;
  }

} // namespace jade
