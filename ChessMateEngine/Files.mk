# Define the directories to include and compile
SRC_DIRS_$(d) := \
    Engine \
    Game \
    Movement \
    Util/Logging \
    Util/Socket \
    Util/String \
    Util/System

# Define libraries to link
LDLIBS_$(d) :=

ifeq ($(IS_WINDOWS),1)
    LDLIBS_$(d) += -lws2_32
else
    LDLIBS_$(d) += -lpthread
endif

# Define source files
$(eval $(call WILDCARD_SOURCES))
