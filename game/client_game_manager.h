#pragma once

#include <memory>

#include <fly/fly.h>

#include <game/game_manager.h>

namespace fly {

DEFINE_CLASS_PTRS(AsyncRequest);
DEFINE_CLASS_PTRS(ConfigManager);
DEFINE_CLASS_PTRS(Socket);
DEFINE_CLASS_PTRS(SocketManager);

}

namespace chessmate {

DEFINE_CLASS_PTRS(ChessGame);
DEFINE_CLASS_PTRS(ClientGameManager);

/**
 * Manager class to own and control all chess game instances.
 *
 * Provides an interface for managing chess games. Allows for adding new games
 * to the manager, and for stopping any game(s).
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 21, 2016
 */
class ClientGameManager : public GameManager
{
public:
    /**
     * Constructor, stores a weak reference to the socket manager.
     *
     * @param ConfigManagerPtr Reference to the configuration manager.
     * @param SocketManagerPtr Reference to the socket manager.
     */
    ClientGameManager(fly::ConfigManagerPtr &, const fly::SocketManagerPtr &);

    /**
     * Destructor. Stop all games if they have not been already.
     */
    ~ClientGameManager();

protected:
    /**
     * Process any data received by the socket manager.
     *
     * @return True if the game manager is healthy.
     */
    virtual bool DoWork();

    /**
     * Create the accept socket for new games to connect to.
     *
     * @param int The port to listen on.
     *
     * @return True if the socket could be created and initialized.
     */
    virtual bool createGameSocket(int);

private:

    bool checkForConnection(fly::AsyncConnect &) const;

    void createGameFromConnect(const fly::AsyncConnect &);
};

}
