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

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    chessmate::ChessMateGui gui;

    if (gui.Initialize())
    {
        gui.show();

        return app.exec();
    }
    else
    {
        return -1;
    }

/*
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
*/
}

