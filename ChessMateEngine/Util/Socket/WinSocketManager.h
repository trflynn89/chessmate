#pragma once

#include <atomic>

#include <Windows.h>

#include <Util/Config/ConfigManager.h>
#include <Util/Socket/SocketManager.h>

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
    SocketManagerImpl(ConfigManagerPtr &spConfigManager);
    virtual ~SocketManagerImpl();

protected:
    void AsyncIoThread();

private:
    bool setReadAndWriteMasks(fd_set *, fd_set *);
    void handleSocketIO(fd_set *, fd_set *);
    SocketPtr acceptNewClient(const SocketPtr &);

    static std::atomic_int s_socketManagerCount;
};

}
