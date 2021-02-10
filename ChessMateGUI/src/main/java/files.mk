SRC_DIRS_$(d) := \
    $(d)/com/flynn/chessmate \
    $(d)/com/flynn/chessmate/communication \
    $(d)/com/flynn/chessmate/game \
    $(d)/com/flynn/chessmate/gui \
    $(d)/com/flynn/chessmate/movement \
    $(d)/com/flynn/chessmate/util

MAIN_CLASS_$(d) := com.flynn.chessmate.ChessMateGUI

CLASS_PATH_$(d) := \
    $(d)/../../../lib/org/freixas/tablelayout/TableLayout-1.0.jar

RESOURCES_$(d) := \
    $(d)/../resources/images
