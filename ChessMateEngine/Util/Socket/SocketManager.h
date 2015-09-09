#pragma once

#include <atomic>
#include <chrono>
#include <future>
#include <memory>
#include <mutex>
#include <vector>

#include <Util/Utilities.h>
#include <Util/Socket/AsyncStructs.h>
#include <Util/Socket/Socket.h>

namespace Util {

DEFINE_CLASS_PTRS(Socket);
DEFINE_CLASS_PTRS(SocketManager);

typedef std::function<void(SocketPtr)> NewClientCallback;
typedef std::function<void(int)> ClosedClientCallback;

/**
 * Class to manage the creation of sockets and IO operations over asynchronous
 * sockets. A single thread is created to perform all IO. Completed IO is
 * pushed onto queues, which other threads may read from.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
class SocketManager : public std::enable_shared_from_this<SocketManager>
{
public:
    /**
     * Constructor.
     */
    SocketManager();

    /**
     * Default destructor.
     */
    virtual ~SocketManager();

    /**
     * Start the thread which monitors asynchronous socket IO.
     */
    void StartSocketManager();

    /**
     * Stop the socket manager. Stop the asynchronous IO thread and close all
     * sockets.
     */
    void StopSocketManager();

    /**
     * Set callbacks for when a client connects or disconnects.
     *
     * @param NewClientCallback Callback for when a new client connects.
     * @param ClosedClientCallback Callback for when a client disconnects.
     */
    void SetClientCallbacks(NewClientCallback, ClosedClientCallback);

    /**
     * Remove the callbacks for when a client connects or disconnects.
     */
    void ClearClientCallbacks();

    /**
     * Create and initialize a synchronous TCP socket.
     *
     * @return Shared pointer to the socket.
     */
    SocketPtr CreateTcpSocket();

    /**
     * Create and initialize an asynchronous TCP socket.
     *
     * @return Shared pointer to the socket.
     */
    SocketPtr CreateAsyncTcpSocket();

    /**
     * Create and initialize a synchronous UDP socket.
     *
     * @return Shared pointer to the socket.
     */
    SocketPtr CreateUdpSocket();

    /**
     * Create and initialize an asynchronous UDP socket.
     *
     * @return Shared pointer to the socket.
     */
    SocketPtr CreateAsyncUdpSocket();

    /**
     * Wait for an asynchronous connect to complete.
     *
     * @param AsyncConnect Structure to store the completion.
     * @param duration Time to wait for a completion.
     *
     * @return True if a completed connect was found in the given duration.
     */
    template <typename R, typename P>
    bool WaitForCompletedConnect(AsyncConnect &, std::chrono::duration<R, P>);

    /**
     * Wait for an asynchronous read to complete.
     *
     * @param AsyncConnect Structure to store the completion.
     * @param duration Time to wait for a completion.
     *
     * @return True if a completed receive was found in the given duration.
     */
    template <typename R, typename P>
    bool WaitForCompletedReceive(AsyncRequest &, std::chrono::duration<R, P>);

    /**
     * Wait for an asynchronous send to complete.
     *
     * @param AsyncConnect Structure to store the completion.
     * @param duration Time to wait for a completion.
     *
     * @return True if a completed send was found in the given duration.
     */
    template <typename R, typename P>
    bool WaitForCompletedSend(AsyncRequest &, std::chrono::duration<R, P>);

protected:
    /**
     * Thread to monitor asynchronous sockets. Virtual to allow subclasses to
     * override this thread's behavior.
     */
    virtual void AsyncIoThread() = 0;

    std::atomic_bool m_aKeepRunning;

    std::mutex m_callbackMutex;
    NewClientCallback m_newClientCallback;
    ClosedClientCallback m_closedClientCallback;

    std::future<void> m_serviceFuture;

    std::vector<SocketPtr> m_aioSockets;
    std::mutex m_aioSocketsMutex;

    AsyncConnect::ConnectQueue m_completedConnects;
    AsyncRequest::RequestQueue m_completedReceives;
    AsyncRequest::RequestQueue m_completedSends;
};

//=============================================================================
template <typename R, typename P>
bool SocketManager::WaitForCompletedConnect(
    AsyncConnect &connect,
    std::chrono::duration<R, P> waitTime
)
{
    return m_completedConnects.Pop(connect, waitTime);
}

//=============================================================================
template <typename R, typename P>
bool SocketManager::WaitForCompletedReceive(
    AsyncRequest &request,
    std::chrono::duration<R, P> waitTime
)
{
    return m_completedReceives.Pop(request, waitTime);
}

//=============================================================================
template <typename R, typename P>
bool SocketManager::WaitForCompletedSend(
    AsyncRequest &request,
    std::chrono::duration<R, P> waitTime
)
{
    return m_completedSends.Pop(request, waitTime);
}

}
