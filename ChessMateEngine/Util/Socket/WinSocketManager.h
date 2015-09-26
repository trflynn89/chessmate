#pragma once

#include "SocketManager.h"

#include <Winsock2.h>
#include <Windows.h>

namespace Util {

/**
 * Windows implementation of the SocketManager interface.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version December 12, 2012
 */
class SocketManagerImpl : public SocketManager
{
public:
    SocketManagerImpl();
    ~SocketManagerImpl();

protected:
    void AsyncIoThread();

private:
    bool setReadAndWriteMasks(fd_set *, fd_set *);
    void handleSocketIO(fd_set *, fd_set *);
    SocketPtr acceptNewClient(const SocketPtr &);
};

}
