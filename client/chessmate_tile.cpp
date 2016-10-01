#include "chessmate_tile.h"
#include "chessmate_gui.h"

#include <fly/logging/logger.h>

namespace chessmate {

namespace
{
    static const QString s_clickedColor(QStringLiteral("background-color: green;"));
    static const QString s_hoverColor(QStringLiteral("background-color: red;"));
}

//==============================================================================
ChessMateTile *ChessMateTile::s_pClicked = NULL;

//==============================================================================
ChessMateTile::ChessMateTile(QWidget *pParent) :
    QLabel(pParent),
    m_pGui(NULL),
    m_originalStyle(),
    m_preHoverStyle()
{
    QObject *pGui = NULL;

    // The UIC creates a QWidget with the ChessMateGui as its parent, and passes
    // that QWidget as this ChessMateTile's parent
    if ((pParent != NULL) && ((pGui = pParent->parent()) != NULL))
    {
        m_pGui = qobject_cast<ChessMateGui *>(pGui);
    }

    if (m_pGui == NULL)
    {
        LOGE(-1, "Could not find ChessMateGui for ChessMateTile: %d, %d",
            pParent == NULL, pGui == NULL);
    }
}

//==============================================================================
ChessMateTile::~ChessMateTile()
{
}

//==============================================================================
void ChessMateTile::setStyleSheet(const QString &style)
{
    if (m_originalStyle.isEmpty())
    {
        m_originalStyle = style;
    }

    QLabel::setStyleSheet(style);
}

//==============================================================================
void ChessMateTile::mousePressEvent(QMouseEvent *pEvent)
{
    if (s_pClicked == NULL)
    {
        setStyleSheet(s_clickedColor);
        s_pClicked = this;
    }
    else if (s_pClicked == this)
    {
        setStyleSheet(m_originalStyle);
        s_pClicked = NULL;
    }
    else if (m_pGui != NULL)
    {
        const QPixmap *pPixmap = s_pClicked->pixmap();

        if (pPixmap != NULL)
        {
            if (m_pGui->MakeMove(s_pClicked, this))
            {
                setPixmap(*pPixmap);
                s_pClicked->clear();
            }
        }

        s_pClicked->setStyleSheet(s_pClicked->m_originalStyle);
        s_pClicked = NULL;
    }

    QLabel::mousePressEvent(pEvent);
}

//==============================================================================
void ChessMateTile::enterEvent(QEvent *pEvent)
{
    if (s_pClicked != this)
    {
        m_preHoverStyle = styleSheet();
        setStyleSheet(s_hoverColor);
    }

    QLabel::enterEvent(pEvent);
}

//==============================================================================
void ChessMateTile::leaveEvent(QEvent *pEvent)
{
    if (s_pClicked != this)
    {
        setStyleSheet(m_preHoverStyle);
    }

    QLabel::leaveEvent(pEvent);
}

/*
//==============================================================================
ChessMateTile *ChessMateTile::getClicked() const
{
    ChessMateTile *pClicked = NULL;
    QObject *pParent = parent();

    if (pParent != NULL)
    {
        foreach (ChessMateTile *pLabel, pParent->findChildren<ChessMateTile *>())
        {
            if (pLabel == s_pClicked)
            {
                pClicked = pLabel;
                break;
            }
        }
    }

    return pClicked;
}
*/

}
