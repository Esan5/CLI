#pragma once

#include <cstdlib>
#include <optional>
#include <print>
#include <span>
#include <string_view>
#include <tuple>

#include <experimental/meta>

#include "p2996/expand.h"
#include "p2996/struct_to_tuple.h"
#include "parsers/parse.h"

namespace cli {

template <auto F>
class command {
  public:
    auto operator()(std::span<std::string_view> const args) -> void {
        if (args[1] != std::meta::identifier_of(std::meta::reflect_function(*F))) {
            return;
        }

        if (args.size() > 2 && (args[2] == "--help" || args[2] == "-h")) {
            help(args);
            return;
        }

        struct storage;

        consteval {
            std::vector<std::meta::info> members;
            for (auto e : std::meta::parameters_of(std::meta::reflect_function(*F))) {
                members.emplace_back(
                    std::meta::data_member_spec(std::meta::remove_cvref(std::meta::type_of(e)), {.name = std::meta::identifier_of(e)}));
            }
            std::meta::define_aggregate(^^storage, members);
        }

        storage s;
        std::vector<std::string> errors;
        [: ::cli::p2996::expand(std::meta::nonstatic_data_members_of(^^storage)) :] >> [&]<auto e> {
            auto name = std::meta::identifier_of(e);
            auto flag = std::format("--{}", name);
            auto iter = std::ranges::find(args, flag);
            auto verify = ::cli::verify<[: std::meta::remove_cvref(std::meta::type_of(e)) :]>{}(iter, args.end());
            if (verify.has_value()) {
                errors.emplace_back(
                        std::format("error {} {}: argument {}: {}", args[0], std::meta::identifier_of(std::meta::reflect_function(*F)), flag, *verify));
                return;
            }
            s.[: e :] = ::cli::parse<[: std::meta::remove_cvref(std::meta::type_of(e)) :]>{}(iter, args.end());
        };

        if (!errors.empty()) {
            for (auto const &error : errors) {
                std::println("{}", error);
            }
            return;
        }

        std::apply(*F, ::cli::p2996::struct_to_tuple(s));
    }

    auto help(std::span<std::string_view> const args) -> void {
        auto usage = std::format("{} {} [-h, --help]", args[0], args[1]);

        std::vector<std::tuple<std::string, std::string>> options = {{"-h, --help", "show this message and exit"}};
        [: ::cli::p2996::expand(std::meta::parameters_of(std::meta::reflect_function(*F))) :] >> [&]<auto e> {
            if constexpr (std::meta::has_template_arguments(std::meta::remove_cvref(std::meta::type_of(e)))) {
                if constexpr (std::meta::template_of(std::meta::remove_cvref(std::meta::type_of(e))) == ^^std::optional) {
                    usage = std::format("{0} [--{1} [{1}]]", usage, std::meta::identifier_of(e));
                    options.emplace_back(
                            std::format("--{}", std::meta::identifier_of(e)), std::meta::display_string_of(std::meta::remove_cvref(std::meta::type_of(e))));
                    return;
                } else if constexpr (std::meta::template_of(std::meta::remove_cvref(std::meta::type_of(e))) == ^^std::vector) {
                    usage = std::format("{0} --{1} {1} ...", usage, std::meta::identifier_of(e));
                    options.emplace_back(
                            std::format("--{}", std::meta::identifier_of(e)), std::meta::display_string_of(std::meta::remove_cvref(std::meta::type_of(e))));
                    return;
                }
            }
            usage = std::format("{0} --{1} {1}", usage, std::meta::identifier_of(e));
            options.emplace_back(
                    std::format("--{}", std::meta::identifier_of(e)), std::meta::display_string_of(std::meta::remove_cvref(std::meta::type_of(e))));
        };

        std::size_t max_width = 0;
        for (auto const &[option, _] : options) {
            max_width = std::max(max_width, option.size());
        }

        std::string option_table = "";
        std::string fmt = std::format("{{}}    {{:<{}}}  {{}}\n", max_width);
        for (auto const &[option, type] : options) {
            option_table = std::format(std::runtime_format(fmt), option_table, option, type);
        }

        std::println(R"-(
usage: {}

options:
{}
        )-", usage, option_table);
    }

  private:
    static constexpr auto info = std::meta::reflect_function(*F);
};

struct program_options_t {
    std::string name;
    std::string usage;
    std::string description;
    std::string epilog;
};

template <auto... Fs>
class program {
  public:
    explicit program(::cli::program_options_t opts = {}) : opts(opts) {}

    auto operator()(int argc, char *argv[]) -> void {
        auto args = std::vector<std::string_view>(argv, argv + argc);
        return operator()(args);
    }

    auto operator()(std::span<std::string_view> const args) -> void {
        if (args.size() == 1 || args[1] == "--help" || args[1] == "-h") {
            help(args);
            return;
        }

        std::array cs = {std::meta::identifier_of(std::meta::reflect_function(*Fs))...};
        if (std::ranges::find(cs, args[1]) == cs.end()) {
            std::println("error {0} {1}: unrecognized command {1}", args[0], args[1]);
            return;
        }

        (::cli::command<Fs>{}(args), ...);
    }

    auto help(std::span<std::string_view> const args) {
        std::array cs = {std::meta::identifier_of(std::meta::reflect_function(*Fs))...};
        std::string command_set = std::format("{{{}", cs[0]);
        for (std::size_t i = 1; i < cs.size(); i += 1) {
            command_set = std::format("{},{}", command_set, cs[i]);
        }
        command_set = std::format("{}}}", command_set);

        if (opts.name == "") {
            opts.name = args[0];
        }

        if (opts.usage == "") {
            opts.usage = std::format("{} [-h, --help] {} ...", opts.name, command_set);
        }
        opts.usage = std::format("usage: {}", opts.usage);

        if (opts.description != "") {
            opts.description = std::format("{}\n", opts.description);
        }

        if (opts.epilog != "") {
            opts.epilog = std::format("{}\n", opts.epilog);
        }

        std::println(R"-(
{}

{}
commands:
    {}

options:
    -h, --help show this message and exit
{}
        )-", opts.usage, opts.description, command_set, opts.epilog);
    }

  private:
    ::cli::program_options_t opts;

    static_assert(sizeof...(Fs) != 0, "program initialized with no commands, did you forget to add any?");

    static_assert([] {
        std::array cs = {std::meta::identifier_of(std::meta::reflect_function(*Fs))...};
        for (std::size_t i = 0; i < cs.size(); i += 1) {
            for (std::size_t j = i + 1; j < cs.size(); j += 1) {
                if (cs[i] == cs[j]) {
                    return false;
                }
            }
        }
        return true;
    }(), "trying to create a program with overlapping commands, did you add the same one twice?");
};

} // namespace cli
