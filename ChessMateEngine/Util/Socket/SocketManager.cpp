/**
 * Implementation of the SocketManager interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
#include "SocketManager.h"

#include <Util/Logging/Logger.h>
#include <Util/Socket/SocketImpl.h>

using std::async;
using std::function;
using std::launch;
using std::lock_guard;
using std::make_shared;
using std::mutex;

namespace Util { namespace Socket {

//=============================================================================
SocketManager::SocketManager() :
	m_aKeepRunning(true),
	m_newClientCallback(nullptr),
	m_closedClientCallback(nullptr)
{
}

//=============================================================================
SocketManager::~SocketManager()
{
	if (m_aKeepRunning.load())
	{
		StopSocketManager();
	}
}

//=============================================================================
void SocketManager::StartSocketManager()
{
	const SocketManagerPtr &spThis = shared_from_this();
	auto function = &SocketManager::AsyncIoThread;

	m_serviceFuture = async(launch::async, function, spThis);
}

//=============================================================================
void SocketManager::StopSocketManager()
{
	LOGC("Stopping socket manager");

	if (m_serviceFuture.valid())
	{
		m_aKeepRunning.store(false);
		m_serviceFuture.get();
	}

	ClearClientCallbacks();

	lock_guard<mutex> lock(m_aioSocketsMutex);
	m_aioSockets.clear();
}

//=============================================================================
void SocketManager::SetClientCallbacks(NewClientCallback newClient,
	ClosedClientCallback closedClient)
{
	lock_guard<mutex> lock(m_callbackMutex);

	m_newClientCallback = newClient;
	m_closedClientCallback = closedClient;
}

//=============================================================================
void SocketManager::ClearClientCallbacks()
{
	SetClientCallbacks(nullptr, nullptr);
}

//=============================================================================
SocketPtr SocketManager::CreateTcpSocket()
{
	SocketPtr spSocket = make_shared<SocketImpl>();

	if (!spSocket->InitTcpSocket())
	{
		spSocket.reset();
	}

	return spSocket;
}

//=============================================================================
SocketPtr SocketManager::CreateAsyncTcpSocket()
{
	SocketPtr spSocket = CreateTcpSocket();

	if (spSocket)
	{
		if (spSocket->SetAsync())
		{
			lock_guard<mutex> lock(m_aioSocketsMutex);
			m_aioSockets.push_back(spSocket);
		}
		else
		{
			spSocket.reset();
		}
	}

	return spSocket;
}

//=============================================================================
SocketPtr SocketManager::CreateUdpSocket()
{
	SocketPtr spSocket = make_shared<SocketImpl>();

	if (!spSocket->InitUdpSocket())
	{
		spSocket.reset();
	}

	return spSocket;
}

//=============================================================================
SocketPtr SocketManager::CreateAsyncUdpSocket()
{
	SocketPtr spSocket = CreateUdpSocket();

	if (spSocket)
	{
		if (spSocket->SetAsync())
		{
			lock_guard<mutex> lock(m_aioSocketsMutex);
			m_aioSockets.push_back(spSocket);
		}
		else
		{
			spSocket.reset();
		}
	}

	return spSocket;
}

}}
