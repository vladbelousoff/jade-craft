#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include <spdlog/spdlog.h>

#include <jade/utils/scope_exit.hpp>

#include "filesystem/mpq_file_manager.hpp"

namespace wow {

   struct GlobalStorage
   {
      std::unique_ptr<wow::MPQFileManager> mpq_file_manager;
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

   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

   SDL_Window* window = SDL_CreateWindow(
       "WoW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

   if (!window) {
      spdlog::error("Failed to create an SDL2 window: {}", SDL_GetError());
      return -1;
   }

   jade::ScopeExit terminate_window([window] {
      spdlog::info("Destroying window...");
      SDL_DestroyWindow(window);
   });

   SDL_GLContext gl_context = SDL_GL_CreateContext(window);
   if (!gl_context) {
      spdlog::error("Failed to create OpenGL context: {}", SDL_GetError());
      return -1;
   }

   jade::ScopeExit terminate_gl_context([&] {
      spdlog::info("Destroying OpenGL context...");
      SDL_GL_DeleteContext(gl_context);
   });

   // Enable VSYNC
   SDL_GL_SetSwapInterval(1);

   if (gl3wInit()) {
      spdlog::error("Failed to initialize gl3w!");
      return -1;
   }

   bool running = true;
   SDL_Event event;

   while (running) {
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT) {
            running = false;
         }
      }

      int display_w;
      int display_h;
      SDL_GL_GetDrawableSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);

      // Render
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      SDL_GL_SwapWindow(window);
   }

   return 0;
}
