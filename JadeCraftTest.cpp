#include <catch2/catch_test_macros.hpp>

#include <jade/threads/task_queue.hpp>
#include <jade/utils/string_manager.hpp>

#include <vector>

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
