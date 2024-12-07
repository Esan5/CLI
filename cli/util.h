#pragma once

#include <optional>
#include <string_view>

namespace cli::util {

auto is_flag(std::string_view const sv) -> bool {
    return sv.size() > 2 && sv.substr(0, 2) == "--";
}

} // namespace cli::util
