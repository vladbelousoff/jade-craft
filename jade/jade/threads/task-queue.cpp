#include "task-queue.hpp"

void
jade::TaskQueue::add_task(jade::TaskQueue::Task&& task)
{
  std::lock_guard lock(mutex);
  task_queue.emplace(std::forward<Task>(task));
  interrupted = false;
  cv.notify_one();
}

auto
jade::TaskQueue::pop_next_task() -> jade::TaskQueue::Task
{
  if (task_queue.empty()) {
    return {};
  }

  auto task = std::move(task_queue.front());
  task_queue.pop();

  return task;
}

void
jade::TaskQueue::perform_all_tasks()
{
  while (!task_queue.empty()) {
    Task task = {};

    {
      std::unique_lock lock(mutex);
      cv.wait(lock, [this] {
        return interrupted || !task_queue.empty();
      });

      task = pop_next_task();
    }

    if (interrupted || !task) {
      break;
    }

    task();
  }
}

void
jade::TaskQueue::perform_tasks_until_interrupted()
{
  do {
    Task task = {};

    {
      std::unique_lock lock(mutex);
      cv.wait(lock, [this] {
        return interrupted || !task_queue.empty();
      });

      task = pop_next_task();
    }

    if (interrupted) {
      break;
    }

    if (!task) {
      continue;
    }

    task();
  } while (true);
}

void
jade::TaskQueue::interrupt()
{
  std::lock_guard lock(mutex);
  interrupted = true;
  cv.notify_all();
}
