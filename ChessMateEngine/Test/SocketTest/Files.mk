# Define the directories to include and compile
SRC_DIRS_$(d) := \
    Util/Logging \
    Util/Socket \
    Util/String

# Define libraries to link
LDLIBS_$(d) :=

ifeq ($(IS_WINDOWS),1)

LDLIBS_$(d) += -lws2_32

else

LDLIBS_$(d) += -lpthread

endif

# Define source files
$(eval $(call WILDCARD_SOURCES))

# Define rule to build and run the test
test: $(TGT_BIN)
	@$(TGT_BIN)
