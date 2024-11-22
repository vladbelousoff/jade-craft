#pragma once

#include <jade/threads/task_queue.hpp>
#include <jade/utils/string_manager.hpp>

#include <filesystem>
#include <thread>

#include "mpq_chain.hpp"

namespace wow {

   class MPQFileManager
   {
    public:
      explicit MPQFileManager(const std::filesystem::path& root);
      virtual ~MPQFileManager();

    private:
      jade::TaskQueue task_queue;
      std::thread thread;
      MPQChain chain{};

      void loop();
   };

} // namespace wow
