# Define the directories to include and compile
SRC_DIRS_$(d) := \
    engine \
    game \
    movement

# Define libraries to link
LDLIBS_$(d) := -lfly -lpthread

# Define QT5 UIC/MOC/RCC source files
QT5_UIC_$(d) := chessmate_gui
QT5_MOC_$(d) := chessmate_gui chessmate_tile
QT5_RCC_$(d) := chessmate_gui

# Define source files
$(eval $(call WILDCARD_SOURCES))
SRC_$(d) += $(d)/../chessmate.cpp
