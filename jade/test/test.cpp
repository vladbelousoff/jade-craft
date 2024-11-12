#include <catch2/catch_all.hpp>

#include <jade/threads/task_queue.hpp>
#include <jade/utils/string_manager.hpp>

#include <functional>
#include <vector>

TEST_CASE("Empty Function", "[Function]")
{
   std::function<void()> f;
   REQUIRE(!f);
}

TEST_CASE("TaskQueue Works in General", "[TaskQueue]")
{
   jade::TaskQueue task_queue;

   constexpr int n = 1000;
   std::vector<int> numbers;
   numbers.reserve(n);

   for (int i = 0; i < n; ++i) {
      task_queue.add_task([i, &numbers]() {
         numbers.push_back(i);
      });
   }

   task_queue.perform_all_tasks();

   for (int i = 0; i < n; ++i) {
      REQUIRE(numbers[i] == i);
   }
}

TEST_CASE("TaskQueue is Thread-Safe", "[TaskQueue][threaded]")
{
   jade::TaskQueue queue;
   std::atomic<int> counter = 0;
   constexpr int n = 50;

   // Add tasks in multiple threads
   std::thread t1([&queue, &counter] {
      for (int i = 0; i < n; ++i) {
         queue.add_task([&counter] {
            counter++;
         });
      }
   });

   std::thread t2([&queue, &counter] {
      for (int i = 0; i < n; ++i) {
         queue.add_task([&counter] {
            counter++;
         });
      }
   });

   t1.join();
   t2.join();

   // Perform tasks in a single thread
   queue.perform_all_tasks();

   REQUIRE(counter == n * 2);
}
