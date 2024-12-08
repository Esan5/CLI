#include "cli.h"

#include <print>

void greet_one(std::string const &one) {
    std::println("Hello {}!", one);
}

void greet_many(std::vector<std::string> const &many) {
    std::println("Hello {}!", many);
}

void greet_maybe(std::optional<std::string> const &maybe) {
    if (maybe.has_value()) {
        std::println("Hello {}!", *maybe);
    } else {
        std::println("no one to greet...");
    }
}

int main(int argc, char *argv[]) {
    auto program = cli::program<greet_one, greet_many, greet_maybe>();
    program(argc, argv);
}
