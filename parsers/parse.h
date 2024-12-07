#pragma once

#include <format>
#include <optional>
#include <string_view>

#include "cli/util.h"
#include "parsers/numeric.h"

namespace cli {

template <typename T>
struct verify {
    auto operator()(auto iter, auto end) -> std::optional<std::string> {
        if (iter == end) {
            return "argument is required";
        } else if (iter + 1 == end || ::cli::util::is_flag(*(iter + 1))) {
            return "expected one argument";
        }
        return std::nullopt;
    }
};

template <typename T>
struct parse {
    auto operator()(auto iter, [[maybe_unused]] auto end) -> T {
        return T(*(iter + 1));
    }
};

template <typename T>
struct parse<std::optional<T>> {
    auto operator()(auto iter, auto end) -> std::optional<T> {
        if (iter == end || ::cli::util::is_flag(*(iter + 1))) {
            return std::nullopt;
        }
        return ::cli::parse<T>{}(iter, end);
    }
};

template <typename T>
struct verify<std::optional<T>> {
    auto operator()([[maybe_unused]] auto iter, [[maybe_unused]] auto end) -> std::optional<std::string> {
        if (iter != end) {
            return verify<T>{}(iter, end);
        }
        return std::nullopt;
    }
};

template <typename T, typename A, typename B>
struct parse<std::vector<std::vector<T, A>, B>> {
    auto operator()(auto iter, auto end) -> std::vector<std::vector<T, A>, B> {
        return {parse<std::vector<T, A>>{}(iter, end)};
    }
};

template <typename T, typename A>
struct parse<std::vector<T, A>> {
    auto operator()(auto iter, auto end) -> std::vector<T, A> {
        std::vector<T, A> ret;
        while (iter + 1 != end && !::cli::util::is_flag(*(iter + 1))) {
            ret.emplace_back(::cli::parse<T>{}(iter, end));
            iter += 1;
        }
        return ret;
    }
};

template <typename T, typename A>
struct verify<std::vector<T, A>> {
    auto operator()(auto iter, auto end) -> std::optional<std::string> {
        if (iter == end) {
            return "argument is required";
        }
        return std::nullopt;
    }
};

} // namespace cli
