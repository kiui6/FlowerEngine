#pragma once

#include <memory>
#include <vector>
#include <cassert>
#include <optional>
#include <type_traits>
#include <cstring>
#include <bit>

#include <Platform/Abstract/Filesystem/FileBase.h>

class DataWriter {
    std::shared_ptr<FileBase> m_file = nullptr;
    std::vector<std::byte> m_buffer;
    size_t m_cursor = 0;
public:
    DataWriter() {}
    // TODO: Decouple from external IO, DataWriter is an in-memory interface
    DataWriter(std::shared_ptr<FileBase> file) : m_file(file) {}

    inline void Advance(ptrdiff_t diff) { 
        if (diff == 0) return;

        if (diff < 0) {
            size_t absDiff = static_cast<size_t>(-diff);
            if (m_cursor <= absDiff) {
                m_cursor = 0;
            } else {
                m_cursor -= absDiff;
            }
            return;
        }

        size_t newPos = m_cursor + static_cast<size_t>(diff);
        if (m_buffer.size() < newPos) {
            m_buffer.resize(newPos);
        }
        m_cursor = newPos;
    }

    inline void SetOffset(size_t offset) {m_cursor = offset;}

    inline std::byte* Data() {return m_buffer.data();}
    inline size_t Size() const {return m_buffer.size();}

    void WriteByte(std::byte val);
    void WriteBytes(std::byte* val, size_t size);

    void WriteString(const std::string& string);

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    void Write(const T& data) {
        constexpr size_t insertSize = sizeof(T);
        
        size_t newPos = m_cursor + insertSize;
        if(m_buffer.size() < newPos) {
            m_buffer.resize(newPos);
        }

        memcpy(m_buffer.data() + m_cursor, &data, insertSize);

        m_cursor = newPos;
    }

    void Clear() {m_buffer.clear();}
};