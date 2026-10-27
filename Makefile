# Thin wrapper so `make <target>` works from the project root.
# The real build lives in CMakeLists.txt; this just forwards to the CMake build dir.

BUILD_DIR ?= build
TARGETS := format format-check lint

# Each iteration lives in its own directory and builds to an executable of the same name.
# `make v1` builds v1_naive; `make run-v1 ARGS="90 20"` builds and runs it.
v1 := v1_naive
v2 := v2_binary_tree
VERSIONS := v1 v2

# Prefer Homebrew's clang: Apple clang 17's ASan runtime hangs at startup on macOS 26.
LLVM_CLANG := /opt/homebrew/opt/llvm/bin/clang
CMAKE_COMPILER_ARG := $(if $(wildcard $(LLVM_CLANG)),-DCMAKE_C_COMPILER=$(LLVM_CLANG))

.PHONY: all clean $(TARGETS) $(VERSIONS) $(addprefix run-,$(VERSIONS))

all: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR)

$(TARGETS): $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --target $@

$(VERSIONS): $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --target $($@)

$(addprefix run-,$(VERSIONS)): run-%: %
	./$(BUILD_DIR)/$($*)/$($*) $(ARGS)

$(BUILD_DIR)/Makefile:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMPILER_ARG)

clean:
	rm -rf $(BUILD_DIR)
