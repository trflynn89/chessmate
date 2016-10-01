#pragma once

#include <QMainWindow>

#include <fly/fly.h>

namespace Ui {

DEFINE_CLASS_PTRS(ChessMateGui);

}

namespace chessmate {

DEFINE_CLASS_PTRS(ChessMate);
DEFINE_CLASS_PTRS(ChessMateGui);
DEFINE_CLASS_PTRS(ChessMateTile);

class ChessMateGui : public QMainWindow
{
    Q_OBJECT;

public:
    ChessMateGui();
    virtual ~ChessMateGui();

    bool Initialize();
    bool MakeMove(ChessMateTile *, ChessMateTile *);

private:
    Ui::ChessMateGuiUPtr m_upChessMateGui;
    ChessMatePtr m_spChessMate;
};

}
