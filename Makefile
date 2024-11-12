.PHONY: all clean install format help

BUILD_DIR = build
CMAKE = cmake
MAKE = make

all: $(BUILD_DIR)
	@printf "Building project...\n"
	@cd $(BUILD_DIR) && $(CMAKE) .. && $(MAKE)

$(BUILD_DIR):
	@printf "Creating build directory...\n"
	@mkdir -p $(BUILD_DIR)

clean:
	@printf "Cleaning build directory...\n"
	@rm -rf $(BUILD_DIR)

install: all
	@printf "Installing...\n"
	@cd $(BUILD_DIR) && sudo $(MAKE) install

format:
	@printf "Formatting code...\n"
	@find src -iname *.hpp -o -iname *.cpp | xargs clang-format -i

help:
	@echo "Available commands:"
	@echo "  make       - Build the project"
	@echo "  make clean - Remove build files"
	@echo "  make install - Install the application"
	@echo "  make format - Format source code"
	@echo "  make help  - Show this help message"

.DEFAULT_GOAL := all