# Define the directories to include and compile
SRC_DIRS_$(d) := \
    Util/Logging \
    Util/String

# Define libraries to link
LDLIBS_$(d) :=

# Define source files
$(eval $(call WILDCARD_SOURCES))
SRC_$(d) += $(d)/../../Game/Message.cpp

# Define rule to build and run the test
test: $(TGT_BIN)
	@$(TGT_BIN)
