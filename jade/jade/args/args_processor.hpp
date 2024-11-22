#pragma once

#include "jade/utils/string_manager.hpp"

#include <unordered_map>

namespace jade {

   class ArgsProcessor
   {
    public:
      static ArgsProcessor& get_instance()
      {
         static ArgsProcessor instance;
         return instance;
      }

      void process_args(int argc, char* argv[]);
      auto get_value(StringView arg) const -> StringView;
      auto is_set(StringView arg) const -> bool;

    private:
      ArgsProcessor() = default;

      std::vector<StringView> args{};
      std::pmr::unordered_map<StringView, int> args_map{};
   };

} // namespace jade
