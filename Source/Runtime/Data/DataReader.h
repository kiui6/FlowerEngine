#pragma once

#include "DataView.h"

#include <optional>
#include <type_traits>

class DataReader {
    DataView& m_view;
    size_t m_cursor = 0;
public:
    DataReader(DataView& view) : m_view(view) {}

    inline void Advance(ptrdiff_t diff) {m_cursor += diff;}
    inline void SetOffset(size_t offset) {m_cursor = offset;}

    inline size_t GetSize() {return m_view.size();}
    inline size_t GetCursor() {return m_cursor;}

    std::optional<std::byte> ReadByte(bool advance = true);
    std::optional<std::byte*> ReadBytes(size_t length, bool advance = true);

    std::optional<int16_t> ReadInt16(bool advance = true);
    std::optional<int32_t> ReadInt32(bool advance = true);
    std::optional<int64_t> ReadInt64(bool advance = true);

    std::optional<uint16_t> ReadUInt16(bool advance = true);
    std::optional<uint32_t> ReadUInt32(bool advance = true);
    std::optional<uint64_t> ReadUInt64(bool advance = true);

    std::optional<const char*> ReadString(size_t length, bool advance = true);

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    std::optional<T> Read(bool advance = true) {
        if((m_view.size() - m_cursor) < sizeof(T)) {
            return {};
        }

        T object;
        memcpy(&object, m_view.data() + m_cursor, sizeof(object));

        if(advance) Advance(sizeof(object));

        return object;
    }
};

class TextualDataReader {
    DataView& m_view;

    // Actual byte offset into the file
    size_t m_cursor = 0;

    // Line inside the file 
    size_t m_line = 0;
    // Symbol in the line
    size_t m_symbol = 0;
public:
    TextualDataReader(DataView& view) : m_view(view) {}

    std::optional<std::string_view> ReadNextLine();
};