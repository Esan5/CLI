#include "cli.h"

#include <optional>
#include <print>
#include <vector>

void vecvecvec(std::vector<std::vector<std::vector<int>>> const &in) {
    std::println("{}", in);
}

void optoptopt(std::optional<std::optional<std::optional<int>>> const &in) {
    if (in.has_value()) {
        if ((*in).has_value()) {
            if ((**in).has_value()) {
                std::println("Value(Value(Value({})))", ***in);
            } else {
                std::println("Value(Value(Nullopt))");
            }
        } else {
            std::println("Value(Nullopt)");
        }
    } else {
        std::println("Nullopt");
    }
}

int main(int argc, char *argv[]) {
    auto program = cli::program<vecvecvec, optoptopt>();
    program(argc, argv);
}
