#include "main_thread_task_queue.hpp"

namespace wow {

  MainThreadTaskQueue& MainThreadTaskQueue::get_instance()
  {
    static MainThreadTaskQueue instance;
    return instance;
  }

} // namespace wow
