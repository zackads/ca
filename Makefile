# Thin wrapper so `make <target>` works from the project root.
# The real build lives in CMakeLists.txt; this just forwards to the CMake build dir.

BUILD_DIR ?= build
TARGETS := format format-check lint

# Prefer Homebrew's clang: Apple clang 17's ASan runtime hangs at startup on macOS 26.
LLVM_CLANG := /opt/homebrew/opt/llvm/bin/clang
CMAKE_COMPILER_ARG := $(if $(wildcard $(LLVM_CLANG)),-DCMAKE_C_COMPILER=$(LLVM_CLANG))

.PHONY: all clean $(TARGETS)

all: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR)

$(TARGETS): $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --target $@

$(BUILD_DIR)/Makefile:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMPILER_ARG)

clean:
	rm -rf $(BUILD_DIR)
