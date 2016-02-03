# Define functions which will declare object, dependency, and executable files

# Target binary
TGT_BIN := $(BIN_DIR)/$(target)$(EXTENSION)

# A list of all generated files to be removed during clean
CLEAN :=

# A list of all object files that have been compiled
OBJS :=

# Set a source directory's source files to all C/C++ files
define WILDCARD_SOURCES

SRC_$(d) := $(wildcard $(d)/*.c) $(wildcard $(d)/*.cc) $(wildcard $(d)/*.cpp)

endef

# Define output files for compiled C/C++ targets
# $(1) = The C/C++ files to be compiled
define OBJ_OUT_FILES

OBJ_DIR_$(d) := $(OBJ_DIR)/$$(subst $(SOURCE_ROOT)/,,$(d))

OBJ_$(d) := $$(addsuffix .o, $$(subst $(d),,$$(basename $$(strip $(1)))))
OBJ_$(d) := $$(addprefix $$(OBJ_DIR_$(d)), $$(OBJ_$(d)))
OBJS += $$(OBJ_$(d))

DEP_$(d) := $$(OBJ_$(d):%.o=%.d)

CLEAN_$(d) := $$(OBJ_$(d)) $$(DEP_$(d))
CLEAN += $$(CLEAN_$(d))

endef

# Define output files for binary targets
# $(1) = The C/C++ files to be compiled
define BIN_OUT_FILES

$(call OBJ_OUT_FILES, $(1))
CLEAN += $(TGT_BIN)

endef
