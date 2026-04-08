{
  description = "RISC-V OS Development Environment";

  inputs = { nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable"; };

  outputs = { nixpkgs, ... }:
    let
      forAllSystems = function:
        nixpkgs.lib.genAttrs [ "x86_64-linux" "aarch64-linux" ] (system:
          let
            pkgs = nixpkgs.legacyPackages.${system};
            # Cross-toolchain for riscv64 bare-metal
            riscvPkgs = import nixpkgs {
              inherit system;
              crossSystem.config = "riscv64-none-elf";
            };
          in function pkgs riscvPkgs);
    in {
      devShells = forAllSystems (pkgs: riscvPkgs: {
        default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            # Host Formatting/Tools
            alejandra
            clang-tools
            cmake-language-server

            # LLVM/Clang Toolchain
            llvmPackages.clang-unwrapped
            llvmPackages.lld

            # RISC-V Specific Utilities (objcopy, nm, etc)
            riscvPkgs.buildPackages.binutils

            # Build system
            just
            cmake
            ninja

            # Emulation and Debugging
            qemu
            gdb
            gef
          ];

          shellHook = ''
            export CC="clang"
            export LD="ld.lld"
          '';
        };
      });
    };
}
