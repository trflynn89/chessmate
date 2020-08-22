SRC_DIRS_$(d) := \
    ChessMateEngine/engine \
    ChessMateEngine/game \
    ChessMateEngine/movement

SRC_$(d) := \
    $(d)/chessmate.cpp

CFLAGS_$(d) += -I$(d)
CXXFLAGS_$(d) += -I$(d)

LDLIBS_$(d) := \
    -lfly
