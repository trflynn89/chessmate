# Define the default make configuration

# Define the target to build - all targets must be added to Targets.mk
target := ChessMateEngine

# Define debug vs. release
release := 0

# Build 64-bit or 32-bit target
arch := $(arch)

# Enable verbose builds
verbose := 0

# Define the output directories
ifeq ($(release), 1)
	OUT_DIR := $(BUILD_ROOT)/release-$(arch)
else
	OUT_DIR := $(BUILD_ROOT)/debug-$(arch)
endif

OBJ_DIR := $(OUT_DIR)/obj
BIN_DIR := $(OUT_DIR)/bin
ETC_DIR := $(OUT_DIR)/etc

# Use @ suppression in non-verbose builds
ifeq ($(verbose),0)
    Q := @
endif
