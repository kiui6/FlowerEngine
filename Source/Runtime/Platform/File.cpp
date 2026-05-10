#include "File.h"

#include <Log/Log.h>

File::~File()
{
}

void File::Open(std::string path, FileAccess access)
{
    // TODO: Construct io mode from FileAccess
    m_stream = SDL_IOFromFile(path.c_str(), "rb");
    if (!m_stream) {
        LOGF(Error, LogFile, "Failed to open file: %s", path.c_str());
        return;
    }

    m_size = SDL_GetIOSize(m_stream);

    m_data = new std::byte[m_size];

    Sint64 bytesRead = SDL_ReadIO(m_stream, m_data, m_size);
    if (bytesRead != m_size) {
        delete m_data;
        m_data = nullptr;
        LOG(Error, LogFile, "Partial read");
    }
}

void File::Close()
{
    SDL_CloseIO(m_stream);
}

size_t File::WriteBytes(std::span<const std::byte> buffer, size_t offset)
{
    return size_t();
}

size_t File::ReadBytes(std::span<std::byte> &buffer, size_t offset) const
{
    return size_t();
}

size_t File::Resize(size_t size)
{
    return size_t();
}
