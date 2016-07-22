# Define the directories to include and compile
SRC_DIRS_$(d) := \
    Engine \
    Game \
    Movement \
    Util/Config \
    Util/File \
    Util/Logging \
    Util/Socket \
    Util/String \
    Util/System \
    Util/Task

# Define libraries to link
LDLIBS_$(d) := -lpthread

# Define source files
$(eval $(call WILDCARD_SOURCES))
