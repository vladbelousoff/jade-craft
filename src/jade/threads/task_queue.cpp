#include "task_queue.hpp"

void
jade::TaskQueue::add_task(jade::TaskQueue::Task&& task)
{
   std::lock_guard lock(mutex);
   task_queue.emplace(std::forward<Task>(task));
}

auto
jade::TaskQueue::get_next_task() -> jade::TaskQueue::Task
{
   std::lock_guard lock(mutex);
   if (task_queue.empty()) {
      return {};
   }

   auto task = std::move(task_queue.front());
   task_queue.pop();

   return task;
}

auto
jade::TaskQueue::is_empty() -> bool
{
   std::lock_guard lock(mutex);
   return task_queue.empty();
}

void
jade::TaskQueue::perform_all_tasks()
{
   while (!is_empty()) {
      auto task = get_next_task();
      if (task) {
         task();
      }
   }
}
