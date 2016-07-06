#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <Util/Utilities.h>

namespace Util {

DEFINE_CLASS_PTRS(FileMonitor);

/**
 * Virtual interface to monitor a local file. This interface is platform
 * independent - OS dependent implementations should inherit from this class.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version July 4, 2016
 */
class FileMonitor : public std::enable_shared_from_this<FileMonitor>
{
public:
    /**
     * Enumerated list of file events.
     */
    enum FileEvent
    {
        FILE_NO_CHANGE,
        FILE_CREATED,
        FILE_DELETED,
        FILE_CHANGED
    };

    /**
     * Callback definition for function to be triggered on a file change.
     */
    typedef std::function<void(FileEvent)> FileEventCallback;

    /**
     * Constructor.
     *
     * @param FileEventCallback Callback to trigger when the file changes.
     * @param string Directory containing the file to monitor.
     * @param string Name of the file to monitor.
     */
    FileMonitor(FileEventCallback, const std::string &, const std::string &);

    /**
     * Destructor. Stop the file monitor thread if necessary.
     */
    virtual ~FileMonitor();

    /**
     * Start the file monitor thread.
     *
     * @return True if the monitor could be started.
     */
    bool StartMonitor();

    /**
     * Stop the file monitor thread.
     */
    void StopMonitor();

protected:
    /**
     * Check if the monitor implementation is in a good state.
     *
     * @return True if the monitor is healthy.
     */
    virtual bool IsValid() const = 0;

    /**
     * Poll for any file changes.
     *
     * @param milliseconds Max time to poll for file changes.
     */
    virtual void Poll(const std::chrono::milliseconds &) = 0;

    /**
     * Trigger the registered callback for a file change.
     *
     * @param FileEvent The file change event.
     */
    void HandleEvent(FileEvent);

    const std::string m_path;
    const std::string m_file;

private:
    /**
     * Thread in which the file is polled and callbacks are triggered.
     */
    void monitorThread();

    std::atomic_bool m_aKeepRunning;
    std::future<void> m_future;

    std::mutex m_callbackMutex;
    FileEventCallback m_handler;
};

}
