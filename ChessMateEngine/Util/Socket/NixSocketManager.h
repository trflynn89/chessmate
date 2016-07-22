#pragma once

#include <atomic>

#include <sys/select.h>

#include <Util/Socket/SocketManager.h>

namespace Util {

DEFINE_CLASS_PTRS(ConfigManager);

/**
 * Linux implementation of the SocketManager interface.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 21, 2016
 */
class SocketManagerImpl : public SocketManager
{
public:
    SocketManagerImpl();
    SocketManagerImpl(ConfigManagerPtr &);
    virtual ~SocketManagerImpl();

protected:
    virtual bool DoWork();

private:
    ssize_t setReadAndWriteMasks(fd_set *, fd_set *);
    void handleSocketIO(fd_set *, fd_set *);
    SocketPtr acceptNewClient(const SocketPtr &);

    static std::atomic_int s_socketManagerCount;
};

}
