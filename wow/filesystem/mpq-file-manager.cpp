#include "mpq-file-manager.hpp"

#include <spdlog/spdlog.h>

wow::MPQFileManager::MPQFileManager(const std::filesystem::path& root)
  : thread(&MPQFileManager::loop, this)
{
  std::filesystem::path data_root = root / "Data";
  spdlog::info("Data root directory: {}", data_root.string().c_str());

  task_queue.add_task([this, data_root]() {
    chain = MPQChain(data_root);
  });
}

void
wow::MPQFileManager::loop()
{
  task_queue.perform_tasks_until_interrupted();
}

wow::MPQFileManager::~MPQFileManager()
{
  task_queue.interrupt();
  thread.join();
}
