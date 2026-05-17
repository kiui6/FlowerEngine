#pragma once

#include "DataView.h"

#include <optional>

class DataReader {
    DataView& m_view;
    size_t cursor = 0;
public:
    DataReader(DataView& view) : m_view(view) {}

    inline void Advance(ptrdiff_t diff) {cursor += diff;}
    inline void SetOffset(size_t offset) {cursor = offset;}

    std::optional<std::byte> ReadByte(bool advance = true);
    std::optional<std::byte*> ReadBytes(size_t length, bool advance = true);

    std::optional<int16_t> ReadInt16(bool advance = true);
    std::optional<int32_t> ReadInt32(bool advance = true);
    std::optional<int64_t> ReadInt64(bool advance = true);

    std::optional<uint16_t> ReadUInt16(bool advance = true);
    std::optional<uint32_t> ReadUInt32(bool advance = true);
    std::optional<uint64_t> ReadUInt64(bool advance = true);

    std::optional<const char*> ReadString(size_t length, bool advance = true);

};