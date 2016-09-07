#include <memory>

#include <QApplication>

#include "chessmate_gui.h"
#include "tile.h"

#include <fly/config/config_manager.h>
#include <fly/logging/logger.h>
#include <fly/socket/socket_impl.h>
#include <fly/socket/socket_manager_impl.h>
#include <fly/system/system.h>

#include <chessmate.h>
#include <game/chess_game.h>
#include <game/game_config.h>
#include <game/message.h>
#include <movement/move_set.h>

int count=0,turn=1,expp[60],max=0;
int wR,wC,bR,bC;
Tile *click1;

Tile *tile[8][8] = { { NULL } };

class Border
{
public:
    Border();
    void outline(QWidget *baseWidget, int xPos, int yPos, int Pos)
    {
         QLabel *outLabel = new QLabel(baseWidget);

        if(!Pos)
            outLabel->setGeometry(xPos,yPos,552,20);        //Horizontal Borders

        else
            outLabel->setGeometry(xPos,yPos,20,512);        //Vertical Borders

        outLabel->setStyleSheet("QLabel { background-color :rgb(170, 170, 127); color : black; }");
    }
};

void accessories(QWidget *baseWidget)
{
    QLabel *player2 = new QLabel(baseWidget);
    QLabel *name2 = new QLabel("Player 2", baseWidget);
    QLabel *time2 = new QLabel("00:00:00", baseWidget);

    QLabel *player1 = new QLabel(baseWidget);
    QLabel *name1 = new QLabel("Player 1", baseWidget);
    QLabel *time1 = new QLabel("00:00:00", baseWidget);

    QLabel *moves = new QLabel(baseWidget);

    name1->setGeometry(125,610,80,20);
    time1->setGeometry(120,635,80,20);
    player1->setGeometry(100,500,100,100);
    player1->setPixmap(QPixmap(":/images/profile.png"));

    name2->setGeometry(125,210,80,20);
    time2->setGeometry(120,235,80,20);
    player2->setGeometry(100,100,100,100);
    player2->setPixmap(QPixmap(":/images/profile.png"));

    moves->setGeometry(1000,105,250,550);
    moves->setStyleSheet("QLabel {background-color: white;}");

}

void chessBoard(QWidget *baseWidget, Tile *tile[8][8])
{
    int i,j,k=0,hor,ver;
    Border *border[4]={ NULL };

    //borderDisplay
    {
    border[0]->outline(baseWidget,330,105,0);
    border[1]->outline(baseWidget,330,637,0);
    border[2]->outline(baseWidget,330,125,1);
    border[2]->outline(baseWidget,862,125,1);
    }

    //Create 64 tiles (allocating memories to the objects of Tile class)
    ver=125;
    for(i=0;i<8;i++)
    {
        hor=350;
        for(j=0;j<8;j++)
        {
            tile[i][j] = new Tile(baseWidget);
            tile[i][j]->tileColor=(i+j)%2;
            tile[i][j]->piece=0;
            tile[i][j]->row=i;
            tile[i][j]->col=j;
            tile[i][j]->tileNum=k++;
            tile[i][j]->tileDisplay();
            tile[i][j]->setGeometry(hor,ver,64,64);
            hor+=64;
        }
        ver+=64;
    }

    //white pawns
    for(j=0;j<8;j++)
    {
        tile[1][j]->piece=1;
        tile[1][j]->pieceColor=0;
        tile[1][j]->display('P');
    }

    //black pawns
    for(j=0;j<8;j++)
    {
        tile[6][j]->piece=1;
        tile[6][j]->pieceColor=1;
        tile[6][j]->display('P');
    }

    //white and black remaining elements
    for(j=0;j<8;j++)
    {
        tile[0][j]->piece=1;
        tile[0][j]->pieceColor=0;
        tile[7][j]->piece=1;
        tile[7][j]->pieceColor=1;
    }

    {
    tile[0][0]->display('R');
    tile[0][1]->display('H');
    tile[0][2]->display('B');
    tile[0][3]->display('Q');
    tile[0][4]->display('K');
    tile[0][5]->display('B');
    tile[0][6]->display('H');
    tile[0][7]->display('R');
    }


    {
    tile[7][0]->display('R');
    tile[7][1]->display('H');
    tile[7][2]->display('B');
    tile[7][3]->display('Q');
    tile[7][4]->display('K');
    tile[7][5]->display('B');
    tile[7][6]->display('H');
    tile[7][7]->display('R');
    }

    wR=7;
    wC=4;

    bR=0;
    bC=4;

}

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    chessmate::ChessMatePtr spChessMate(
        std::make_shared<chessmate::ChessMate>(false)
    );

    if (!spChessMate->Start())
    {
        LOGC("Could not start ChessMate");
        return -1;
    }

    fly::ConfigManagerPtr spConfigManager = spChessMate->GetConfigManager();
    chessmate::GameConfigPtr spConfig = spConfigManager->CreateConfig<chessmate::GameConfig>();

    fly::SocketManagerPtr spSocketMangaer = spChessMate->GetSocketManager();
    fly::SocketPtr spSocket = spSocketMangaer->CreateTcpSocket();

    if (!spSocket->IsValid())
    {
        LOGC("Could not create client socket");
        return -1;
    }
    else if (!spSocket->Connect("localhost", spConfig->AcceptPort()))
    {
        LOGC("Could not connect client socket");
        return -1;
    }

    chessmate::Message message(chessmate::Message::START_GAME, "0 1");
    chessmate::MoveSetPtr spMoveSet = std::make_shared<chessmate::MoveSet>();

    if (spSocket->Send(message.Serialize()))
    {
        std::string received = spSocket->Recv();
        LOGC("%s", received);
    }
    else
    {
        LOGC("Could not send message");
    }

    chessmate::ChessGamePtr spGame = chessmate::ChessGame::Create(
        spConfig, spSocket, spMoveSet, message
    );

    QWidget *myWidget = new QWidget();
    myWidget->setGeometry(0,0,1370,700);

    accessories(myWidget);
    chessBoard(myWidget,tile);

    myWidget->show();
    return application.exec();
}

