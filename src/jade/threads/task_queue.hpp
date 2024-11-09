#pragma once

#include <functional>
#include <mutex>
#include <queue>

namespace jade {

   class TaskQueue
   {
      using Task = std::function<void()>;

    public:
      void add_task(Task&& task);
      void perform_all_tasks();

    private:
      auto is_empty() -> bool;
      auto get_next_task() -> Task;

      std::mutex mutex = {};
      std::queue<Task> task_queue = {};
   };

} // namespace jade
