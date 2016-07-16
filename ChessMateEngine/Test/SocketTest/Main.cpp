#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include <Util/Concurrency/ConcurrentQueue.h>
#include <Util/Logging/Logger.h>
#include <Util/Socket/AsyncStructs.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/String/String.h>

//==============================================================================
class SocketTest : public ::testing::Test
{
public:
    SocketTest() :
        m_spServerSocketManager(std::make_shared<Util::SocketManagerImpl>()),
        m_spClientSocketManager(std::make_shared<Util::SocketManagerImpl>()),
        m_message(Util::String::GenerateRandomString(64 << 10)),
        m_host("localhost"),
        m_port(12390)
    {
    }

    virtual void ServerThread(bool doAsync) = 0;
    virtual void ClientThread(bool doAsync) = 0;

protected:
    /**
     * Start the socket managers.
     */
    void SetUp()
    {
        m_spServerSocketManager->StartSocketManager();
        m_spClientSocketManager->StartSocketManager();
    }

    /**
     * Stop the socket managers.
     */
    void TearDown()
    {
        m_spClientSocketManager->StopSocketManager();
        m_spServerSocketManager->StopSocketManager();
    }

    /**
     * Create either a synchronous or an asynchronous socket.
     */
    Util::SocketPtr CreateSocket(
        const Util::SocketManagerPtr &spSocketManager,
        bool doAsync,
        bool isTcp
    )
    {
        Util::SocketPtr spSocket;

        if (doAsync)
        {
            Util::SocketWPtr wpSocket = (isTcp ? spSocketManager->CreateAsyncTcpSocket() : spSocketManager->CreateAsyncUdpSocket());
            spSocket = wpSocket.lock();
        }
        else
        {
            spSocket = (isTcp ? spSocketManager->CreateTcpSocket() : spSocketManager->CreateUdpSocket());
        }

        return spSocket;
    }

    Util::SocketManagerPtr m_spServerSocketManager;
    Util::SocketManagerPtr m_spClientSocketManager;

    Util::ConcurrentQueue<int> m_eventQueue;

    std::string m_message;
    std::string m_host;
    int m_port;
};

//==============================================================================
class TcpSocketTest : public SocketTest
{
public:
    TcpSocketTest() : SocketTest()
    {
    }

    /**
     * Thread to run server functions do handle accepting a client socket and
     * receiving data from it.
     */
    void ServerThread(bool doAsync)
    {
        Util::SocketPtr spAcceptSocket = CreateSocket(m_spServerSocketManager, doAsync, true);
        ASSERT_TRUE(spAcceptSocket && spAcceptSocket->IsValid());
        ASSERT_EQ(spAcceptSocket->IsAsync(), doAsync);

        ASSERT_TRUE(spAcceptSocket->BindForReuse(Util::Socket::InAddrAny(), m_port));
        ASSERT_TRUE(spAcceptSocket->Listen());
        m_eventQueue.Push(1);

        if (doAsync)
        {
            Util::AsyncRequest request;
            std::chrono::seconds waitTime(120);

            ASSERT_TRUE(m_spServerSocketManager->WaitForCompletedReceive(request, waitTime));
            ASSERT_EQ(m_message, request.GetRequest());
        }
        else
        {
            Util::SocketPtr spRecvSocket = spAcceptSocket->Accept();
            ASSERT_EQ(spRecvSocket->Recv(), m_message);
        }
    }

    /**
     * Thread to run client functions to connect to the server socket and send
     * data to it.
     */
    void ClientThread(bool doAsync)
    {
        Util::SocketPtr spSendSocket = CreateSocket(m_spClientSocketManager, doAsync, true);
        ASSERT_TRUE(spSendSocket && spSendSocket->IsValid());
        ASSERT_EQ(spSendSocket->IsAsync(), doAsync);

        int item = 0;
        std::chrono::seconds waitTime(120);
        m_eventQueue.Pop(item, waitTime);

        if (doAsync)
        {
            Util::Socket::ConnectedState state = spSendSocket->ConnectAsync(m_host, m_port);
            ASSERT_NE(state, Util::Socket::NOT_CONNECTED);

            if (state == Util::Socket::CONNECTING)
            {
                Util::AsyncConnect connect;
                std::chrono::seconds waitTime(1);
                ASSERT_TRUE(m_spClientSocketManager->WaitForCompletedConnect(connect, waitTime));
            }

            Util::AsyncRequest request;
            std::chrono::seconds waitTime(120);

            ASSERT_TRUE(spSendSocket->SendAsync(m_message));
            ASSERT_TRUE(m_spClientSocketManager->WaitForCompletedSend(request, waitTime));
            ASSERT_EQ(m_message, request.GetRequest());
        }
        else
        {
            ASSERT_TRUE(spSendSocket->Connect(m_host, m_port));
            ASSERT_EQ(spSendSocket->Send(m_message), m_message.length());
        }
    }
};

