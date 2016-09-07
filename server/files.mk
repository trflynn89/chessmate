# Define the directories to include and compile
SRC_DIRS_$(d) := \
    engine \
    game \
    movement

# Define libraries to link
LDLIBS_$(d) := -lfly -lpthread

# Define source files
$(eval $(call WILDCARD_SOURCES))
SRC_$(d) += $(d)/../chessmate.cpp
