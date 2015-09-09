#pragma once

#include <string>

namespace Util {

/**
 * Linux implementation of the Socket interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version December 12, 2012
 */
class SocketImpl
{
public:
    SocketImpl();
    ~SocketImpl();

    static int InAddrAny();

    void Close();
    bool IsValid() const;

    bool InitTcpSocket();
    bool InitUdpSocket();

    void SetAsync();
    bool IsAsync() const;

    int GetHandle() const;
    void SetHandle(int);

    int GetClientIp() const;
    void SetClientIp(int);

    int GetClientPort() const;
    void SetClientPort(int);

    bool Bind(int, int) const;
    bool BindForReuse(int, int) const;
    bool Listen();
    bool IsListening() const;
    bool Connect(std::string, int) const;
    SocketImpl *Accept() const;

    unsigned int Send(std::string) const;
    unsigned int Send(std::string, bool &) const;

    std::string Recv() const;
    std::string Recv(bool &) const;

private:
    // File descriptor for this socket.
    int m_socketHandle;

    // Client IP and port this socket is connected to.
    int m_clientIp;
    int m_clientPort;

    // Whether this socket should allow asynchronous operations
    bool m_isAsync;

    // Whether this socket is a listening socket
    bool m_isListening;
};

}