//==============================================================================
class UdpSocketTest : public SocketTest
{
public:
    UdpSocketTest() : SocketTest()
    {
    }

    /**
     * Thread to run server functions do handle accepting a client socket and
     * receiving data from it.
     */
    void ServerThread(bool doAsync)
    {
        Util::SocketPtr spRecvSocket = CreateSocket(m_spServerSocketManager, doAsync, false);
        ASSERT_TRUE(spRecvSocket && spRecvSocket->IsValid());
        ASSERT_EQ(spRecvSocket->IsAsync(), doAsync);

        ASSERT_TRUE(spRecvSocket->BindForReuse(Util::Socket::InAddrAny(), m_port));
        m_eventQueue.Push(1);

        if (doAsync)
        {
            Util::AsyncRequest request;
            std::chrono::seconds waitTime(120);

            ASSERT_TRUE(m_spServerSocketManager->WaitForCompletedReceive(request, waitTime));
            ASSERT_EQ(m_message, request.GetRequest());
        }
        else
        {
            ASSERT_EQ(spRecvSocket->RecvFrom(), m_message);
        }
    }

    /**
     * Thread to run client functions to connect to the server socket and send
     * data to it.
     */
    void ClientThread(bool doAsync)
    {
        Util::SocketPtr spSendSocket = CreateSocket(m_spClientSocketManager, doAsync, false);
        ASSERT_TRUE(spSendSocket && spSendSocket->IsValid());
        ASSERT_EQ(spSendSocket->IsAsync(), doAsync);

        int item = 0;
        std::chrono::seconds waitTime(120);
        m_eventQueue.Pop(item, waitTime);

        if (doAsync)
        {
            Util::AsyncRequest request;
            std::chrono::seconds waitTime(120);

            ASSERT_TRUE(spSendSocket->SendToAsync(m_message, m_host, m_port));
            ASSERT_TRUE(m_spClientSocketManager->WaitForCompletedSend(request, waitTime));
            ASSERT_EQ(m_message, request.GetRequest());
        }
        else
        {
            ASSERT_EQ(spSendSocket->SendTo(m_message, m_host, m_port), m_message.length());
        }
    }
};

/**
 * Test a synchronous server with a synchronous client.
 */
TEST_F(TcpSocketTest, SyncServer_SyncClient_Test)
{
    auto server = std::async(std::launch::async, &TcpSocketTest::ServerThread, this, false);
    auto client = std::async(std::launch::async, &TcpSocketTest::ClientThread, this, false);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test an asynchronous server with a synchronous client.
 */
TEST_F(TcpSocketTest, AsyncServer_SyncClient_Test)
{
    auto server = std::async(std::launch::async, &TcpSocketTest::ServerThread, this, true);
    auto client = std::async(std::launch::async, &TcpSocketTest::ClientThread, this, false);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test a synchronous server with an asynchronous client.
 */
TEST_F(TcpSocketTest, SyncServer_AsyncClient_Test)
{
    auto server = std::async(std::launch::async, &TcpSocketTest::ServerThread, this, false);
    auto client = std::async(std::launch::async, &TcpSocketTest::ClientThread, this, true);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test an asynchronous server with an asynchronous client.
 */
TEST_F(TcpSocketTest, AsyncServer_AsyncClient_Test)
{
    auto server = std::async(std::launch::async, &TcpSocketTest::ServerThread, this, true);
    auto client = std::async(std::launch::async, &TcpSocketTest::ClientThread, this, true);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test a synchronous server with a synchronous client.
 */
TEST_F(UdpSocketTest, SyncServer_SyncClient_Test)
{
    auto server = std::async(std::launch::async, &UdpSocketTest::ServerThread, this, false);
    auto client = std::async(std::launch::async, &UdpSocketTest::ClientThread, this, false);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test an asynchronous server with a synchronous client.
 */
TEST_F(UdpSocketTest, AsyncServer_SyncClient_Test)
{
    auto server = std::async(std::launch::async, &UdpSocketTest::ServerThread, this, true);
    auto client = std::async(std::launch::async, &UdpSocketTest::ClientThread, this, false);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test a synchronous server with an asynchronous client.
 */
TEST_F(UdpSocketTest, SyncServer_AsyncClient_Test)
{
    auto server = std::async(std::launch::async, &UdpSocketTest::ServerThread, this, false);
    auto client = std::async(std::launch::async, &UdpSocketTest::ClientThread, this, true);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test an asynchronous server with an asynchronous client.
 */
TEST_F(UdpSocketTest, AsyncServer_AsyncClient_Test)
{
    auto server = std::async(std::launch::async, &UdpSocketTest::ServerThread, this, true);
    auto client = std::async(std::launch::async, &UdpSocketTest::ClientThread, this, true);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}
