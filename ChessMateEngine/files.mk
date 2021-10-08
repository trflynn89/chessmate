SRC_DIRS_$(d) := \
    $(d)/engine \
    $(d)/game \
    $(d)/movement

SRC_$(d) := \
    $(d)/chessmate.cpp

CXXFLAGS_$(d) += -I$(SOURCE_ROOT)/ChessMateEngine
