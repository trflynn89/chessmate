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
LDLIBS_$(d) := -lpthread

# Define source files
$(eval $(call WILDCARD_SOURCES))
