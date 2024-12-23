#include "main-thread-task-queue.hpp"

namespace wow {

  MainThreadTaskQueue& MainThreadTaskQueue::get_instance()
  {
    static MainThreadTaskQueue instance;
    return instance;
  }

} // namespace wow
