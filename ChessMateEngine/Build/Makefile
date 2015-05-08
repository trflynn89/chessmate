# Define the path to the source directory
SOURCE_ROOT := ..

# Define the path to the build directory
BUILD_ROOT := $(SOURCE_ROOT)/Build

.PHONY: all
.PHONY: targets
.PHONY: clean

# System architecture
include $(BUILD_ROOT)/System.mk

# Build configuration
include $(BUILD_ROOT)/Config.mk

# Define all targets
include $(BUILD_ROOT)/Targets.mk

# Build flags
include $(BUILD_ROOT)/Flags.mk

# Define stack functions
include $(BUILD_ROOT)/Stack.mk

# Define output files
include $(BUILD_ROOT)/Files.mk

# Define rules for compilation
include $(BUILD_ROOT)/Compile.mk

# Define 'all' target before including source directory
all: $(TGT_BIN)

# Include top-level source directory's Rules.mk file
$(eval $(call INCLUDE_BIN_DIR, $(SOURCE_ROOT), $(MAIN_DIR)))

# Clean up output files
clean:
	@echo "[Clean $(OUT_DIR)]"
	$(Q)$(RM) $(CLEAN)

# Run all unit tests defined in Targets.mk
tests:
	$(Q)numPass=0; numFail=0;                                                 \
	for tgt in $(TEST_TARGETS) ; do                                           \
		echo -e "------------- [Test $$tgt] -------------\n";                 \
		$(MAKE) -j $(NUM_CORES) target=$$tgt test;                            \
		if [ $$? -ne 0 ] ; then                                               \
			echo -e "[ERROR $$tgt]\n";                                        \
			numFail=$$((numFail+1));                                          \
		else                                                                  \
			echo -e "[SUCCESS $$tgt]\n";                                      \
			numPass=$$((numPass+1));                                          \
		fi;                                                                   \
	done;                                                                     \
	echo -e "------------- [Pass $$numPass, Fail $$numFail] -------------\n"; \
	exit $$numFail
