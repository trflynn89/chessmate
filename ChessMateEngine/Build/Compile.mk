# Define compilation functions for each object and binary target. Every source
# directory should contain a Files.mk file. Every binary target directory must
# also contain a Files.mk file, and include the source directorises it depends
# upon. What each of the Files.mk files must define is described below.

# Build tools
COMP_CC := $(Q)$(CC) $$(CFLAGS) -o $$@ -c $$<
LINK_CC := $(Q)$(CC) $(CFLAGS) -o $$@ $$(OBJS) $(LDFLAGS) $$(LDLIBS)

COMP_CXX := $(Q)$(CXX) $$(CXXFLAGS) -o $$@ -c $$<
LINK_CXX := $(Q)$(CXX) $(CXXFLAGS) -o $$@ $$(OBJS) $(LDFLAGS) $$(LDLIBS)

STRIP := $(Q)strip $$@

# Compile C/C++ files to object files
# $(1) = Path to directory where object files should be placed
define OBJ_RULES

# Force recompile if any compile flags change
MAKEFILES_$(d) := $(BUILD_ROOT)/Flags.mk $(d)/*.mk

$$(strip $(1))/%.o: CFLAGS := $(CFLAGS_$(d)) $(CFLAGS)
$$(strip $(1))/%.o: CXXFLAGS := $(CXXFLAGS_$(d)) $(CXXFLAGS)

# C files
$$(strip $(1))/%.o: $(d)/%.c $$(MAKEFILES_$(d))
	@mkdir -p $$(@D)
	@echo "[Compile $$<]"
	$(COMP_CC)

# CC files
$$(strip $(1))/%.o: $(d)/%.cc $$(MAKEFILES_$(d))
	@mkdir -p $$(@D)
	@echo "[Compile $$<]"
	$(COMP_CXX)

# C++ files
$$(strip $(1))/%.o: $(d)/%.cpp $$(MAKEFILES_$(d))
	@mkdir -p $$(@D)
	@echo "[Compile $$<]"
	$(COMP_CXX)

endef

# Compile a set of C/C++ files for a source directory. These rules aren't used
# by the make system, but are defined for convenience. These targets may be
# invoked by calling "make The/Directory/To/Build".
# $(1) = All object files to be built in this source directory
# $(2) = All intermediate files to be cleaned with this directory
define SRC_RULES

$(td): $(1)
	@echo "[Finished $$@]"

$(td)_clean:
	@echo "[Clean $(td)]"
	$(Q)$(RM) $(2)

endef

# Link a binary file from a set of object and library files
# $(1) = All external libraries to link into the binary file
define BIN_RULES

# Force recompile if any compile flags change
MAKEFILES_$(d) := $(BUILD_ROOT)/Flags.mk $(d)/*.mk

$(TGT_BIN): LDLIBS := $(1)

$(TGT_BIN): $$(OBJS) $$(MAKEFILES_$(d))
	@mkdir -p $$(@D)
	@echo "[Link $$@]"
	$(LINK_CXX)

ifeq ($(release),1)
	@echo "[Strip $$@]"
	$(STRIP)
endif

	@echo "[Finished $$@]"

endef

# Define the rules to build a binary target. The Files.mk should define:
# 1. SRC_DIRS_$(d) = The source directories to include in the build
# 2. LDLIBS_$(d) = The libraries to be linked in the target binary
# 3. SRC_$(d) = The sources to be built in the target binary
define DEFINE_BIN_RULES

# Push current dir to stack
$$(eval $$(call PUSH_DIR))

# Define source, object, dependency, and binary files
include $$(d)/Files.mk
$$(eval $$(call BIN_OUT_FILES, $$(SRC_$$(d))))

# Include the source directories
$$(eval $$(call INCLUDE_SRC_DIRS, $$(SRC_DIRS_$$(d))))

# Define the compile rules
$$(eval $$(call OBJ_RULES, $$(OBJ_DIR_$$(d))))
$$(eval $$(call BIN_RULES, $$(LDLIBS_$$(d))))

# Include dependency files
-include $$(DEP_$$(d))

# Pop current dir from stack
$$(eval $$(call POP_DIR))

endef

# Define the rules to build a source directory. The Files.mk should define:
# 1. SRC_$(d) = The C/C++ sources to be built
define DEFINE_SRC_RULES

# Push current dir to stack
$$(eval $$(call PUSH_DIR))

# Define source, object and dependency files
include $$(d)/Files.mk
$$(eval $$(call OBJ_OUT_FILES, $$(SRC_$$(d))))

# Define the compile rules
$$(eval $$(call OBJ_RULES, $$(OBJ_DIR_$$(d))))
$$(eval $$(call SRC_RULES, $$(OBJ_$$(d)), $$(CLEAN_$$(d))))

# Include dependency files
-include $$(DEP_$$(d))

# Pop current dir from stack
$$(eval $$(call POP_DIR))

endef
