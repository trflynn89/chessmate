#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace Util {

/**
 * Wrapper around a std::queue to provide thread safe access.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version November 26, 2014
 */
template <typename T>
class ConcurrentQueue
{
public:
    typedef typename std::queue<T>::size_type size_type;

    /**
     * Pop an item from the queue. If the queue is empty, wait indefinitely for
     * an item to be available.
     *
     * @param T Reference to an object of type T where the item will be stored.
     */
    void Pop(T &);

    /**
     * Pop an item from the queue. If the queue is empty, wait (at most) for
     * the specified amount of time for an item to be available.
     *
     * @param T Reference to an object of type T where the item will be stored.
     * @param duration The amount of time to wait.
     *
     * @return True if an object was popped in the given duration.
     */
    template <typename R, typename P>
    bool Pop(T &, std::chrono::duration<R, P>);

    /**
     * Push an item onto the queue.
     *
     * @param T Reference to an object of type T to push onto the queue.
     */
    void Push(const T &);

    /**
     * @return True if the queue is empty, false otherwise.
     */
    bool IsEmpty();

    /**
     * @return The number of items in the queue.
     */
    size_type Size();

private:
    std::queue<T> m_queue;
    std::mutex m_queueMutex;

    std::condition_variable m_pushCondition;
};

//=============================================================================
template <typename T>
void ConcurrentQueue<T>::Pop(T &item)
{
    std::unique_lock<std::mutex> lock(m_queueMutex);

    while (m_queue.empty())
    {
        m_pushCondition.wait(lock);
    }

    item = std::move(m_queue.front());
    m_queue.pop();
}

//=============================================================================
template <typename T> template <typename R, typename P>
bool ConcurrentQueue<T>::Pop(T &item, std::chrono::duration<R, P> waitTime)
{
    std::unique_lock<std::mutex> lock(m_queueMutex);

    auto emptyTest = [&] { return !m_queue.empty(); };
    bool itemPopped = false;

    if (m_pushCondition.wait_for(lock, waitTime, emptyTest))
    {
        item = std::move(m_queue.front());
        m_queue.pop();

        itemPopped = true;
    }

    return itemPopped;
}

//=============================================================================
template <typename T>
void ConcurrentQueue<T>::Push(const T &item)
{
    std::unique_lock<std::mutex> lock(m_queueMutex);

    m_queue.push(std::move(item));
    lock.unlock();

    m_pushCondition.notify_one();
}

//=============================================================================
template <typename T>
bool ConcurrentQueue<T>::IsEmpty()
{
    std::unique_lock<std::mutex> lock(m_queueMutex);
    return m_queue.empty();
}

//=============================================================================
template <typename T>
typename ConcurrentQueue<T>::size_type ConcurrentQueue<T>::Size()
{
    std::unique_lock<std::mutex> lock(m_queueMutex);
    return m_queue.size();
}

}
