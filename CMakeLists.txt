cmake_minimum_required(VERSION 4.2)
project(code C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_FORMAT clang-format)
find_program(CLANG_TIDY clang-tidy HINTS /opt/homebrew/opt/llvm/bin)

option(ENABLE_CLANG_TIDY "Run clang-tidy during build" OFF)
if (ENABLE_CLANG_TIDY)
    if (NOT CLANG_TIDY)
        message(FATAL_ERROR "ENABLE_CLANG_TIDY is ON but clang-tidy was not found")
    endif ()
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY})
endif ()

# Convenience targets: `make format`, `make format-check`, `make lint`.
file(GLOB C_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/*.c)
file(GLOB H_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/*.h)

if (CLANG_FORMAT)
    add_custom_target(format
            COMMAND ${CLANG_FORMAT} -i ${C_SOURCES} ${H_SOURCES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting sources")
    add_custom_target(format-check
            COMMAND ${CLANG_FORMAT} --dry-run --Werror ${C_SOURCES} ${H_SOURCES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Checking formatting")
endif ()

if (CLANG_TIDY)
    # Homebrew's clang-tidy can't find the macOS SDK headers on its own.
    set(TIDY_EXTRA_ARGS)
    if (APPLE)
        execute_process(COMMAND xcrun --show-sdk-path
                OUTPUT_VARIABLE SDK_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(TIDY_EXTRA_ARGS --extra-arg=-isysroot${SDK_PATH})
    endif ()
    add_custom_target(lint
            COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${TIDY_EXTRA_ARGS} ${C_SOURCES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy")
endif ()

include_directories(.)

# Strict warnings everywhere; AddressSanitizer + UBSan in Debug builds.
function(configure_target t)
    target_compile_options(${t} PRIVATE
            -Wall -Wextra -Wpedantic -Wshadow -Wconversion
            -Wstrict-prototypes -Wmissing-prototypes
            $<$<CONFIG:Debug>:-fsanitize=address,undefined -fno-omit-frame-pointer>)
    target_link_options(${t} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endfunction()

add_executable(code
        ca.c
        ca.h
        main.c)
configure_target(code)