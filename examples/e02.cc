#include "cli.h"

#include <print>
#include <ranges>

void echo(int const &one) {
    std::println("{}", one);
}

void add(long long v1, long long v2) {
    std::println("{} + {} = {}", v1, v2, v1 + v2);
}

void min_of(std::vector<float> const &fs) {
    std::println("min={}", std::ranges::min(fs));
}

int main(int argc, char* argv[]) {
    auto program = cli::program<echo, add, min_of>();
    program(argc, argv);
}
