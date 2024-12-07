#pragma once

#include <string>

namespace cli {

struct program_options_t {
    std::string name;
    std::string usage;
    std::string description;
    std::string epilog;
};

}; // namespace cli
