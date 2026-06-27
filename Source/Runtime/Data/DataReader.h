#pragma once

#include "DataView.h"

#include <optional>
#include <type_traits>
#include <cstring>

class DataReader {
    DataView& m_view;
    size_t m_cursor = 0;
public:
    DataReader(DataView& view) : m_view(view) {}

    inline void Advance(ptrdiff_t diff) {m_cursor += diff;}
    inline void SetOffset(size_t offset) {m_cursor = offset;}

    inline size_t GetSize() {return m_view.size();}
    inline size_t GetCursor() {return m_cursor;}

    inline const std::byte* GetData() const  {return m_view.data() + m_cursor;}

    std::optional<std::byte> ReadByte(bool advance = true);
    bool ReadBytes(size_t length, void* result, bool advance = true);

    bool ReadString(size_t length, std::string& result, bool advance = true) {
        if((m_view.size() - m_cursor) < length) {
            return false;
        }

        result.reserve(length);
        memcpy(result.data(), m_view.data() + m_cursor, length);

        if(advance) Advance(length);

        return true;
    }
    
    template <typename T>
    requires std::is_trivially_copyable_v<T>
    bool ReadArray(size_t length, std::vector<T>& result, bool advance = true) {
        if((m_view.size() - m_cursor) < (sizeof(T) * length)) {
            return false;
        }

        result.reserve(length);
        memcpy(result.data(), m_view.data() + m_cursor, sizeof(T) * length);

        if(advance) Advance(sizeof(T) * length);

        return true;
    }

    template <typename T>
    requires std::is_trivially_copyable_v<T>
    bool Read(T& result, bool advance = true) {
        if((m_view.size() - m_cursor) < sizeof(T)) {
            return false;
        }

        memcpy(&result, m_view.data() + m_cursor, sizeof(result));

        if(advance) Advance(sizeof(result));

        return true;
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