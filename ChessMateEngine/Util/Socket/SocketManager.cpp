#include "SocketManager.h"

#include <Util/Logging/Logger.h>
#include <Util/Socket/SocketImpl.h>

namespace Util {

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

    m_serviceFuture = std::async(std::launch::async, function, spThis);
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

    std::lock_guard<std::mutex> lock(m_aioSocketsMutex);
    m_aioSockets.clear();
}

//=============================================================================
void SocketManager::SetClientCallbacks(NewClientCallback newClient,
    ClosedClientCallback closedClient)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

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
    SocketPtr spSocket = std::make_shared<SocketImpl>();

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
            std::lock_guard<std::mutex> lock(m_aioSocketsMutex);
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
    SocketPtr spSocket = std::make_shared<SocketImpl>();

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
            std::lock_guard<std::mutex> lock(m_aioSocketsMutex);
            m_aioSockets.push_back(spSocket);
        }
        else
        {
            spSocket.reset();
        }
    }

    return spSocket;
}

}
