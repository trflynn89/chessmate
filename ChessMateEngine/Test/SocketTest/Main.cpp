#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include <Util/Logging/Logger.h>
#include <Util/Socket/AsyncStructs.h>
#include <Util/Socket/SocketManager.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/String/String.h>

//=============================================================================
Util::LoggerPtr InitLogger()
{
    Util::LoggerPtr spLogger = std::make_shared<Util::Logger>();
    Util::Logger::SetInstance(spLogger);

    return spLogger;
}

//=============================================================================
bool CreateAcceptSocket(Util::SocketManagerPtr spSocketManager, int acceptPort)
{
    Util::SocketPtr spSocket = spSocketManager->CreateAsyncTcpSocket();

    if (spSocket)
    {
        if (!spSocket->BindForReuse(Util::Socket::InAddrAny(), acceptPort))
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
Util::SocketManagerPtr InitSocketManager(int acceptPort)
{
    Util::SocketManagerPtr spSocketManager = std::make_shared<Util::SocketManagerImpl>();
    spSocketManager->StartSocketManager();

    if (!CreateAcceptSocket(spSocketManager, acceptPort))
    {
        spSocketManager.reset();
    }

    return spSocketManager;
}

//=============================================================================
void StopSocketManager(Util::SocketManagerPtr spSocketManager)
{
    if (spSocketManager)
    {
        spSocketManager->StopSocketManager();
    }
}

//=============================================================================
Util::SocketPtr ConnectNewSocket(Util::SocketManagerPtr spSocketManager, std::string host, int port)
{
    Util::SocketPtr spSocket = spSocketManager->CreateAsyncTcpSocket();

    if (spSocket)
    {
        Util::Socket::ConnectedState state = spSocket->ConnectAsync(host, port);

        if (state == Util::Socket::CONNECTED)
        {
            LOGC("Connected immediately to %s:%d", host, port);
        }
        else if (state == Util::Socket::CONNECTING)
        {
            Util::AsyncConnect connect;
            std::chrono::seconds waitTime(1);

            if (spSocketManager->WaitForCompletedConnect(connect, waitTime))
            {
                if (spSocket->IsConnected())
                {
                    LOGC("Connected asynchronously to %s:%d", host, port);
                }
                else
                {
                    LOGC("Async connect to %s:%d was valid, but not connected", host, port);
                    spSocket.reset();
                }
            }
            else
            {
                LOGC("Not connected to %s:%d", host, port);
                spSocket.reset();
            }
        }
        else
        {
            LOGC("Connect to %s:%d failed", host, port);
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
bool RunSendRecvTest(Util::SocketManagerPtr spSocketManager, Util::SocketPtr spSocket, unsigned int messageLength)
{
    if (!spSocket->IsValid())
    {
        LOGC("Socket has been closed");
        return false;
    }

    LOGC("Generating message of length %u", messageLength);
    const std::string message = Util::String::GenerateRandomString(messageLength);

    LOGC("Client sending message of length %u", message.length());
    auto startTime = std::chrono::high_resolution_clock::now();

    if (spSocket->SendAsync(message))
    {
        Util::AsyncRequest request;
        std::chrono::seconds waitTime(120);

        if (spSocketManager->WaitForCompletedReceive(request, waitTime))
        {
            auto endTime = std::chrono::high_resolution_clock::now();

            std::string received = request.GetRequest();
            std::string::size_type bytesReceived = received.length();

            auto recvTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
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

    Util::LoggerPtr spLogger = InitLogger();

    Util::SocketManagerPtr spSocketManager = InitSocketManager(12390);
    Util::SocketPtr spSocket;

    if (spSocketManager)
    {
        spSocket = ConnectNewSocket(spSocketManager, "localhost", 12390);
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
        exitCode = 5;
    }

    spLogger->Flush();
    return exitCode;
}
