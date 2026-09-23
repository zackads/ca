# Thin wrapper so `make <target>` works from the project root.
# The real build lives in CMakeLists.txt; this just forwards to the CMake build dir.

BUILD_DIR ?= build
TARGETS := format format-check lint

.PHONY: all clean $(TARGETS)

all: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR)

$(TARGETS): $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --target $@

$(BUILD_DIR)/Makefile:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

clean:
	rm -rf $(BUILD_DIR)
