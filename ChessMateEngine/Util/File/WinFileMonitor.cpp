#include "WinFileMonitor.h"

#include <Util/Logging/Logger.h>
#include <Util/System/System.h>

namespace Util {

namespace
{
    static const DWORD s_accessFlags = (
        FILE_LIST_DIRECTORY
    );

    static const DWORD s_shareFlags = (
        FILE_SHARE_WRITE |
        FILE_SHARE_READ |
        FILE_SHARE_DELETE
    );

    static const DWORD s_dispositionFlags = (
        OPEN_EXISTING
    );

    static const DWORD s_attributeFlags = (
        FILE_FLAG_BACKUP_SEMANTICS |
        FILE_FLAG_OVERLAPPED
    );

    static const DWORD s_changeFlags = (
        FILE_NOTIFY_CHANGE_FILE_NAME |
        FILE_NOTIFY_CHANGE_DIR_NAME |
        FILE_NOTIFY_CHANGE_LAST_WRITE
    );

    static BYTE s_directoryBuffer[8 << 10];
}

//=============================================================================
FileMonitorImpl::FileMonitorImpl(
    FileEventCallback handler,
    const std::string &path,
    const std::string &file
) :
    FileMonitor(handler, path, file),
    m_monitorHandle(INVALID_HANDLE_VALUE)
{
    m_overlapped.hEvent = INVALID_HANDLE_VALUE;

    m_monitorHandle = ::CreateFile(
        m_path.c_str(), s_accessFlags, s_shareFlags, NULL, s_dispositionFlags,
        s_attributeFlags, NULL
    );

    if (m_monitorHandle == INVALID_HANDLE_VALUE)
    {
        LOGW(-1, "Could not initialize monitor for \"%s\": %s",
            m_path, Util::System::GetLastError()
        );
    }
    else
    {
        m_overlapped.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

        if (m_overlapped.hEvent == INVALID_HANDLE_VALUE)
        {
            LOGW(-1, "Could not create overlapped for \"%s\": %s",
                m_path, Util::System::GetLastError()
            );
        }
    }
}

//=============================================================================
FileMonitorImpl::~FileMonitorImpl()
{
    close();
}

//=============================================================================
bool FileMonitorImpl::IsValid() const
{
    return (m_overlapped.hEvent != INVALID_HANDLE_VALUE);
}

//=============================================================================
void FileMonitorImpl::Poll(const std::chrono::milliseconds &timeout)
{
    DWORD bytes = 0;

    BOOL success = ::ReadDirectoryChangesW(
        m_monitorHandle, &s_directoryBuffer, sizeof(s_directoryBuffer), FALSE,
        s_changeFlags, &bytes, &m_overlapped, NULL
    );

    if (success == TRUE)
    {
        DWORD millis = static_cast<DWORD>(timeout.count());

        success = ::GetOverlappedResultEx(
            m_monitorHandle, &m_overlapped, &bytes, millis, FALSE
        );

        if (success == TRUE)
        {
            handleEvents(s_directoryBuffer);
        }
        else
        {
            ::CancelIoEx(m_monitorHandle, &m_overlapped);

            int error = 0;
            std::string errorStr = Util::System::GetLastError(&error);

            if (error != WAIT_TIMEOUT)
            {
                LOGW(-1, "Could not read polled event for \"%s\": %s", m_path, errorStr);
                close();
            }
        }
    }
    else
    {
        LOGW(-1, "Could not create poller for \"%s\": %s",
            m_path, Util::System::GetLastError()
        );

        close();
    }
}

//=============================================================================
void FileMonitorImpl::handleEvents(PBYTE pBuffer)
{
    PFILE_NOTIFY_INFORMATION info = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(pBuffer);

    do {
        std::wstring wFileName(info->FileName, info->FileNameLength / sizeof(wchar_t));
        std::string fileName(wFileName.begin(), wFileName.end());

        if (fileName.compare(m_file) == 0)
        {
            FileMonitor::FileEvent type = FileMonitor::FILE_NO_CHANGE;

            switch (info->Action)
            {
            case FILE_ACTION_ADDED:
            case FILE_ACTION_RENAMED_NEW_NAME:
                type = FileMonitor::FILE_CREATED;
                break;

            case FILE_ACTION_REMOVED:
            case FILE_ACTION_RENAMED_OLD_NAME:
                type = FileMonitor::FILE_DELETED;
                break;

            case FILE_ACTION_MODIFIED:
                type = FileMonitor::FILE_CHANGED;
                break;
            }

            HandleEvent(type);
        }

        info = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(
            reinterpret_cast<PBYTE >(info) + info->NextEntryOffset
        );
    } while (info->NextEntryOffset > 0);
}

//=============================================================================
void FileMonitorImpl::close()
{
    if (m_monitorHandle != INVALID_HANDLE_VALUE)
    {
        if (m_overlapped.hEvent != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(m_overlapped.hEvent);
            m_overlapped.hEvent = INVALID_HANDLE_VALUE;
        }

        ::CloseHandle(m_monitorHandle);
        m_monitorHandle = INVALID_HANDLE_VALUE;
    }
}

}
