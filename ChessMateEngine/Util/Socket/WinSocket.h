#pragma once

#include <string>

#include <Util/Utilities.h>
#include <Util/Socket/Socket.h>

namespace Util {

DEFINE_CLASS_PTRS(SocketImpl);

/**
 * Windows implementation of the Socket interface.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version December 12, 2012
 */
class SocketImpl : public Socket
{
public:
    SocketImpl();
    ~SocketImpl();

    static int InAddrAny();

    void Close();

    bool InitTcpSocket();
    bool InitUdpSocket();

    bool IsErrorFree();

    bool SetAsync();

    bool Bind(int, int) const;
    bool BindForReuse(int, int) const;
    bool Listen();
    bool Connect(const std::string &, int);
    SocketPtr Accept() const;

    size_t Send(const std::string &) const;
    size_t Send(const std::string &, bool &) const;

    std::string Recv() const;
    std::string Recv(bool &, bool &) const;
};

}
