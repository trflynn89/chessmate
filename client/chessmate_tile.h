#pragma once

#include <QLabel>

#include <fly/fly.h>

namespace chessmate {

DEFINE_CLASS_PTRS(ChessMateGui);
DEFINE_CLASS_PTRS(ChessMateTile);

class ChessMateTile : public QLabel
{
    Q_OBJECT;

public:
    ChessMateTile(QWidget *);
    virtual ~ChessMateTile();

    void setStyleSheet(const QString &);

protected:
    void mousePressEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    static ChessMateTile *s_pClicked;

    ChessMateGui *m_pGui;

    QString m_originalStyle;
    QString m_preHoverStyle;
};

}
