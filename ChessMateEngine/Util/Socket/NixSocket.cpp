#include <WinSock.h>

#include "WinSocket.h"

namespace Util {

//=============================================================================
SocketImpl::SocketImpl() :
    m_socketHandle(-1),
    m_clientIp(-1),
    m_clientPort(-1),
    m_isAsync(false),
    m_isListening(false)
{
    WSADATA wsadata;

    // Create the winsocket
    if (WSAStartup(0x0202, &wsadata) != 0)
    {
        WSACleanup();
    }
}

//=============================================================================
SocketImpl::~SocketImpl()
{
    Close();

    m_socketHandle = -1;
    m_clientIp = -1;
    m_clientPort = -1;
}

//=============================================================================
int SocketImpl::InAddrAny()
{
    return INADDR_ANY;
}

//=============================================================================
void SocketImpl::Close()
{
    if (IsValid())
    {
        ::closesocket(m_socketHandle);
        m_socketHandle = -1;

        WSACleanup();
    }
}

//=============================================================================
bool SocketImpl::IsValid() const
{
    return (m_socketHandle > 0);
}

//=============================================================================
bool SocketImpl::InitTcpSocket()
{
    if ((m_socketHandle = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return false;
    }

    return true;
}

//=============================================================================
bool SocketImpl::InitUdpSocket()
{
    if ((m_socketHandle = ::socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        return false;
    }

    return true;
}

//=============================================================================
void SocketImpl::SetAsync()
{
    unsigned long nonZero = 1;

    if (ioctlsocket(m_socketHandle, FIONBIO, &nonZero) == 0)
    {
        m_isAsync = true;
    }
}

//=============================================================================
bool SocketImpl::IsAsync() const
{
    return m_isAsync;
}

//=============================================================================
int SocketImpl::GetHandle() const
{
    return m_socketHandle;
}

//=============================================================================
void SocketImpl::SetHandle(const int handle)
{
    m_socketHandle = handle;
}

//=============================================================================
int SocketImpl::GetClientIp() const
{
    return m_clientIp;
}

//=============================================================================
void SocketImpl::SetClientIp(int ip)
{
    m_clientIp = ip;
}

//=============================================================================
int SocketImpl::GetClientPort() const
{
    return m_clientPort;
}

//=============================================================================
void SocketImpl::SetClientPort(int port)
{
    m_clientPort= port;
}

//=============================================================================
bool SocketImpl::Bind(int addr, int port) const
{
    // Construct the server address structure
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(addr);
    servAddr.sin_port = htons(port);

    // Bind socket to the server
    if (::bind(m_socketHandle, (struct sockaddr *)&servAddr, sizeof(servAddr)) != 0)
    {
        return false;
    }

    return true;
}

//=============================================================================
bool SocketImpl::BindForReuse(int addr, int port) const
{
    const char optval = 1;
    ::setsockopt(m_socketHandle, SOL_SOCKET, SO_REUSEADDR, &optval, 4);
    return Bind(addr, port);
}

//=============================================================================
bool SocketImpl::Listen()
{
    m_isListening = (::listen(m_socketHandle, 100) == 0);
    return m_isListening;
}

//=============================================================================
bool SocketImpl::IsListening() const
{
    return m_isListening;
}

//=============================================================================
bool SocketImpl::Connect(std::string hostname, int port) const
{
    // Convert hostname to IP
    struct hostent *serverIP = NULL;
    if ((serverIP = gethostbyname(hostname.c_str())) == NULL)
    {
        return false;
    }

    // Construct the server address structure
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    memcpy((char *)&server.sin_addr, serverIP->h_addr, serverIP->h_length);
    server.sin_port = htons(port);

    if (::connect(m_socketHandle, (struct sockaddr *)&server, sizeof(server)) != 0)
    {
        return false;
    }

    return true;
}

//=============================================================================
SocketImpl *SocketImpl::Accept() const
{
    struct sockaddr_in client;
    int clientLen = sizeof(client),  skt;
    SocketImpl *ret = new SocketImpl();

    if ((skt = ::accept(m_socketHandle, (struct sockaddr *)&client, &clientLen)) < 0)
    {
        return NULL;
    }

    ret->m_socketHandle = skt;
    ret->m_clientIp = ntohl(client.sin_addr.s_addr);
    ret->m_clientPort = ntohl(client.sin_port);

    return ret;
}

//=============================================================================
unsigned int SocketImpl::Send(std::string msg) const
{
    bool ignore;
    return Send(msg, ignore);
}

//=============================================================================
unsigned int SocketImpl::Send(std::string msg, bool &wouldBlock) const
{
    bool keepSending = (msg.length() > 0);
    unsigned int bytesSent = 0;

    wouldBlock = false;

    // Send data through socket until done
    while (keepSending)
    {
        int currSent = ::send(m_socketHandle, msg.c_str(), msg.length(), 0);

        if (currSent > 0)
        {
            msg = msg.substr(currSent, std::string::npos);
            bytesSent += currSent;

            keepSending = (msg.length() > 0);
        }
        else
        {
            int error = WSAGetLastError();
            keepSending = false;

            if ((currSent == -1) && (error == WSAEWOULDBLOCK))
            {
                wouldBlock = true;
            }
        }
    }

    return bytesSent;
}

//=============================================================================
std::string SocketImpl::Recv() const
{
    bool ignore;
    return Recv(ignore);
}

//=============================================================================
std::string SocketImpl::Recv(bool &wouldBlock) const
{
    bool keepReading = true;
    std::string ret;

    wouldBlock = false;

    while (keepReading)
    {
        char *buff = (char *)calloc(1, 128 * sizeof(char));

        // Read data
        int bytesRead = ::recv(m_socketHandle, buff, 127, 0);

        if (bytesRead > 0)
        {
            ret.append(buff, bytesRead);

            keepReading = (ret.back() != 0x04);
        }
        else
        {
            int error = WSAGetLastError();
            keepReading = false;

            if ((bytesRead == -1) && (error == WSAEWOULDBLOCK))
            {
                wouldBlock = true;
            }
        }

        free(buff);
    }

    return ret;
}

}
