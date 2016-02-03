# Define system dependent variables. Supports MinGW and GCC.

SYSTEM = $(shell gcc -dumpmachine)

# Determine default architecture
supported_archs := x86 x64

ifneq ($(findstring x86_64, $(SYSTEM)),)
    arch := x64
else ifeq ($(arch),x64)
    $(error Cannot build 64-bit architecture on 32-bit machine)
else
    arch := x86
endif

ifneq ($(arch), $(filter $(supported_archs), $(arch)))
    $(error Architecture $(arch) not supported, check System.mk)
endif

# Determine number of cores
NUM_CORES := $(shell grep -c ^processor /proc/cpuinfo)
