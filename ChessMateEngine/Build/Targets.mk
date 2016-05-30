# Define all possible targets and their source directories

# Function to define a target
# $(1) = The target's name
# $(2) = The target's source directory
define ADD_TARGET

ifeq ($(target),$$(strip $(1)))
    MAIN_DIR := $$(strip $(2))
endif

endef

# Function to define a test target. All unit test Files.mk must define a rule
# for test, which should depend on the unit test binary and then run the test.
# It is expected that each unit test returns 0 on success.
# $(1) = The target's name
# $(2) = The target's source directory
define ADD_TEST_TARGET

$(call ADD_TARGET, $(1), $(2))
TEST_TARGETS += $$(strip $(1))

endef

# Make sure target is defined
ifeq ($(target),)
    $(error Target must be defined)
endif

### START define all targets

# Main targets
$(eval $(call ADD_TARGET, ChessMateEngine, .))

# Unit tests
$(eval $(call ADD_TEST_TARGET, SocketTest, Test/SocketTest))
$(eval $(call ADD_TEST_TARGET, StringTest, Test/StringTest))
$(eval $(call ADD_TEST_TARGET, MessageTest, Test/MessageTest))
$(eval $(call ADD_TEST_TARGET, ConcurrencyTest, Test/ConcurrencyTest))
$(eval $(call ADD_TEST_TARGET, MovementTest, Test/MovementTest))
$(eval $(call ADD_TEST_TARGET, TraitsTest, Test/TraitsTest))

### END define all targets

# Make sure target was known
ifeq ($(MAIN_DIR),)
    $(error Unknown target $(target), please check Targets.mk)
endif
