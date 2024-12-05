#pragma once

#include <jade/threads/task_queue.hpp>

namespace wow {

  struct MainThreadTaskQueue : public jade::TaskQueue
  {
    static MainThreadTaskQueue& get_instance();
  };

} // namespace wow
