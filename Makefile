# Define the path to the source directory.
SOURCE_ROOT := $(CURDIR)

# Define the application version.
VERSION = $(shell cat $(SOURCE_ROOT)/VERSION.md)

# Import the build API.
include /usr/local/src/fly/api.mk

# Main targets.
$(eval $(call ADD_TARGET, chessmate, ChessMateEngine, BIN))
$(eval $(call ADD_TARGET, ChessMate, ChessMateGUI/src/main/java, JAR))

# Override default flymake configuration.
output ?= $(SOURCE_ROOT)/build

# Import the build system.
include /usr/local/src/fly/build.mk
