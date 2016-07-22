#include "SocketManager.h"

#include <Util/Config/ConfigManager.h>
#include <Util/Logging/Logger.h>
#include <Util/Socket/SocketConfig.h>
#include <Util/Socket/SocketImpl.h>

namespace Util {

//==============================================================================
SocketManager::SocketManager() :
    Runner("SocketManager", 1),
    m_newClientCallback(nullptr),
    m_closedClientCallback(nullptr),
    m_spConfig(std::make_shared<SocketConfig>())
{
}

//==============================================================================
SocketManager::SocketManager(ConfigManagerPtr &spConfigManager) :
    Runner("SocketManager", 1),
    m_newClientCallback(nullptr),
    m_closedClientCallback(nullptr),
    m_spConfig(spConfigManager->CreateConfig<SocketConfig>())
{
}

//==============================================================================
SocketManager::~SocketManager()
{
}

//==============================================================================
void SocketManager::SetClientCallbacks(NewClientCallback newClient,
    ClosedClientCallback closedClient)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);

    m_newClientCallback = newClient;
    m_closedClientCallback = closedClient;
}

//==============================================================================
void SocketManager::ClearClientCallbacks()
{
    SetClientCallbacks(nullptr, nullptr);
}

//==============================================================================
SocketPtr SocketManager::CreateTcpSocket()
{
    SocketImplPtr spSocket = std::make_shared<SocketImpl>(
        Socket::SOCKET_TCP, m_spConfig
    );

    if (!spSocket->IsValid())
    {
        spSocket.reset();
    }

    return std::dynamic_pointer_cast<Socket>(spSocket);
}

//==============================================================================
SocketWPtr SocketManager::CreateAsyncTcpSocket()
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

//==============================================================================
SocketPtr SocketManager::CreateUdpSocket()
{
    SocketImplPtr spSocket = std::make_shared<SocketImpl>(
        Socket::SOCKET_UDP, m_spConfig
    );

    if (!spSocket->IsValid())
    {
        spSocket.reset();
    }

    return std::dynamic_pointer_cast<Socket>(spSocket);
}

//==============================================================================
SocketWPtr SocketManager::CreateAsyncUdpSocket()
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

//==============================================================================
bool SocketManager::StartRunner()
{
    return true;
}

//==============================================================================
void SocketManager::StopRunner()
{
    LOGC("Stopping socket manager");

    ClearClientCallbacks();

    std::lock_guard<std::mutex> lock(m_aioSocketsMutex);
    m_aioSockets.clear();
}

}
