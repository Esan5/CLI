# CLI[-inator]

## What is it?

`cli` is an **experimental** header only library for simple command line programs written in C++. It presents a look at what modern reflection based libraries may look like in C++26. It's designed for ease of use and modeled off of python's `argparse`.

Simply pass the functions you want to expose on the command line and get a command line interface for each:


```cc
// main.cc

void add(int a, int b) {
    std::println("{} + {} = {}", a, b, a + b);
}

void sub(int a, int b) {
    std::println("{} - {} = {}", a, b, a - b);
}

...

int main(int argc, char *argv[]) {
    auto program = cli::program<add, sub>(); // pass functions as template parameters
    program(argc, argv);
}
```

CLI will automatically generate help messages:

```bash
$ a.out --help

usage: a.out [-h, --help] {sub,add} ...


commands:
    {sub,add}

options:
    -h, --help show this message and exit

$ a.out add --help

usage: a.out add [-h, --help] --v1 v1 --v2 v2

options:
    -h, --help  show this message and exit
    --v1        int
    --v2        int
```

And run the correct function:

```bash
$ a.out add --v1 1 --v2 2
1 + 2 = 3
$ a.out sub --v1 1 --v2 2
1 - 2 = -1
```

## Quickstart

### Building clang-p2996

If you have nix installed, there is an included `flake.nix` with a nix-shell for the `clang-p2996` implementation. Just run `nix develop` from inside the repo and wait for `llvm` to build (takes forever).

If you don't have nix installed, install the [clang-p2996](https://github.com/bloomberg/clang-p2996/blob/p2996/P2996.md). Follow the steps to build and install clang with the following commands:

```
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi;libunwind -S llvm -B build
cmake --build build
cmake --build build --target install install-cxx install-cxxabi
```

Make sure to check the `flake.lock` file to see the git revision of the clang fork the current examples work with.

### Running the examples

The `CMakeLists.txt` is set up to build the examples.

```
cmake -S . -B build
cmake --build build
build/e01 --help
```

## Usage

To test the library with you own code just clone the repo somewhere on you inlude path and include `cli.h`. In you main function, initialize a program with:

```cc
auto program = cli::program<my_func1, my_func2, ...>();
```

And pass `argc` and `argv` as parameters:

```cc
program(argc, argv);
```

Using function reflection, it will determine function name and parameters for every function passed as a template argument.

After compiling your program (with `clang-p2996`), you can run you binary from the command line, passing the names of the functions to call different commands.

```
$ build/a.out my_func1 --my_opt opt ...
```

The names of the function parameters will be used as command line options which get parsed and passed to the function as parameters. Currently supported parameter types are string types and numerics as well as optionals and vectors. You can write a custom verifier and parser to enable user defined types to be passed as command line arguments.

See `examples/` for more details.
