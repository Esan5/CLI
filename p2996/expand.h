#pragma once

#include <experimental/meta>

namespace cli::p2996 {

template <auto... vals>
struct replicator_type {
    template <typename F>
    constexpr void operator>>(F body) const {
        (body.template operator()<vals>(), ...);
    }
};

template <auto... vals>
replicator_type<vals...> replicator = {};

template <typename R>
consteval auto expand(R range) {
    std::vector<std::meta::info> args;
    for (auto r : range) {
        args.push_back(std::meta::reflect_value(r));
    }
    return std::meta::substitute(^^replicator, args);
}

} // namespace cli::p2996
