#include "WindowsMappedFile.h"

WindowsMappedFile::~WindowsMappedFile()
{
}

void WindowsMappedFile::Open(std::string path, FileAccess access)
{
}

void WindowsMappedFile::Close()
{
}

size_t WindowsMappedFile::WriteBytes(std::span<const std::byte> buffer, size_t offset)
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
