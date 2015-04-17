#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include <ChessMateEngine.h>
#include <Util/Logging/Logger.h>
#include <Util/Socket/AsyncStructs.h>
#include <Util/Socket/SocketManager.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/String/String.h>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::seconds;
using std::make_shared;
using std::string;

using Util::Logging::Logger;
using Util::Logging::LoggerPtr;
using Util::Socket::AsyncConnect;
using Util::Socket::AsyncRequest;
using Util::Socket::Socket;
using Util::Socket::SocketPtr;
using Util::Socket::SocketManagerPtr;
using Util::Socket::SocketManagerImpl;
using Util::String::String;

//=============================================================================
LoggerPtr InitLogger()
{
	LoggerPtr spLogger = make_shared<Logger>();
	Logger::SetInstance(spLogger);

	return spLogger;
}

//=============================================================================
bool CreateAcceptSocket(SocketManagerPtr spSocketManager, int acceptPort)
{
	SocketPtr spSocket = spSocketManager->CreateAsyncTcpSocket();

	if (spSocket)
	{
		if (!spSocket->BindForReuse(Socket::InAddrAny(), acceptPort))
		{
			spSocket.reset();
		}

		else if (!spSocket->Listen())
		{
			spSocket.reset();
		}
	}

	return (spSocket && spSocket->IsListening());
}

//=============================================================================
SocketManagerPtr InitSocketManager(int acceptPort)
{
	SocketManagerPtr spSocketManager = make_shared<SocketManagerImpl>();
	spSocketManager->StartSocketManager();

	if (!CreateAcceptSocket(spSocketManager, acceptPort))
	{
		spSocketManager.reset();
	}

	return spSocketManager;
}

//=============================================================================
void StopSocketManager(SocketManagerPtr spSocketManager)
{
	if (spSocketManager)
	{
		spSocketManager->StopSocketManager();
	}
}

//=============================================================================
SocketPtr ConnectNewSocket(SocketManagerPtr spSocketManager, string host, int port)
{
	SocketPtr spSocket = spSocketManager->CreateAsyncTcpSocket();

	if (spSocket)
	{
		Socket::ConnectedState state = spSocket->ConnectAsync(host, port);

		if (state == Socket::CONNECTED)
		{
			LOGC("Connected immediately to \"%s:%d\"", host.c_str(), port);
		}
		else if (state == Socket::CONNECTING)
		{
			AsyncConnect connect;
			seconds waitTime(1);

			if (spSocketManager->WaitForCompletedConnect(connect, waitTime))
			{
				if (spSocket->IsConnected())
				{
					LOGC("Connected asynchronously to \"%s:%d\"", host.c_str(), port);
				}
				else
				{
					LOGC("Async connect to \"%s:%d\" was valid, but not connected", host.c_str(), port);
					spSocket.reset();
				}
			}
			else
			{
				LOGC("Not connected to \"%s:%d\"", host.c_str(), port);
				spSocket.reset();
			}
		}
		else
		{
			LOGC("Connect to \"%s:%d\" failed", host.c_str(), port);
			spSocket.reset();
		}
	}
	else
	{
		LOGC("Could not create socket");
	}

	return spSocket;
}

//=============================================================================
bool RunSendRecvTest(SocketManagerPtr spSocketManager, SocketPtr spSocket, unsigned int messageLength)
{
	if (!spSocket->IsValid())
	{
		LOGC("Socket has been closed");
		return false;
	}

	LOGC("Generating message of length %u", messageLength);
	const string message = String::GenerateRandomString(messageLength);

	LOGC("Client sending message of length %u", message.length());
	high_resolution_clock::time_point startTime = high_resolution_clock::now();

	if (spSocket->SendAsync(message))
	{
		AsyncRequest request;
		seconds waitTime(120);

		if (spSocketManager->WaitForCompletedReceive(request, waitTime))
		{
			high_resolution_clock::time_point endTime = high_resolution_clock::now();

			string received = request.GetRequest();
			string::size_type bytesReceived = received.length();

			duration<double> recvTime = duration_cast<duration<double>>(endTime - startTime);
			double throughput = static_cast<double>(bytesReceived) / 1024.0 / recvTime.count();

			LOGC("Server received message of length %u, average throughput of %f KBps",
				bytesReceived, throughput);

			return (message == received);
		}
		else
		{
			LOGC("Server did not receive message");
		}
	}
	else
	{
		LOGC("Error sending message");
	}

	return false;
}

//=============================================================================
int main()
{
	int exitCode = 0;

	LoggerPtr spLogger = InitLogger();

	SocketManagerPtr spSocketManager = InitSocketManager(CHESSMATE_PORT);
	SocketPtr spSocket;

	if (spSocketManager)
	{
		spSocket = ConnectNewSocket(spSocketManager, "localhost", CHESSMATE_PORT);
	}
	else
	{
		LOGC("Could not create socket manager");
		exitCode = 1;
	}

	if (spSocket && spSocket->IsValid())
	{
		const unsigned int smallMessageLength = 128;
		const unsigned int largeMessageLength = 128 << 20;

		if (RunSendRecvTest(spSocketManager, spSocket, smallMessageLength))
		{
			LOGC("Small send/recv test passed");
		}
		else if (exitCode == 0)
		{
			LOGC("Small send/recv test failed");
			spSocket->Close();
			exitCode = 3;
		}

		if (RunSendRecvTest(spSocketManager, spSocket, largeMessageLength))
		{
			LOGC("Large send/recv test passed");
		}
		else if (exitCode == 0)
		{
			LOGC("Large send/recv test failed");
			spSocket->Close();
			exitCode = 4;
		}
	}
	else if (exitCode == 0)
	{
		LOGC("Could not connect new socket");
		exitCode = 2;
	}

	StopSocketManager(spSocketManager);

	// Make sure there are no lingering references to the socket manager
	long useCount = spSocketManager.use_count();

	if (useCount != 1)
	{
		LOGC("Too many references to socket manager: %ld", useCount);
		exitCode = 4;
	}

	spLogger->Flush();
	return exitCode;
}
