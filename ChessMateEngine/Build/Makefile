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
	$(Q)numPass=0; numFail=0; \
	for tgt in $(TEST_TARGETS) ; do \
		printf -- "----------- [Test $$tgt] -----------\n\n"; \
		$(MAKE) -j $(NUM_CORES) target=$$tgt test; \
		if [ $$? -ne 0 ] ; then \
			printf -- "[ERROR $$tgt]\n\n"; \
			numFail=$$((numFail+1)); \
		else \
			printf -- "[SUCCESS $$tgt]\n\n"; \
			numPass=$$((numPass+1)); \
		fi; \
	done; \
	printf -- "----------- [Pass $$numPass, Fail $$numFail] -----------\n\n"; \
	exit $$numFail

# Command to generate GNU Global tags
tags:
	$(Q)cd $(SOURCE_ROOT) && rm -f GPATH GRTAGS GTAGS
	$(Q)cd $(SOURCE_ROOT) && gtags $(GTAGS_FLAGS)
