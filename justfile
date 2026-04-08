configure ARCH="riscv64":
    cmake --preset {{ARCH}}

build:
    cmake --build build

run: build
    cmake --build build --target run

debug: build
    cmake --build build --target debug & \
    sleep 1; \
    gef -x build/.gdbinit; \
    pkill -f qemu-system

# Misc tasks

format:
    find src -iname *.c -o -iname *.h | xargs clang-format -i

clean:
    rm -rf build
