# CLI[-inator]

## What is it?

`cli` is a header only library for simple command line programs written in C++. It presents a look at what modern reflection based libraries may look like in C++26. It's designed for ease of use and modeled off of python's `argparse`.

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

```
auto program = cli::program<my_func1, my_func2, ...>();
```

And pass `argc` and `argv` as parameters:

```
program(argc, argv);
```

Using function reflection, it will determine function name and parameters for every function passed as a template argument.

After compiling your program (with `clang-p2996`), you can run you binary from the command line, passing the names of the functions to call different commands.

```
$ build/a.out my_func1 --my_opt opt ...
```

The names of the function parameters will be used as command line options which get parsed and passed to the function as parameters. Currently supported parameter types are string types and numerics as well as optionals and vectors. You can write a custom verifier and parser to enable user defined types to be passed as command line arguments.
