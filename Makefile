BUILD_DIR = build
CMAKE_BUILD_TYPE ?= Debug

.PHONY: all build clean run test fast help

all: help

build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) ..
	cd $(BUILD_DIR) && make -j$(shell nproc)

fast:
	cd $(BUILD_DIR) && make -j$(shell nproc)

run:
	./$(BUILD_DIR)/SwiftMini

test: build
	cd $(BUILD_DIR) && ctest --output-on-failure

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "build   - Build project"
	@echo "run     - Run"
	@echo "clean   - Clean build"
	@echo "test    - Run Test"
	@echo "fast    - Quick rebuild"
