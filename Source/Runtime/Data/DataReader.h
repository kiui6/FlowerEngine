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

    std::optional<std::string> ReadString(size_t length, bool advance = true) {
        if((m_view.size() - m_cursor) < length) {
            return {};
        }

        std::string str;
        str.reserve(length);
        memcpy(str.data(), m_view.data() + m_cursor, length);

        if(advance) Advance(length);

        return str;
    }
    
    template <typename T>
    requires std::is_trivially_copyable_v<T>
    std::optional<std::vector<T>> ReadArray(size_t length, bool advance = true) {
        if((m_view.size() - m_cursor) < (sizeof(T) * length)) {
            return {};
        }

        std::vector<T> vector;
        vector.reserve(length);
        memcpy(vector.data(), m_view.data() + m_cursor, sizeof(T) * length);

        if(advance) Advance(sizeof(T) * length);

        return vector;
    }

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

    void NextLine();

    std::optional<std::string> ReadLine();
};