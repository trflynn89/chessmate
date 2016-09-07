#include "chessmate_gui.h"
#include "chessmate_gui.uic.h"

ChessMateGui::ChessMateGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChessMateGui)
{
    ui->setupUi(this);
}

ChessMateGui::~ChessMateGui()
{
    delete ui;
}


void ChessMateGui::on_pushButton_clicked()
{

}

