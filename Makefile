# This project depends on FTXUI, which is a CMake library pulled in via
# FetchContent. Building it by hand is impractical, so the Makefile simply
# delegates to CMake. The first build downloads and compiles FTXUI.

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/dashboard

.PHONY: all release run clean

# Default: fast, unoptimized build for quick iteration. The empty build type is
# set explicitly so `make` reverts cleanly after a `make release`.
all:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=
	cmake --build $(BUILD_DIR)

# Optimized, size-minimized build (LTO + dead-strip). Slower to build.
release:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
