# Define the directories to include and compile
SRC_DIRS_$(d) := \
    Engine \
    Game \
    Movement

# Define libraries to link
LDLIBS_$(d) := -lfly -lpthread

# Define source files
$(eval $(call WILDCARD_SOURCES))
