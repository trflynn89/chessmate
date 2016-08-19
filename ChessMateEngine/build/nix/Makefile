# Define the path to the source directory
SOURCE_ROOT := $(CURDIR)/../..

# Define the application version
VERSION = $(shell cat $(SOURCE_ROOT)/../VERSION.md)

# Import the build API
include /usr/src/fly/api.mk

# Set default target
$(eval $(call SET_DEFAULT_TARGET, chessmate_engine))

# Main targets
$(eval $(call ADD_TARGET, chessmate_engine, ., BIN))

# Import the build system
include /usr/src/fly/build.mk
