#include "chessmate_gui.h"
#include "chessmate_gui.uic.h"

#include <fly/logging/logger.h>

#include <chessmate.h>

namespace chessmate {

//==============================================================================
ChessMateGui::ChessMateGui() :
    QMainWindow(),
    m_upChessMateGui(std::make_unique<Ui::ChessMateGui>()),
    m_spChessMate(std::make_shared<ChessMate>(false))
{
}

//==============================================================================
ChessMateGui::~ChessMateGui()
{
    m_spChessMate->Stop();
}

//==============================================================================
bool ChessMateGui::Initialize()
{
    if (m_spChessMate->Start())
    {
        m_upChessMateGui->setupUi(this);
        return true;
    }

    return false;
}

//==============================================================================
bool ChessMateGui::MakeMove(ChessMateTile *pStart, ChessMateTile *pEnd)
{
    //ChessMateTile *pLabel = findChild<ChessMateTile *>(QStringLiteral("A1"));
    LOGC("%s %s", pStart->objectName().toStdString(), pEnd->objectName().toStdString());

    return true;
}

}
