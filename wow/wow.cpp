#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

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

   if (!glfwInit()) {
      spdlog::error("Failed to initialize GLFW!");
      return -1;
   }

   jade::ScopeExit terminate_glfw([]() {
      spdlog::info("Terminating GLFW...");
      glfwTerminate();
   });

   glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   glfwSetErrorCallback([](int error, const char* description) {
      spdlog::error("Error code: 0x{:x}", error);
      spdlog::error("Error description: {}", description);
   });

   // Create a windowed mode window and its OpenGL context
   GLFWwindow* window = glfwCreateWindow(1920, 1080, "WoW", nullptr, nullptr);
   if (!window) {
      spdlog::error("Failed to create a GLFW window!");
      return -1;
   }

   jade::ScopeExit terminate_window([window]() {
      spdlog::info("Terminating the window...");
      glfwDestroyWindow(window);
   });

   // Make the window's context current
   glfwMakeContextCurrent(window);

   // VSYNC
   glfwSwapInterval(1);

   if (gl3wInit()) {
      spdlog::error("Failed to initialize gl3w!");
      return -1;
   }

   // Main loop
   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      int display_w;
      int display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);

      // Basic render
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glfwSwapBuffers(window);
   }

   return 0;
}
