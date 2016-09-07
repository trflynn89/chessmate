#pragma once

#include <QMainWindow>

namespace Ui {
    class ChessMateGui;
}

class ChessMateGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChessMateGui(QWidget *parent = 0);
    virtual ~ChessMateGui();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ChessMateGui *ui;
};

