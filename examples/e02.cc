#include "cli.h"

#include <print>

void add(int v1, int v2) {
    std::println("{} + {} = {}", v1, v2, v1 + v2);
}

void sub(int v1, int v2) {
    std::println("{} - {} = {}", v1, v2, v1 - v2);
}

int main(int argc, char *argv[]) {
    auto program = cli::program<sub, add>();
    program(argc, argv);
}
