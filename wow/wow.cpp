#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include <spdlog/spdlog.h>

#include <jade/utils/scope_exit.hpp>

#include "filesystem/mpq_file_manager.hpp"
#include "jade/args/args_processor.hpp"
#include "render/render_context/dx9/render_context_dx_9.hpp"
#include "render/render_context/open_gl/render_context_open_gl.hpp"

namespace wow {

   struct GlobalStorage
   {
      std::unique_ptr<MPQFileManager> mpq_file_manager;
      std::unique_ptr<RenderContext> render_context;
   };

   std::unique_ptr<GlobalStorage> global;

} // namespace wow

int
main(int argc, char* argv[])
{
   auto& args = jade::ArgsProcessor::get_instance();
   args.process_args(argc, argv);

   wow::global = std::make_unique<wow::GlobalStorage>();
   jade::ScopeExit terminate_global([] {
      wow::global = nullptr;
      spdlog::info("Global storage terminated");
   });

   if (constexpr auto root = "-root"; args.is_set(root)) {
      wow::global->mpq_file_manager = std::make_unique<wow::MPQFileManager>(args.get_value(root));
   } else {
      wow::global->mpq_file_manager = std::make_unique<wow::MPQFileManager>(std::filesystem::current_path());
   }

   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      spdlog::error("Failed to initialize SDL2: {}", SDL_GetError());
      return -1;
   }

   jade::ScopeExit terminate_sdl([] {
      spdlog::info("Terminating SDL2...");
      SDL_Quit();
   });

#ifdef WOW_ENABLE_D3D9
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

   auto get_render_context = [&]() -> std::unique_ptr<RenderContext> {
#ifdef WOW_ENABLE_D3D9
      if (d3d9_active) {
         return std::make_unique<wow::RenderContextDX9>(window);
      }
#endif
      return std::make_unique<wow::RenderContextOpenGL>(window);
   };

   wow::global->render_context = get_render_context();
   RenderContext* render_context = wow::global->render_context.get();

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

      auto [display_w, display_h] = render_context->get_drawable_size();
      render_context->viewport(0, 0, display_w, display_h);

      render_context->draw_scene([&] {
         render_context->clear(0.1f, 0.1f, 0.1f, 1.0f);
      });
   }

   return 0;
}
