#include "AsyncStructs.h"

namespace Util {

//=============================================================================
namespace
{
    static int s_invalidId = -1;
}

//=============================================================================
AsyncBase::AsyncBase() : m_socketId(s_invalidId)
{
}

//=============================================================================
AsyncBase::AsyncBase(int socketId) : m_socketId(socketId)
{
}

//=============================================================================
bool AsyncBase::IsValid() const
{
    return (m_socketId != s_invalidId);
}

//=============================================================================
int AsyncBase::GetSocketId() const
{
    return m_socketId;
}

//=============================================================================
AsyncRequest::AsyncRequest() : AsyncBase(s_invalidId), m_request()
{
}

//=============================================================================
AsyncRequest::AsyncRequest(int socketId) : AsyncBase(socketId), m_request()
{
}

//=============================================================================
AsyncRequest::AsyncRequest(int socketId, std::string request) :
    AsyncBase(socketId), m_request(request)
{
}

//=============================================================================
std::string AsyncRequest::GetRequest() const
{
    return m_request;
}

//=============================================================================
AsyncConnect::AsyncConnect() : AsyncBase(s_invalidId), m_hostname(), m_port(0)
{
}

//=============================================================================
AsyncConnect::AsyncConnect(int socketId, std::string host, int port) :
    AsyncBase(socketId), m_hostname(host), m_port(port)
{
}

//=============================================================================
std::string AsyncConnect::GetHostname() const
{
    return m_hostname;
}

//=============================================================================
int AsyncConnect::GetPort() const
{
    return m_port;
}

}
