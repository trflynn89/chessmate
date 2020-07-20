# Define the path to the source directory
SOURCE_ROOT := $(CURDIR)/../..

# Define the application version
VERSION = $(shell cat $(SOURCE_ROOT)/../VERSION.md)

# Import the build API
include /usr/src/fly/api.mk

# Main targets
$(eval $(call ADD_TARGET, chessmate, ., BIN))

# Import the build system
include /usr/src/fly/build.mk
