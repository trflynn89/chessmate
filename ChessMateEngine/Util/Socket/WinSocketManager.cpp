#include "WinSocketManager.h"

#include <algorithm>
#include <vector>

#include <Util/Logging/Logger.h>

namespace Util {

//=============================================================================
SocketManagerImpl::SocketManagerImpl() : SocketManager()
{
    WORD version = MAKEWORD(2, 2);
    WSADATA wsadata;

    if (WSAStartup(version, &wsadata) != 0)
    {
        WSACleanup();
    }
}

//=============================================================================
SocketManagerImpl::~SocketManagerImpl()
{
    WSACleanup();
}

//=============================================================================
void SocketManagerImpl::AsyncIoThread()
{
    fd_set readFd, writeFd;
    struct timeval tv { 0, 10000 }; // 10 milliseconds

    while (m_aKeepRunning.load())
    {
        bool anyMasksSet = false;
        {
            std::lock_guard<std::mutex> lock(m_aioSocketsMutex);
            anyMasksSet = setReadAndWriteMasks(&readFd, &writeFd);
        }

        if (anyMasksSet)
        {
            // First argument of ::select() is ignored in Windows
            // https://msdn.microsoft.com/en-us/library/windows/desktop/ms740141(v=vs.85).aspx
            if (::select(0, &readFd, &writeFd, NULL, &tv) > 0)
            {
                std::lock_guard<std::mutex> lock(m_aioSocketsMutex);
                handleSocketIO(&readFd, &writeFd);
            }
        }
    }
}

//=============================================================================
bool SocketManagerImpl::setReadAndWriteMasks(fd_set *readFd, fd_set *writeFd)
{
    bool anyMasksSet = false;

    FD_ZERO(readFd);
    FD_ZERO(writeFd);

    for (auto it = m_aioSockets.begin(); it != m_aioSockets.end(); )
    {
        SocketPtr &spSocket = *it;

        if (spSocket->IsValid())
        {
            FD_SET(spSocket->GetHandle(), readFd);
            FD_SET(spSocket->GetHandle(), writeFd);

            anyMasksSet = true;
            ++it;
        }
        else
        {
            it = m_aioSockets.erase(it);

            std::lock_guard<std::mutex> lock(m_callbackMutex);

            if (m_closedClientCallback != nullptr)
            {
                m_closedClientCallback(spSocket->GetSocketId());
            }
        }
    }

    return anyMasksSet;
}

//=============================================================================
void SocketManagerImpl::handleSocketIO(fd_set *readFd, fd_set *writeFd)
{
    std::vector<SocketPtr> newClients;

    for (auto it = m_aioSockets.begin(); it != m_aioSockets.end(); )
    {
        SocketPtr &spSocket = *it;

        if (spSocket->IsValid())
        {
            size_t handle = spSocket->GetHandle();

            // Handle socket accepts and reads
            if (FD_ISSET(handle, readFd))
            {
                if (spSocket->IsListening())
                {
                    SocketPtr spNewClient = acceptNewClient(spSocket);

                    if (spNewClient->IsValid())
                    {
                        newClients.push_back(spNewClient);
                    }
                }
                else
                {
                    spSocket->ServiceRecvRequests(m_completedReceives);
                }
            }

            // Handle socket connects and writes
            if (FD_ISSET(handle, writeFd))
            {
                if (spSocket->IsConnecting())
                {
                    spSocket->ServiceConnectRequests(m_completedConnects);
                }
                else
                {
                    spSocket->ServiceSendRequests(m_completedSends);
                }
            }

            ++it;
        }
        else
        {
            it = m_aioSockets.erase(it);

            std::lock_guard<std::mutex> lock(m_callbackMutex);

            if (m_closedClientCallback != nullptr)
            {
                m_closedClientCallback(spSocket->GetSocketId());
            }
        }
    }

    // Add new clients to the list of asynchronous sockets
    for (auto it = newClients.begin(); it != newClients.end(); ++it)
    {
        m_aioSockets.push_back(std::move(*it));
    }
}

//=============================================================================
SocketPtr SocketManagerImpl::acceptNewClient(const SocketPtr &spSocket)
{
    SocketPtr spNewClientSocket = spSocket->Accept();

    if (spNewClientSocket->SetAsync())
    {
        std::lock_guard<std::mutex> lock(m_callbackMutex);

        if (m_newClientCallback != nullptr)
        {
            m_newClientCallback(spNewClientSocket);
        }
    }
    else
    {
        LOGW(-1, "Could not make new client socket asynchronous, closing");
        spNewClientSocket->Close();
    }

    return spNewClientSocket;
}

}
