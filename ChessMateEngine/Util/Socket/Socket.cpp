#include "Socket.h"
#include "SocketImpl.h"

#include <Util/Logging/Logger.h>
#include <Util/String/String.h>

namespace Util {

//=============================================================================
const char Socket::s_socketEoM = 0x04;
std::atomic_int Socket::s_aNumSockets(0);

//=============================================================================
Socket::Socket() :
    m_socketHandle(-1),
    m_clientIp(-1),
    m_clientPort(-1),
    m_isAsync(false),
    m_isListening(false),
    m_aConnectedState(Socket::NOT_CONNECTED),
    m_socketId(s_aNumSockets.fetch_add(1))
{
}

//=============================================================================
int Socket::InAddrAny()
{
    return SocketImpl::InAddrAny();
}

//=============================================================================
bool Socket::IsValid() const
{
    return (m_socketHandle > 0);
}

//=============================================================================
bool Socket::IsAsync() const
{
    return m_isAsync;
}

//=============================================================================
int Socket::GetHandle() const
{
    return m_socketHandle;
}

//=============================================================================
int Socket::GetClientIp() const
{
    return m_clientIp;
}

//=============================================================================
int Socket::GetClientPort() const
{
    return m_clientPort;
}

//=============================================================================
int Socket::GetSocketId() const
{
    return m_socketId;
}

//=============================================================================
bool Socket::IsListening() const
{
    return m_isListening;
}

//=============================================================================
bool Socket::IsConnecting() const
{
    return (m_aConnectedState.load() == Socket::CONNECTING);
}

//=============================================================================
bool Socket::IsConnected() const
{
    return (m_aConnectedState.load() == Socket::CONNECTED);
}

//=============================================================================
Socket::ConnectedState Socket::ConnectAsync(std::string hostname, int port)
{
    Socket::ConnectedState state = NOT_CONNECTED;

    if (IsAsync())
    {
        if (Connect(hostname, port))
        {
            LOGD(m_socketId, "Connected to %s:%d", hostname, port);
            state = CONNECTED;
        }
        else if (IsConnecting())
        {
            LOGD(m_socketId, "Connect to %s:%d in progress", hostname, port);

            AsyncConnect connect(m_socketId, hostname, port);
            m_pendingConnects.Push(connect);

            state = CONNECTING;
        }
        else
        {
            LOGW(m_socketId, "Could not connect to %s:%d, closing socket",
                hostname, port);

            Close();
        }
    }

    return state;
}

//=============================================================================
bool Socket::SendAsync(const std::string &msg)
{
    static const std::string eom(1, s_socketEoM);

    if (IsAsync())
    {
        AsyncRequest request(m_socketId, msg + eom);
        m_pendingSends.Push(request);

        return true;
    }

    return false;
}

//=============================================================================
void Socket::ServiceConnectRequests(AsyncConnect::ConnectQueue &completedConnects)
{
    bool wouldBlock = false;

    while (IsValid() && !m_pendingConnects.IsEmpty() && !wouldBlock)
    {
        AsyncConnect connect;
        m_pendingConnects.Pop(connect);

        if (connect.IsValid())
        {
            const std::string hostname = connect.GetHostname();
            const int port = connect.GetPort();

            if (IsErrorFree())
            {
                LOGD(m_socketId, "Connected to %s:%d", hostname, port);
                m_aConnectedState.store(Socket::CONNECTED);

                completedConnects.Push(connect);
            }
            else
            {
                LOGW(m_socketId, "Could not connect to %s:%d, closing socket",
                    hostname, port);
                m_aConnectedState.store(Socket::NOT_CONNECTED);

                Close();
            }
        }
    }
}

//=============================================================================
void Socket::ServiceSendRequests(AsyncRequest::RequestQueue &completedSends)
{
    bool wouldBlock = false;

    while (IsValid() && !m_pendingSends.IsEmpty() && !wouldBlock)
    {
        AsyncRequest request;
        m_pendingSends.Pop(request);

        if (request.IsValid())
        {
            const std::string &msg = request.GetRequest();
            unsigned int bytesSent = Send(msg, wouldBlock);

            if (bytesSent == msg.length())
            {
                LOGD(m_socketId, "Sent %u bytes", bytesSent);
                completedSends.Push(request);
            }
            else if (wouldBlock)
            {
                LOGI(m_socketId, "Send would block - sent %u of %u bytes, "
                    "will finish later", bytesSent, msg.length());

                SendAsync(msg.substr(bytesSent, std::string::npos));
            }
            else
            {
                LOGW(m_socketId, "Can't send, closing socket");
                Close();
            }
        }
    }
}

//=============================================================================
void Socket::ServiceRecvRequests(AsyncRequest::RequestQueue &completedReceives)
{
    bool wouldBlock = false;

    while (IsValid() && !wouldBlock)
    {
        std::string received = Recv(wouldBlock);

        if (received.length() > 0)
        {
            LOGD(m_socketId, "Received %u bytes, %u in buffer",
                received.length(), m_receiveBuffer.length());

            m_receiveBuffer += received;

            for (std::string &message : splitReceiveBuffer())
            {
                LOGD(m_socketId, "Completed message, %u bytes", message.length());

                AsyncRequest request(m_socketId, message);
                completedReceives.Push(request);
            }
        }
        else if (wouldBlock)
        {
            LOGI(m_socketId, "Receive would block - received %u bytes, "
                "will finish later", m_receiveBuffer.length());
        }
        else
        {
            LOGW(m_socketId, "Can't receive, closing socket");
            Close();
        }
    }
}

//=============================================================================
std::vector<std::string> Socket::splitReceiveBuffer()
{
    std::vector<std::string> messages = String::Split(m_receiveBuffer, s_socketEoM);

    // Check if the buffer ends with a complete message
    // If not, keep the last part in the buffer
    if (m_receiveBuffer.rfind(s_socketEoM) != (m_receiveBuffer.length() - 1))
    {
        m_receiveBuffer = messages.back();
        messages.pop_back();
    }
    else
    {
        m_receiveBuffer.clear();
    }

    return messages;
}

}
