# Define system dependent variables. Supports MinGW and GCC.

SYSTEM = $(shell gcc -dumpmachine)

IS_WINDOWS :=
IS_LINUX :=
EXTENSION :=

# Decide if Windows or Linux
ifneq ($(findstring mingw, $(SYSTEM)),)
    IS_WINDOWS := 1
    IS_LINUX := 0
    EXTENSION := .exe
    # TODO this assumes Windows 8.1
    SYSTEM_FLAGS := -DWINVER=0x0603 -D_WIN32_WINNT=0x0603
else ifneq ($(findstring linux, $(SYSTEM)),)
    IS_WINDOWS := 0
    IS_LINUX := 1
    EXTENSION :=
    SYSTEM_FLAGS :=
else
    $(error Unrecognized compilation system)
endif

# Determine default architecture
supported_archs := 32 64

ifneq ($(findstring x86_64, $(SYSTEM)),)
    arch := 64
else ifeq ($(arch),64)
    $(error Cannot build 64-bit architecture on 32-bit machine)
else
    arch := 32
endif

ifneq ($(arch), $(filter $(supported_archs), $(arch)))
    $(error Architecture $(arch) not supported, check System.mk)
endif

# Determine number of cores
NUM_CORES := 1

ifeq ($(IS_WINDOWS),1)
    NUM_CORES := $(NUMBER_OF_PROCESSORS)
else ifeq ($(IS_LINUX),1)

endif
