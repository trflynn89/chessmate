# Define the directories to include and compile
SRC_DIRS_$(d) := \
    ChessMateEngine/engine \
    ChessMateEngine/game \
    ChessMateEngine/movement

# Define compiler flags
CFLAGS_$(d) += -I$(d)
CXXFLAGS_$(d) += -I$(d)

# Define libraries to link
LDLIBS_$(d) := -lfly -lpthread

# Define source files
SRC_$(d) := \
    $(d)/chessmate.cpp
