SRC_DIRS_$(d) := \
    $(d)/engine \
    $(d)/game \
    $(d)/movement

SRC_$(d) := \
    $(d)/chessmate.cpp

CFLAGS_$(d) += -I$(d)
CXXFLAGS_$(d) += -I$(d)

LDLIBS_$(d) := \
    -lfly
