#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include <spdlog/spdlog.h>

#include <jade/utils/scope_exit.hpp>

#include "filesystem/mpq_file_manager.hpp"
#include "jade/args/args_processor.hpp"
#include "render/render_context/dx9/render_context_dx9.hpp"
#include "render/render_context/gl/render_context_open_gl.hpp"

namespace wow {

  MPQFileManager* GFileManager = nullptr;
  RenderContext* GRenderContext = nullptr;

} // namespace wow

int
main(int argc, char* argv[])
{
  using namespace wow;

  auto& args = jade::ArgsProcessor::get_instance();
  args.process_args(argc, argv);

  if (constexpr auto root = "-root"; args.is_set(root)) {
    GFileManager = new MPQFileManager(args.get_value(root));
  } else {
    GFileManager = new MPQFileManager(std::filesystem::current_path());
  }

  jade::ScopeExit terminate_file_manager([] {
    spdlog::info("Terminating GFileManager...");
    delete GFileManager;
    GFileManager = nullptr;
  });

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    spdlog::error("Failed to initialize SDL2: {}", SDL_GetError());
    return -1;
  }

  jade::ScopeExit terminate_sdl([] {
    spdlog::info("Terminating SDL2...");
    SDL_Quit();
  });

#ifdef WOW_D3D9_SUPPORT
  const bool d3d9_active = !args.is_set("-opengl") || args.is_set("-d3d9");
#else
  constexpr bool d3d9_active = false;
#endif

  SDL_Window* window = SDL_CreateWindow(d3d9_active ? "WoW [D3D9]" : "WoW [OpenGL]", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 1280, 720, d3d9_active ? SDL_WINDOW_SHOWN : SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (!window) {
    spdlog::error("Failed to create an SDL2 window: {}", SDL_GetError());
    return -1;
  }

  // ReSharper disable CppDFAMemoryLeak

  auto get_render_context = [&]() -> RenderContext* {
#ifdef WOW_D3D9_SUPPORT
    if (d3d9_active) {
      return new RenderContextDX9(window);
    }
#endif
    return new RenderContextOpenGL(window);
  };

  GRenderContext = get_render_context();
  jade::ScopeExit terminate_render_context([] {
    spdlog::info("Terminating GRenderContext...");
    delete GRenderContext;
    GRenderContext = nullptr;
  });

  // ReSharper enable CppDFAMemoryLeak

  jade::ScopeExit terminate_window([window] {
    spdlog::info("Destroying window...");
    SDL_DestroyWindow(window);
  });

  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    auto [display_w, display_h] = GRenderContext->get_drawable_size();
    GRenderContext->viewport(0, 0, display_w, display_h);

    GRenderContext->draw_scene([&] {
      GRenderContext->clear(0.1f, 0.1f, 0.1f, 1.0f);
    });
  }

  return 0;
}
