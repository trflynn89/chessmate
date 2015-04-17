#pragma once

#include "SocketManager.h"

#include <Winsock2.h>
#include <Windows.h>

namespace Util { namespace Socket {

class SocketManagerImpl : public SocketManager
{
public:
	SocketManagerImpl();
	~SocketManagerImpl();

protected:
	void AsyncIoThread();

private:
	int setReadAndWriteMasks(fd_set *, fd_set *);
	void handleSocketIO(fd_set *, fd_set *);
	SocketPtr acceptNewClient(const SocketPtr &);
};

}}
