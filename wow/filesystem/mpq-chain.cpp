#include "mpq-chain.hpp"

#include <glob/glob.h>
#include <spdlog/spdlog.h>

namespace wow {

  MPQChain::MPQChain(const std::filesystem::path& data_root)
  {
    namespace fs = std::filesystem;

    static std::vector<std::string> patterns = { "common.MPQ", "common-2.MPQ", "expansion.MPQ", "lichking.MPQ",
      "*/locale-*.MPQ", "*/speech-*.MPQ", "*/expansion-locale-*.MPQ", "*/lichking-locale-*.MPQ",
      "*/expansion-speech-*.MPQ", "*/lichking-speech-*.MPQ", "*/patch-????.MPQ", "*/patch-*.MPQ", "patch.MPQ",
      "patch-*.MPQ" };

    std::vector<std::string> full_patterns;
    for (const auto& path : patterns) {
      auto full_path = data_root / path;
      full_patterns.push_back(full_path.string());
    }

    for (const auto& path : glob::glob(full_patterns)) {
      spdlog::info("Found '{}'", path.filename().string());

      if (archive.is_valid()) {
        if (!archive.patch(path)) {
          spdlog::error("Error patching: {}", path.filename().string());
        }
      } else {
        archive = MPQArchive(path);
      }
    }

    spdlog::info("MPQ chain loading done!");
  }

} // namespace wow
