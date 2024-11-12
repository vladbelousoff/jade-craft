#pragma once

#include <jade/utils/string_manager.hpp>

#include "mpq_archive.hpp"

namespace wow {

   class MPQChain
   {
    public:
      explicit MPQChain() = default;
      explicit MPQChain(const std::filesystem::path& data_root);

    public:
      auto get_archive() const -> MPQArchive
      {
         return archive;
      }

    private:
      MPQArchive archive{};
   };

} // namespace wow
