#include "WindowsMappedFile.h"

#include <Log/Log.h>

WindowsMappedFile::~WindowsMappedFile()
{
}

void WindowsMappedFile::Open(const std::string& path, FileAccess access)
{
    DWORD dwAccess = 0;
    DWORD dwShareMode = 0;
    DWORD dwFlags = 0;
    if(access & FileAccess::Read) {
        dwAccess |= GENERIC_READ;
        dwShareMode |= FILE_SHARE_READ;
        dwFlags |= OPEN_EXISTING;
    }
    if(access & FileAccess::Write) {
        dwAccess |= OPEN_EXISTING;
        dwShareMode = 0;
    }
    
    m_hFile = CreateFileA(path.c_str(), dwAccess, dwShareMode, nullptr, dwFlags, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (m_hFile == INVALID_HANDLE_VALUE) {
        LOG(Error, LogWindowsMappedFile, "Failed to create file handle");
        return;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(m_hFile, &fileSize)) {
        CloseHandle(m_hFile);
        return;
    }

    m_size = static_cast<size_t>(fileSize.QuadPart);
    if (m_size == 0) {
        CloseHandle(m_hFile);
        return;
    }

    DWORD protect = access & FileAccess::Write ? PAGE_READWRITE : PAGE_READONLY;
    m_hMapping = CreateFileMappingA(m_hFile, nullptr, protect, 0, 0, nullptr);
    if (!m_hMapping) {
        LOG(Error, LogWindowsMappedFile, "Failed to map file");
        CloseHandle(m_hFile);
        return;
    }

    DWORD desiredAccess = access & FileAccess::Write ? FILE_MAP_WRITE : FILE_MAP_READ;
    m_data = static_cast<std::byte*>(MapViewOfFile(m_hMapping, desiredAccess, 0, 0, 0));
    if (!m_data) {
        LOG(Error, LogWindowsMappedFile, "Failed to create a view into the file");
        CloseHandle(m_hMapping);
        CloseHandle(m_hFile);
        return;
    }
}

void WindowsMappedFile::Close()
{
    m_size = 0;
    m_data = nullptr;
    CloseHandle(m_hMapping);
    CloseHandle(m_hFile);
}

size_t WindowsMappedFile::WriteBytes(const std::span<const std::byte>& buffer, size_t offset)
{
    return size_t();
}

size_t WindowsMappedFile::ReadBytes(std::span<std::byte> &buffer, size_t offset) const
{
    return size_t();
}

size_t WindowsMappedFile::Resize(size_t size)
{
    return size_t();
}
