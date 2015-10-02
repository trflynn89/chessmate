#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <Util/Logging/Logger.h>
#include <Util/Socket/AsyncStructs.h>
#include <Util/Socket/SocketManager.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/String/String.h>

namespace
{
    //=========================================================================
    Util::LoggerPtr InitLogger()
    {
        Util::LoggerPtr spLogger = std::make_shared<Util::Logger>();
        Util::Logger::SetInstance(spLogger);

        return spLogger;
    }

    //=========================================================================
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

    //=========================================================================
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

    //=========================================================================
    void StopSocketManager(Util::SocketManagerPtr spSocketManager)
    {
        if (spSocketManager)
        {
            spSocketManager->StopSocketManager();
        }
    }

    //=========================================================================
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
}

//=============================================================================
class SocketTest : public ::testing::Test
{
protected:
    SocketTest() :
        m_spLogger(InitLogger()),
        m_spSocketManager(InitSocketManager(12390)),
        m_spSocket(ConnectNewSocket(m_spSocketManager, "localhost", 12390))
    {
    }

    virtual ~SocketTest()
    {
        StopSocketManager(m_spSocketManager);
        m_spLogger->Flush();
    }

    //=========================================================================
    void RunSendRecvTest(unsigned int messageLength)
    {
        ASSERT_TRUE(m_spSocket && m_spSocket->IsValid());

        LOGC("Generating message of length %u", messageLength);
        const std::string message = Util::String::GenerateRandomString(messageLength);

        LOGC("Client sending message of length %u", message.length());
        auto startTime = std::chrono::high_resolution_clock::now();

        ASSERT_TRUE(m_spSocket->SendAsync(message));

        Util::AsyncRequest request;
        std::chrono::seconds waitTime(120);
        ASSERT_TRUE(m_spSocketManager->WaitForCompletedReceive(request, waitTime));
         
        auto endTime = std::chrono::high_resolution_clock::now();

        std::string received = request.GetRequest();
        std::string::size_type bytesReceived = received.length();

        auto recvTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
        double throughput = static_cast<double>(bytesReceived) / 1024.0 / recvTime.count();

        LOGC("Server received message of length %u, average throughput of %f KBps",
            bytesReceived, throughput);

        ASSERT_EQ(message, received);
    }

    Util::LoggerPtr m_spLogger;
    Util::SocketManagerPtr m_spSocketManager;
    Util::SocketPtr m_spSocket;
};

//=============================================================================
TEST_F(SocketTest, SmallSendRecvTest)
{
    RunSendRecvTest(128);
}

//=============================================================================
TEST_F(SocketTest, LargeSendRecvTest)
{
    RunSendRecvTest(128 << 10);
}
