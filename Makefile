
# Define directories to be formatted
SRC_DIR := src
MODULES_DIR := modules

# Find all source files under the directories
SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
MODULES_FILES := $(shell find $(MODULES_DIR) -type f -name "*.cppm")

# Combine all files
ALL_FILES := $(SRC_FILES) $(MODULES_FILES)

# Define the format action
.PHONY: format
format:
	@echo "Formatting all source files..."
	clang-format -i $(ALL_FILES)
	@echo "Formatting complete."

# Add other targets as needed
