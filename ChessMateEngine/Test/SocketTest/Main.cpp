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

//=============================================================================
class SocketTest : public ::testing::Test
{
protected:
    SocketTest() :
        m_spServerSocketManager(std::make_shared<Util::SocketManagerImpl>()),
        m_spClientSocketManager(std::make_shared<Util::SocketManagerImpl>()),
        m_wpClientSocket(m_spClientSocketManager->CreateAsyncTcpSocket())
    {
    }

    /**
     * Start the socket managers, create an accept socket (to mimic the server
     * side), and connect the send socket (to mimic the client side).
     */
    void SetUp()
    {
        m_spServerSocketManager->StartSocketManager();
        m_spClientSocketManager->StartSocketManager();

        createAcceptSocket(12390);
        connectSendSocket("localhost", 12390);
    }

    /**
     * Stop the socket managers and flush the logger.
     */
    void TearDown()
    {
        m_spClientSocketManager->StopSocketManager();
        m_spServerSocketManager->StopSocketManager();
    }

    /**
     * Send a message with the given length across the previously connected
     * socket. Ensure the message received is the same as the message sent.
     */
    void RunSendRecvTest(unsigned int messageLength)
    {
        Util::SocketPtr spSocket = m_wpClientSocket.lock();
        ASSERT_TRUE(spSocket && spSocket->IsValid());

        LOGC("Generating message of length %u", messageLength);
        const std::string message = Util::String::GenerateRandomString(messageLength);

        LOGC("Client sending message of length %u", message.length());
        auto startTime = std::chrono::high_resolution_clock::now();

        ASSERT_TRUE(spSocket->SendAsync(message));

        Util::AsyncRequest request;
        std::chrono::seconds waitTime(120);
        ASSERT_TRUE(m_spServerSocketManager->WaitForCompletedReceive(request, waitTime));
         
        auto endTime = std::chrono::high_resolution_clock::now();

        std::string received = request.GetRequest();
        double bytesReceived = static_cast<double>(received.length());

        typedef std::chrono::duration<double> dms;
        auto recvTime = std::chrono::duration_cast<dms>(endTime - startTime);
        double throughput = bytesReceived / 1024.0 / recvTime.count();

        LOGC("Server received message of length %u, average throughput of %f KBps",
            bytesReceived, throughput);

        ASSERT_EQ(message, received);
    }

private:
    /**
     * Create an asynchronous TCP socket to be used as the socket manager's
     * accept socket.
     */
    void createAcceptSocket(int acceptPort)
    {
        Util::SocketWPtr wpSocket = m_spServerSocketManager->CreateAsyncTcpSocket();
        Util::SocketPtr spSocket = wpSocket.lock();

        if (spSocket)
        {
            ASSERT_TRUE(spSocket->BindForReuse(Util::Socket::InAddrAny(), acceptPort));
            ASSERT_TRUE(spSocket->Listen());
        }

        ASSERT_TRUE(spSocket && spSocket->IsListening());
    }

    /**
     * Aynchnously connect the TCP socket to be used for sending messages.
     */
    void connectSendSocket(std::string host, int port)
    {
        Util::SocketPtr spSocket = m_wpClientSocket.lock();
        ASSERT_TRUE(spSocket && spSocket->IsValid());

        Util::Socket::ConnectedState state = spSocket->ConnectAsync(host, port);
        ASSERT_NE(state, Util::Socket::NOT_CONNECTED);

        if (state == Util::Socket::CONNECTING)
        {
            Util::AsyncConnect connect;
            std::chrono::seconds waitTime(1);

            ASSERT_TRUE(m_spClientSocketManager->WaitForCompletedConnect(connect, waitTime));
            ASSERT_TRUE(spSocket->IsConnected());
        }
    }

    Util::SocketManagerPtr m_spServerSocketManager;
    Util::SocketManagerPtr m_spClientSocketManager;
    Util::SocketWPtr m_wpClientSocket;
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
