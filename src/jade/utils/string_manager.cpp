#include "string_manager.hpp"

#include "assert.hpp"

std::shared_mutex jade::StringManager::mutex;
std::unordered_map<jade::StringId, jade::String> jade::StringManager::id_to_string;
std::unordered_map<jade::StringView, jade::StringId> jade::StringManager::string_to_id;
std::size_t jade::StringManager::string_counter = 0;

auto
jade::StringManager::get_string_by_id(jade::StringId id) -> jade::StringView
{
   std::shared_lock<std::shared_mutex> lock(mutex);
   auto it = id_to_string.find(id);
   if (it != id_to_string.end()) {
      return it->second;
   }

   return "";
}

auto
jade::StringManager::get_id_by_string(jade::StringView string) -> jade::StringId
{
   {
      std::shared_lock<std::shared_mutex> lock(mutex);
      auto it = string_to_id.find(string);
      if (it != string_to_id.end()) {
         return it->second;
      }
   }

   StringId id = StringId{};

   {
      std::unique_lock<std::shared_mutex> lock(mutex);
      JADE_ASSERT(string_counter < std::numeric_limits<std::size_t>::max());
      id.id = string_counter++;
      string_to_id.insert(std::make_pair(string, id));
      id_to_string.insert(std::make_pair(id, string));
   }

   return id;
}

jade::StringId::StringId()
  : id(0)
{
}

jade::StringId::StringId(jade::StringView string)
  : id(StringManager::get_id_by_string(string).id)
{
}

auto
jade::StringId::data() const -> jade::StringView
{
   return StringManager::get_string_by_id(*this);
}
