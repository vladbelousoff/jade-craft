#pragma once

#include <StormLib.h>

#include <filesystem>

#include <jade/strings/string-manager.hpp>

namespace wow {

  class MPQArchive
  {
  public:
    explicit MPQArchive() = default;
    explicit MPQArchive(const std::filesystem::path& path);

    auto is_valid() const -> bool
    {
      return handle != HANDLE{};
    }

    auto get_handle() const -> HANDLE
    {
      return handle;
    }

    auto patch(const std::filesystem::path& path, jade::StringView prefix = "") const -> bool;

  private:
    HANDLE handle = {};
  };

} // namespace wow
