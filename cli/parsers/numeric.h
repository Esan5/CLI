#pragma once

#include <optional>
#include <string>

#include "cli/util.h"

namespace cli {

template <typename T>
struct parse;

template <>
struct parse<int> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> int {
        return std::stoi(std::string(*(iter + 1)));
    }
};

template <>
struct parse<long> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> long {
        return std::stol(std::string(*(iter + 1)));
    }
};

template <>
struct parse<unsigned long> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> unsigned long {
        return std::stoul(std::string(*(iter + 1)));
    }
};

template <>
struct parse<long long> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> long long {
        return std::stoll(std::string(*(iter + 1)));
    }
};

template <>
struct parse<unsigned long long> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> unsigned long long {
        return std::stoull(std::string(*(iter + 1)));
    }
};

template <>
struct parse<float> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> float {
        return std::stof(std::string(*(iter + 1)));
    }
};

template <>
struct parse<double> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> double {
        return std::stod(std::string(*(iter + 1)));
    }
};

template <>
struct parse<long double> {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> long double {
        return std::stold(std::string(*(iter + 1)));
    }
};

}; // namespace cli
