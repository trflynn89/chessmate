#include <WinSock.h>

#include "WinSocket.h"

#include <Util/Logging/Logger.h>

using std::make_shared;
using std::string;

namespace Util { namespace Socket {

//=============================================================================
SocketImpl::SocketImpl() : Socket()
{
}

//=============================================================================
SocketImpl::~SocketImpl()
{
	Close();
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
	}
}

//=============================================================================
bool SocketImpl::InitTcpSocket()
{
	m_socketHandle = ::socket(AF_INET, SOCK_STREAM, 0);
	return IsValid();
}

//=============================================================================
bool SocketImpl::InitUdpSocket()
{
	m_socketHandle = ::socket(AF_INET, SOCK_DGRAM, 0);
	return IsValid();
}

//=============================================================================
bool SocketImpl::IsErrorFree()
{
	int opt = 0;
	int len = sizeof(opt);

	int ret = ::getsockopt(m_socketHandle, SOL_SOCKET, SO_ERROR, (char *)&opt, &len);

	return ((ret == 0) && (opt == 0));
}

//=============================================================================
bool SocketImpl::SetAsync()
{
	unsigned long nonZero = 1;

	if (ioctlsocket(m_socketHandle, FIONBIO, &nonZero) == 0)
	{
		m_isAsync = true;
	}

	return m_isAsync;
}

//=============================================================================
bool SocketImpl::Bind(int addr, int port) const
{
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(addr);
	servAddr.sin_port = htons(port);

	struct sockaddr *sockAddr = reinterpret_cast<sockaddr *>(&servAddr);
	int ret = ::bind(m_socketHandle, sockAddr, sizeof(servAddr));

	return (ret != -1);
}

//=============================================================================
bool SocketImpl::BindForReuse(int addr, int port) const
{
	const char opt = 1;
	int ret = ::setsockopt(m_socketHandle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	return ((ret != -1) && Bind(addr, port));
}

//=============================================================================
bool SocketImpl::Listen()
{
	m_isListening = (::listen(m_socketHandle, 100) == 0);
	return m_isListening;
}

//=============================================================================
bool SocketImpl::Connect(const string &hostname, int port)
{
	if (m_aConnectedState.load() != Socket::NOT_CONNECTED)
	{
		return false;
	}

	// Convert hostname to IP
	struct hostent *serverIP = gethostbyname(hostname.c_str());

	if (serverIP == NULL)
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
		int error = WSAGetLastError();

		if ((error == WSAEWOULDBLOCK) || (error == WSAEINPROGRESS))
		{
			m_aConnectedState.store(Socket::CONNECTING);
		}

		return false;
	}

	m_aConnectedState.store(Socket::CONNECTED);
	return true;
}

//=============================================================================
SocketPtr SocketImpl::Accept() const
{
	SocketImplPtr ret = make_shared<SocketImpl>();

	struct sockaddr_in client;
	int clientLen = sizeof(client);

	int skt = ::accept(m_socketHandle, (struct sockaddr *)&client, &clientLen);

	if (skt == INVALID_SOCKET)
	{
		ret.reset();
	}
	else
	{
		ret->m_socketHandle = skt;
		ret->m_clientIp = ntohl(client.sin_addr.s_addr);
		ret->m_clientPort = ntohl(client.sin_port);
		ret->m_aConnectedState.store(Socket::CONNECTED);
	}

	return ret;
}

//=============================================================================
unsigned int SocketImpl::Send(const string &msg) const
{
	bool ignore;
	return Send(msg, ignore);
}

//=============================================================================
unsigned int SocketImpl::Send(const string &msg, bool &wouldBlock) const
{
	string toSend = msg;

	bool keepSending = (toSend.length() > 0);
	unsigned int bytesSent = 0;

	wouldBlock = false;

	while (keepSending)
	{
		int currSent = ::send(m_socketHandle, toSend.c_str(), toSend.size(), 0);

		if (currSent > 0)
		{
			toSend = toSend.substr(currSent, string::npos);
			bytesSent += currSent;

			keepSending = (toSend.length() > 0);
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
string SocketImpl::Recv() const
{
	bool ignore;
	return Recv(ignore);
}

//=============================================================================
string SocketImpl::Recv(bool &wouldBlock) const
{
	string ret;

	wouldBlock = false;
	bool keepReading = true;

	while (keepReading)
	{
		static const int buffLen = 4096;
		char *buff = (char *)calloc(1, buffLen * sizeof(char));

		int bytesRead = ::recv(m_socketHandle, buff, buffLen, 0);

		if (bytesRead > 0)
		{
			ret.append(buff, bytesRead);
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

}}
