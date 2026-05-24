#include "DataReader.h"

std::optional<std::byte> DataReader::ReadByte(bool advance)
{
    return std::optional<std::byte>();
}

std::optional<std::byte *> DataReader::ReadBytes(size_t length, bool advance)
{
    if((m_view.size() - m_cursor) < (length)) {
        return {};
    }

    std::byte* data = new std::byte[length];
    memcpy(data, m_view.data() + m_cursor, length);

    if(advance) Advance(length);

    return data;
}

void TextualDataReader::NextLine()
{
    const std::byte* begin = m_view.begin();
    while(m_cursor < m_view.size() && *(begin + m_cursor) != static_cast<std::byte>('\n')) {
        m_symbol++;
        m_cursor++;
    }

    if (m_cursor < m_view.size() && begin[m_cursor] == static_cast<std::byte>('\n')) {
        m_cursor++;
        m_line++;
        m_symbol = 0;
    }
}

std::optional<std::string> TextualDataReader::ReadLine()
{
    if(m_cursor >= m_view.size()) {
        return {};
    }

    std::string value;
    size_t length;
    uint8_t hasR = false;

    const std::byte* begin = m_view.begin();
    size_t tempCursor = m_cursor;
    while(tempCursor < m_view.size() && *(begin + tempCursor) != static_cast<std::byte>('\n')) {
        tempCursor++;
        if(begin[tempCursor] == static_cast<std::byte>('\r')) {
            hasR = 1;
        }
    }

    length = tempCursor - m_cursor - hasR;
    value.resize(length);
    memcpy(value.data(), begin + m_cursor, length);

    m_cursor = tempCursor;

    if (m_cursor < m_view.size() && begin[m_cursor] == static_cast<std::byte>('\n')) {
        m_cursor++;
        m_line++;
        m_symbol = 0;
    }

    return value;
}
