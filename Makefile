# Define the path to the source directory.
SOURCE_ROOT := $(CURDIR)

# Define the application version.
VERSION = $(shell cat $(SOURCE_ROOT)/VERSION.md)

# Import the build API.
include $(SOURCE_ROOT)/extern/flymake/src/api.mk

CFLAGS := -I$(SOURCE_ROOT)/extern/libfly
CXXFLAGS := -I$(SOURCE_ROOT)/extern/libfly

# Dependencies.
$(eval $(call ADD_TARGET, libfly, extern/libfly/fly, LIB))

# Main targets.
$(eval $(call ADD_TARGET, chessmate, ChessMateEngine, BIN, libfly))
$(eval $(call ADD_TARGET, ChessMate, ChessMateGUI/src/main/java, JAR))

# Override default flymake configuration.
output ?= $(SOURCE_ROOT)/build

# Import the build system.
include $(SOURCE_ROOT)/extern/flymake/src/build.mk
