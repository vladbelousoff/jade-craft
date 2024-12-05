#include "mpq_archive.hpp"

#include <spdlog/spdlog.h>

namespace wow {

  MPQArchive::MPQArchive(const std::filesystem::path& path)
  {
    if (!SFileOpenArchive(path.string().c_str(), 0, MPQ_OPEN_NO_LISTFILE | MPQ_OPEN_READ_ONLY, &handle)) {
      spdlog::error("Error opening MPQ archive: {}", path.string());
    }
  }

  auto MPQArchive::patch(const std::filesystem::path& path, jade::StringView prefix) const -> bool
  {
    return SFileOpenPatchArchive(handle, path.string().c_str(), prefix.data(), 0);
  }

} // namespace wow
