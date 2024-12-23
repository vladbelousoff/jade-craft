
#include <filesystem/mpq_file_manager.hpp>
#include <jade/args/args_processor.hpp>
#include <jade/engine/application_context.hpp>
#include <jade/utils/scope_exit.hpp>

#include <spdlog/spdlog.h>

namespace wow {

  MPQFileManager* file_manager = nullptr;

} // namespace wow

namespace wow {

  class GameContext final : public jade::IApplicationContext
  {
  public:
    void on_init() override
    {
    }

    void on_term() override
    {
    }

    void on_update(float dt) override
    {
      spdlog::info("FPS: {}", 1.f / dt);
    }

    void on_render(jade::RenderContext* render_context) override
    {
      render_context->clear(0.1f, 0.1f, 0.1f, 1.0f);
    }
  };

} // namespace wow

int
main(int argc, char* argv[])
{
  using namespace wow;

  auto& args = jade::ArgsProcessor::get_instance();
  args.process_args(argc, argv);

  if (constexpr auto root = "-root"; args.is_set(root)) {
    file_manager = new MPQFileManager(args.get_value(root));
  } else {
    file_manager = new MPQFileManager(std::filesystem::current_path());
  }

  jade::ScopeExit terminate_file_manager([] {
    spdlog::info("Terminating MPQ file manager...");
    delete file_manager;
    file_manager = nullptr;
  });

  // Default value based on what is supported
  auto render_interface =
#ifdef JADE_D3D9_SUPPORT
    jade::RenderInterface::Direct3D9
#else
    jade::RenderInterface::OpenGL
#endif
    ;

  // If we forcefully use opengl
  if (constexpr auto open_gl = "-opengl"; args.is_set(open_gl)) {
    render_interface = jade::RenderInterface::OpenGL;
  }

  jade::StringView render_context_name;
  switch (render_interface) {
    case jade::RenderInterface::OpenGL:
      render_context_name = "OpenGL";
      break;
    case jade::RenderInterface::Direct3D9:
      render_context_name = "Direct3D9";
      break;
  }

  GameContext game_context;
  const auto title = fmt::format("WoW [{}]", render_context_name);
  return run_app(&game_context, render_interface, title);
}
