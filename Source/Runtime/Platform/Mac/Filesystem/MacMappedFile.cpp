#include "MacMappedFile.h"

#include <Log/Log.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

MacMappedFile::~MacMappedFile()
{
}

void MacMappedFile::Open(const std::string& path, FileAccess access)
{
    int fileFlags = 0;
    int protFlags = 0;
    int mapFlags = MAP_PRIVATE;
    if(access & FileAccess::Read) {
        fileFlags = O_RDONLY;
        protFlags |= PROT_READ;
    }
    if(access & FileAccess::Write) {
        fileFlags = O_RDWR;
        protFlags |= PROT_WRITE;
        mapFlags = MAP_SHARED;
    }

    m_fd = open(path.c_str(), fileFlags);
    if (m_fd == -1) {
        LOG(Error, LogMacMappedFile, "Failed to create file handle");
        return;
    }

    // Get file size
    struct stat st;
    if (fstat(m_fd, &st) == -1) {
        LOG(Error, LogMacMappedFile, "Failed to get file stats");
        close(m_fd);
        return;
    }

    m_size = st.st_size;

    m_data = static_cast<std::byte*>(mmap(nullptr, m_size, protFlags, mapFlags, m_fd, 0));
    if (m_data == MAP_FAILED) {
        LOG(Error, LogMacMappedFile, "Failed to memory map file!");
        close(m_fd);
        return;
    }
}

void MacMappedFile::Close()
{
    munmap(m_data, m_size);
    close(m_fd);

    m_size = 0;
    m_data = nullptr;
}

size_t MacMappedFile::WriteBytes(const std::span<const std::byte>& buffer, size_t offset)
{
    return size_t();
}

size_t MacMappedFile::ReadBytes(std::span<std::byte> &buffer, size_t offset) const
{
    return size_t();
}

size_t MacMappedFile::Resize(size_t size)
{
    return size_t();
}
