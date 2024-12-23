#pragma once

#include <functional>
#include <mutex>
#include <queue>

namespace jade {

  class TaskQueue
  {
    using Task = std::function<void()>;

  public:
    ~TaskQueue()
    {
      interrupt();
    }

    void add_task(Task&& task);
    void perform_all_tasks();
    void perform_tasks_until_interrupted();
    void interrupt();

  private:
    auto pop_next_task() -> Task;

    std::mutex mutex = {};
    std::queue<Task> task_queue = {};
    std::condition_variable cv = {};
    bool interrupted = false;
  };

} // namespace jade
