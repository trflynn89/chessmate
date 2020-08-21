# Define the directories to include and compile
SRC_DIRS_$(d) := \
    ChessMateGUI/src/main/java/com/flynn/chessmate \
    ChessMateGUI/src/main/java/com/flynn/chessmate/communication \
    ChessMateGUI/src/main/java/com/flynn/chessmate/game \
    ChessMateGUI/src/main/java/com/flynn/chessmate/gui \
    ChessMateGUI/src/main/java/com/flynn/chessmate/movement \
    ChessMateGUI/src/main/java/com/flynn/chessmate/util

# Define the application entry point
MAIN_CLASS_$(d) := com.flynn.chessmate.ChessMateGUI

# Define JARs and paths to include on the class path
CLASS_PATH_$(d) := \
    $(d)/../../../lib/org/freixas/tablelayout/TableLayout/1.0/TableLayout-1.0.jar

# Define resources to include in the JAR
RESOURCES_$(d) := \
    $(d)/../resources/images
