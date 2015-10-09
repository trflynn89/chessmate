#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include <Util/Logging/Logger.h>
#include <Util/Socket/AsyncStructs.h>
#include <Util/Socket/SocketManager.h>
#include <Util/Socket/SocketManagerImpl.h>
#include <Util/String/String.h>

//=============================================================================
class SocketTest : public ::testing::Test
{
public:
    SocketTest() :
        m_spServerSocketManager(std::make_shared<Util::SocketManagerImpl>()),
        m_spClientSocketManager(std::make_shared<Util::SocketManagerImpl>()),
        m_message(Util::String::GenerateRandomString(128 << 10)),
        m_host("localhost"),
        m_port(12390)
    {
    }

    /**
     * Thread to run server functions do handle accepting a client socket and
     * receiving data from it.
     */
    void ServerThread(bool doAsync)
    {
        Util::SocketPtr spAcceptSocket = CreateSocket(m_spServerSocketManager, doAsync);
        ASSERT_TRUE(spAcceptSocket && spAcceptSocket->IsValid());
        ASSERT_EQ(spAcceptSocket->IsAsync(), doAsync);

        ASSERT_TRUE(spAcceptSocket->BindForReuse(Util::Socket::InAddrAny(), m_port));
        ASSERT_TRUE(spAcceptSocket->Listen());

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
        Util::SocketPtr spSendSocket = CreateSocket(m_spClientSocketManager, doAsync);
        ASSERT_TRUE(spSendSocket && spSendSocket->IsValid());
        ASSERT_EQ(spSendSocket->IsAsync(), doAsync);

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
    Util::SocketPtr CreateSocket(const Util::SocketManagerPtr &spSocketManager, bool doAsync)
    {
        Util::SocketPtr spSocket;

        if (doAsync)
        {
            Util::SocketWPtr wpSocket = spSocketManager->CreateAsyncTcpSocket();
            spSocket = wpSocket.lock();
        }
        else
        {
            spSocket = spSocketManager->CreateTcpSocket();
        }

        return spSocket;
    }

    Util::SocketManagerPtr m_spServerSocketManager;
    Util::SocketManagerPtr m_spClientSocketManager;

    std::string m_message;
    std::string m_host;
    int m_port;
};

/**
 * Test a synchronous server with a synchronous client.
 */
TEST_F(SocketTest, SyncServer_SyncClient_Test)
{
    auto server = std::async(std::launch::async, &SocketTest::ServerThread, this, false);
    auto client = std::async(std::launch::async, &SocketTest::ClientThread, this, false);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test an asynchronous server with a synchronous client.
 */
TEST_F(SocketTest, AsyncServer_SyncClient_Test)
{
    auto server = std::async(std::launch::async, &SocketTest::ServerThread, this, true);
    auto client = std::async(std::launch::async, &SocketTest::ClientThread, this, false);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test a synchronous server with an asynchronous client.
 */
TEST_F(SocketTest, SyncServer_AsyncClient_Test)
{
    auto server = std::async(std::launch::async, &SocketTest::ServerThread, this, false);
    auto client = std::async(std::launch::async, &SocketTest::ClientThread, this, true);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}

/**
 * Test an asynchronous server with an asynchronous client.
 */
TEST_F(SocketTest, AsyncServer_AsyncClient_Test)
{
    auto server = std::async(std::launch::async, &SocketTest::ServerThread, this, true);
    auto client = std::async(std::launch::async, &SocketTest::ClientThread, this, true);

    ASSERT_TRUE(server.valid() && client.valid());
    client.get(); server.get();
}
