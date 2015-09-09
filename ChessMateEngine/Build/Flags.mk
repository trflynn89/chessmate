# Define build flags for C/C++ files

# Remove built-in rules
MAKEFLAGS += --no-builtin-rules --no-print-directory
.SUFFIXES:

# Flags for both C and C++ files
CF_ALL := -MMD -MP
CF_ALL += -Wall -Wextra
CF_ALL += -I$(SOURCE_ROOT) $(SYSTEM_FLAGS)

# Optimize release builds and add GDB symbols to debug builds
ifeq ($(release), 1)
	CF_ALL += -O2
else
	CF_ALL += -g
endif

ifeq ($(arch), 32)
	CF_ALL += -m32
endif

# C and C++ specific flags
CFLAGS := -std=c11 $(CF_ALL)
CXXFLAGS := -std=c++11 $(CF_ALL)

# Linker flags
LDFLAGS :=
LDLIBS :=
