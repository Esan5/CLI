{
  inputs = {
    # Nixpkgs / NixOS version to use.
    nixpkgs.url = "nixpkgs/nixos-24.11";

    flake-utils.url = "github:numtide/flake-utils";

    clang-p2996 = {
      url = "github:bloomberg/clang-p2996/p2996";
      flake = false;
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      clang-p2996,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        formatter = pkgs.nixfmt-rfc-style;

        packages = {
          default = pkgs.stdenv.mkDerivation rec {
            name = "CLI";
            src = ./.;
            installPhase = ''cp -r $src $out '';
          };

          clang-p2996 = pkgs.stdenv.mkDerivation rec {
            name = "clang-p2996";
            src = clang-p2996;

            nativeBuildInputs = [
              pkgs.cmake
              pkgs.python3
              pkgs.ninja
              pkgs.git
            ];

            cmakeFlags = [
              "-DCMAKE_BUILD_TYPE=Release"
              "-DLLVM_ENABLE_PROJECTS=clang"
              "-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi;libunwind"
            ];

            configurePhase = ''
              cmake $cmakeFlags -DCMAKE_INSTALL_PREFIX=$out -G Ninja -S $src/llvm -B build
            '';

            buildPhase = ''
              cmake --build build
            '';

            installPhase = ''
              cmake --build build --target install install-cxx install-cxxabi
            '';
          };
        };

        devShells = {
          default = pkgs.mkShell {
            buildInputs = [
              self.packages.${system}.clang-p2996
              pkgs.git
            ];

            shellHook = ''
              export PATH=${self.packages.${system}.clang-p2996}/bin:$PATH
              export LD_OIBRARY_PATH=${self.packages.${system}.clang-p2996}/include:$PATH
              export LDFLAGS="-L${self.packages.${system}.clang-p2996}/lib -rpath ${
                self.packages.${system}.clang-p2996
              }/lib"
            '';
          };
        };
      }
    );
}
