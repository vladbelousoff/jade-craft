#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include <spdlog/spdlog.h>

#include <jade/utils/scope_exit.hpp>

#include "filesystem/mpq_file_manager.hpp"
#include "render/render_context/dx9/render_context_dx_9.hpp"
#include "render/render_context/open_gl/render_context_open_gl.hpp"

namespace wow {

   struct GlobalStorage
   {
      std::unique_ptr<wow::MPQFileManager> mpq_file_manager;
      std::unique_ptr<IRenderContext> render_context;
   };

   GlobalStorage* global = nullptr;

} // namespace wow

int
main(int argc, char* argv[])
{
   wow::global = new wow::GlobalStorage();
   jade::ScopeExit terminate_global([] {
      delete wow::global;
   });

   if (argc > 1) {
      wow::global->mpq_file_manager = std::make_unique<wow::MPQFileManager>(argv[1]);
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

   if (argc > 2 && !strcmp(argv[2], "-opengl")) {
      wow::global->render_context = std::make_unique<wow::RenderContextOpenGL>();
   } else {
      wow::global->render_context = std::make_unique<wow::RenderContextDX9>();
   }

   IRenderContext* render_context = wow::global->render_context.get();

   SDL_Window* window = SDL_CreateWindow("WoW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
       SDL_WINDOW_SHOWN | render_context->get_window_flags());

   if (!window) {
      spdlog::error("Failed to create an SDL2 window: {}", SDL_GetError());
      return -1;
   }

   jade::ScopeExit terminate_window([window] {
      spdlog::info("Destroying window...");
      SDL_DestroyWindow(window);
   });

   if (!render_context->init(window)) {
      spdlog::error("Failed to create render context: {}", SDL_GetError());
      return -1;
   }

   jade::ScopeExit terminate_render_context([window, render_context] {
      spdlog::info("Terminating render context...");
      render_context->term(window);
   });

   bool running = true;
   SDL_Event event;

   while (running) {
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT) {
            running = false;
         }
      }

      auto [display_w, display_h] = render_context->get_drawable_size(window);
      render_context->viewport(0, 0, display_w, display_h);

      render_context->clear(0.1f, 0.1f, 0.1f, 1.0f);
      render_context->init_scene();
      render_context->term_scene();

      render_context->swap(window);
   }

   return 0;
}