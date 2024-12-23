#include "args-processor.hpp"

namespace jade {

  void ArgsProcessor::process_args(int argc, char* argv[])
  {
    args.reserve(argc);

    for (int i = 1; i < argc; ++i) {
      args.emplace_back(argv[i]);
      args_map.insert({ argv[i], i - 1 });
    }
  }

  auto ArgsProcessor::get_value(StringView arg) const -> StringView
  {
    if (const auto it = args_map.find(arg); it != args_map.end()) {
      if (it->second + 1 < args.size()) {
        return args[it->second + 1];
      }
    }

    return "";
  }

  auto ArgsProcessor::is_set(StringView arg) const -> bool
  {
    return args_map.contains(arg);
  }

} // namespace jade
