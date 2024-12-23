#pragma once

#include <jade/threads/task-queue.hpp>

#include <filesystem>
#include <thread>

#include "mpq-chain.hpp"

namespace wow {

  class MPQFileManager final
  {
  public:
    explicit MPQFileManager(const std::filesystem::path& root);
    ~MPQFileManager();

  private:
    jade::TaskQueue task_queue;
    std::thread thread;
    MPQChain chain{};

    void loop();
  };

} // namespace wow
