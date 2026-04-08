cmake_minimum_required(VERSION 3.20)
project("Lux" LANGUAGES C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ========= CONFIGURATION =========

set(
    COMPILE_OPTIONS 
    -Wall -Wextra -Werror
    -Wpedantic # Ensure pure ISO C99 compliance
    -ffreestanding
)
set(
    LINK_OPTIONS
    -fuse-ld=lld
    -nostdlib
)

set(TARGET_NAME lux_kernel.elf)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND COMPILE_OPTIONS -g -O0)
    add_compile_definitions(DEBUG_BUILD)
else()
    list(APPEND COMPILE_OPTIONS -O2)
endif()

# === MISC ===

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ========= BUILD =========

# Generic Kernel source files
file(GLOB_RECURSE KERNEL_SOURCES CONFIGURE_DEPENDS "src/*.c")
list(FILTER KERNEL_SOURCES EXCLUDE REGEX "src/arch/.*") # exclude arch sources (will be linked in)

# Add arch specific HAL
add_subdirectory(src/arch)

add_executable(${TARGET_NAME} ${KERNEL_SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE arch_backend)

target_compile_options(${TARGET_NAME} PRIVATE ${COMPILE_OPTIONS})
target_link_options(${TARGET_NAME} PRIVATE "-T${ARCH_LINKER_SCRIPT}" ${LINK_OPTIONS})
target_include_directories(${TARGET_NAME} PRIVATE src src/arch)

# ========= QEMU Runner Config =========

configure_file(
    ".gdbinit.in"
    "${CMAKE_BINARY_DIR}/.gdbinit"
    @ONLY
)

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "riscv64")
    set(QEMU_BIN qemu-system-riscv64)
    set(QEMU_MACHINE -machine virt -cpu rv64 -bios none)
elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    set(QEMU_BIN qemu-system-aarch64)
    set(QEMU_MACHINE -machine virt -cpu cortex-a57)
endif()

add_custom_target(run
    COMMAND ${QEMU_BIN} ${QEMU_MACHINE}
            -display none
            -serial stdio
            -monitor none
            -kernel ${TARGET_NAME}
    DEPENDS ${TARGET_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    USES_TERMINAL
)

add_custom_target(debug
    COMMAND ${QEMU_BIN} ${QEMU_MACHINE}
            -display none
            -serial stdio
            -monitor none
            -s -S
            -kernel ${TARGET_NAME}
    DEPENDS ${TARGET_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    USES_TERMINAL
)
